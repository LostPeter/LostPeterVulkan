/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/Texture.h"
#include "../include/Image.h"
#include "../include/StreamTexture.h"

namespace LostPeterEngine
{
    Texture::Texture(uint32 nGroup, const String& strName)
		: Base(nGroup, strName)
		, m_eTexture(F_Texture_2D)
		, m_nUsage(E_TextureUsage_Default)
		, m_bIsLoaded(false)
		, m_bIsManual(false)
		, m_ePixelFormat(F_PixelFormat_Unknown)
		, m_nWidth(512)
		, m_nHeight(512)
		, m_nDepth(1)
		, m_nSize(0)
		, m_ePixelFormatSrc(F_PixelFormat_Unknown)
		, m_nSrcWidth(0)
		, m_nSrcHeight(0)
		, m_nSrcDepth(0)
		, m_ePixelFormatDesired(F_PixelFormat_Unknown)
		, m_nDesiredIntegerBitDepth(0)
		, m_nDesiredFloatBitDepth(0)
		, m_bTreatLuminanceAsAlpha(false)
		, m_nNumRequestedMipMaps(0)
		, m_nNumMipMaps(0)
		, m_bMipMapsHardwareGenerated(false)
		, m_fGamma(1.0f)
		, m_nFSAA(0)
		, m_bInternalResourcesCreated(false)
	{

	}

	Texture::~Texture()
	{

	}
	
	size_t Texture::GetNumFaces() const
	{
		return GetTextureType() == F_Texture_CubeMap ? 6 : 1;
	}

	bool Texture::HasAlpha() const
	{
		return FPixelFormat::HasAlpha(m_ePixelFormat);
	}

	size_t Texture::CalculateSize() const
	{
		 return GetNumFaces() * FPixelFormat::GetPixelFormatMemorySize(m_nWidth, m_nHeight, m_nDepth, m_ePixelFormat);
	}

	bool Texture::LoadFromRawData(FFileMemory* pInput, size_t nWidth, size_t nHeight, FPixelFormatType ePixelFormat)
	{
		Image img;
		if (!img.LoadRawData(pInput, nWidth, nHeight, ePixelFormat))
		{
			F_LogError("Texture::LoadFromRawData: Create image form raw data failed !");
			F_Assert(false && "Texture::LoadFromRawData: Create image form raw data failed !")
			return false;
		}
		return LoadFromImage(&img);
	}

	bool Texture::LoadFromImage(Image* pImage)
	{
		F_Assert(pImage && "Texture::LoadFromImage")

		ImagePtrVector aImages;
		aImages.push_back(pImage);
		return LoadFromImages(aImages);
	}

	bool Texture::LoadFromImages(const ImagePtrVector& aImages)
	{
		if (aImages.empty())
			return false;

		m_nSrcWidth	 = m_nWidth  = aImages[0]->GetWidth();
		m_nSrcHeight = m_nHeight = aImages[0]->GetHeight();
		m_nSrcDepth	 = m_nDepth  = aImages[0]->GetDepth();
		m_ePixelFormatSrc = aImages[0]->GetPixelFormat();
		if (m_bTreatLuminanceAsAlpha && m_ePixelFormatSrc == F_PixelFormat_BYTE_L8_UNORM)
		{
			m_ePixelFormatSrc = F_PixelFormat_BYTE_A8_UNORM;
		}

		if (m_ePixelFormatDesired != F_PixelFormat_Unknown)
		{
			m_ePixelFormat = m_ePixelFormatDesired;
		}
		else
		{
			m_ePixelFormat = FPixelFormat::ParsePixelFormatForBitDepths(m_ePixelFormatSrc, m_nDesiredIntegerBitDepth, m_nDesiredFloatBitDepth);
		}

		size_t imageMips = aImages[0]->GetNumMipMaps();
		if (imageMips > 0) 
		{
			m_nNumMipMaps = m_nNumRequestedMipMaps = imageMips;
			m_nUsage &= ~E_TextureUsage_AutoMipMap;
		}

		CreateInternalResources();

		size_t faces;
		bool multiImage;
		if(aImages.size() > 1)
		{
			faces = aImages.size();
			multiImage = true;
		}
		else
		{
			faces = aImages[0]->GetNumFaces();
			multiImage = false;
		}

		if(faces > GetNumFaces())
			faces = GetNumFaces();

		std::ostringstream str;
		str << "Texture: '" << m_strName << "': Loading " << faces << " faces"
			<< "(" << FPixelFormat::GetPixelFormatName(aImages[0]->GetPixelFormat()) << "," <<
			aImages[0]->GetWidth() << "x" << aImages[0]->GetHeight() << "x" << aImages[0]->GetDepth() <<
			") with ";
		if (!(m_bMipMapsHardwareGenerated && m_nNumMipMaps == 0))
			str << m_nNumMipMaps;
		if (m_nUsage & E_TextureUsage_AutoMipMap)
		{
			if (m_bMipMapsHardwareGenerated)
				str << " hardware";

			str << " generated mipmaps";
		}
		else
		{
			str << " custom mipmaps";
		}
		if (multiImage)
			str << " from multiple Images !";
		else
			str << " from Image !";

        StreamTexture* pTexStream = GetTextureStream(0, 0);
        str << " Internal format is " << FPixelFormat::GetPixelFormatName(pTexStream->GetPixelFormat()) << 
            "," << pTexStream->GetWidth() << "x" << pTexStream->GetHeight() << "x" << pTexStream->GetDepth() << ".";
		
		F_LogInfo(str.str().c_str());


		for (size_t mip = 0; mip <= imageMips; ++mip)
		{
			for(size_t i = 0; i < faces; ++i)
			{
				FPixelBox src;
				if (multiImage)
				{
					if (!aImages[i]->GetPixelBox(src, 0, mip))
					{
						F_Assert(false && "Texture::LoadFromImages: Load from multiple images failed !")
						return false;
					}
				}
				else
				{
					if (!aImages[0]->GetPixelBox(src, i, mip))
					{
						F_Assert(false && "Texture::LoadFromImages: Load from faces of aImages[0] failed !")
						return false;
					}
				}

				src.m_ePixelFormat = m_ePixelFormatSrc;
				if (m_fGamma != 1.0f) 
				{
					FFileMemory* pFM = FFileManager::GetSingleton().CreateFileMemory(FPixelFormat::GetPixelFormatMemorySize(
						src.GetWidth(), src.GetHeight(), src.GetDepth(), src.m_ePixelFormat));

					FPixelBox corrected = FPixelBox(src.GetWidth(), src.GetHeight(), src.GetDepth(), src.m_ePixelFormat, pFM->GetBuffer());
					FPixelFormat::BulkPixelConversion(src,corrected);

					Image::ApplyGamma(static_cast<uint8*>(corrected.m_pData), m_fGamma, corrected.GetConsecutiveSize(), 
						static_cast<uint8>(FPixelFormat::GetPixelFormatElemBits(src.m_ePixelFormat)));

					GetTextureStream(i, mip)->BlitFromMemory(corrected);
					FFileManager::GetSingleton().DeleteFileMemory(pFM);
				}
				else 
				{
					GetTextureStream(i, mip)->BlitFromMemory(src);
				}
			}
		}

		m_nSize = GetNumFaces() * FPixelFormat::GetPixelFormatMemorySize(m_nWidth, m_nHeight, m_nDepth, m_ePixelFormat);
		return true;
	}

	bool Texture::CreateInternalResources()
	{
		if (!m_bInternalResourcesCreated)
		{
			createInternalResourcesImpl();
			m_bInternalResourcesCreated = true;
		}
		return true;
	}

	void Texture::FreeInternalResources()
	{
		if (m_bInternalResourcesCreated)
		{
			freeInternalResourcesImpl();
			m_bInternalResourcesCreated = false;
		}
	}

}; //LostPeterEngine