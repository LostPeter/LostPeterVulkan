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
#include "../include/TextureManager.h"
#include "../include/Image.h"
#include "../include/StreamTexture.h"

namespace LostPeterEngine
{
	////////////////////////// TextureCmd //////////////////////////
	//TextureCmd_Usage
	class TextureCmd_Usage : public FParameterCommand
	{
	public:	
		virtual String DoGet(const void* pTarget) const
		{
			const Texture* pTexture = static_cast<const Texture*>(pTarget);
			return FUtilString::SaveUInt(pTexture->GetUsage());
		}
		virtual void DoSet(void* pTarget, const String& strValue)
		{
			Texture* pTexture = static_cast<Texture*>(pTarget);
			pTexture->SetUsage(FUtilString::ParserUInt(strValue));
		}
	};
	//TextureCmd_TextureType
	class TextureCmd_TextureType : public FParameterCommand
	{
	public:	
		virtual String DoGet(const void* pTarget) const
		{
			const Texture* pTexture = static_cast<const Texture*>(pTarget);
			return F_GetTextureTypeName(pTexture->GetTextureType());
		}
		virtual void DoSet(void* pTarget, const String& strValue)
		{
			Texture* pTexture = static_cast<Texture*>(pTarget);
			pTexture->SetTextureType(F_ParseTextureType(strValue));
		}
	};
	//TextureCmd_TextureFilterType
	class TextureCmd_TextureFilterType : public FParameterCommand
	{
	public:	
		virtual String DoGet(const void* pTarget) const
		{
			const Texture* pTexture = static_cast<const Texture*>(pTarget);
			return F_GetTextureFilterTypeName(pTexture->GetTextureFilterType());
		}
		virtual void DoSet(void* pTarget, const String& strValue)
		{
			Texture* pTexture = static_cast<Texture*>(pTarget);
			pTexture->SetTextureFilterType(F_ParseTextureFilterType(strValue));
		}
	};
	//TextureCmd_TextureAddressingType
	class TextureCmd_TextureAddressingType : public FParameterCommand
	{
	public:	
		virtual String DoGet(const void* pTarget) const
		{
			const Texture* pTexture = static_cast<const Texture*>(pTarget);
			return F_GetTextureAddressingTypeName(pTexture->GetTextureAddressingType());
		}
		virtual void DoSet(void* pTarget, const String& strValue)
		{
			Texture* pTexture = static_cast<Texture*>(pTarget);
			pTexture->SetTextureAddressingType(F_ParseTextureAddressingType(strValue));
		}
	};
	//TextureCmd_TextureBorderColorType
	class TextureCmd_TextureBorderColorType : public FParameterCommand
	{
	public:	
		virtual String DoGet(const void* pTarget) const
		{
			const Texture* pTexture = static_cast<const Texture*>(pTarget);
			return F_GetTextureBorderColorTypeName(pTexture->GetTextureBorderColorType());
		}
		virtual void DoSet(void* pTarget, const String& strValue)
		{
			Texture* pTexture = static_cast<Texture*>(pTarget);
			pTexture->SetTextureBorderColorType(F_ParseTextureBorderColorType(strValue));
		}
	};
	//TextureCmd_MSAASampleCountType
	class TextureCmd_MSAASampleCountType : public FParameterCommand
	{
	public:	
		virtual String DoGet(const void* pTarget) const
		{
			const Texture* pTexture = static_cast<const Texture*>(pTarget);
			return F_GetMSAASampleCountTypeName(pTexture->GetMSAASampleCountType());
		}
		virtual void DoSet(void* pTarget, const String& strValue)
		{
			Texture* pTexture = static_cast<Texture*>(pTarget);
			pTexture->SetMSAASampleCountType(F_ParseMSAASampleCountType(strValue));
		}
	};
	//TextureCmd_PixelFormatType
	class TextureCmd_PixelFormatType : public FParameterCommand
	{
	public:	
		virtual String DoGet(const void* pTarget) const
		{
			const Texture* pTexture = static_cast<const Texture*>(pTarget);
			return FPixelFormat::GetPixelFormatName(pTexture->GetPixelFormat());
		}
		virtual void DoSet(void* pTarget, const String& strValue)
		{
			Texture* pTexture = static_cast<Texture*>(pTarget);
			pTexture->SetPixelFormat(FPixelFormat::ParsePixelFormatFromName(strValue));
		}
	};
	//TextureCmd_Width
	class TextureCmd_Width : public FParameterCommand
	{
	public:	
		virtual String DoGet(const void* pTarget) const
		{
			const Texture* pTexture = static_cast<const Texture*>(pTarget);
			return FUtilString::SaveUInt(pTexture->GetWidth());
		}
		virtual void DoSet(void* pTarget, const String& strValue)
		{
			Texture* pTexture = static_cast<Texture*>(pTarget);
			pTexture->SetWidth(FUtilString::ParserUInt(strValue));
		}
	};
	//TextureCmd_Height
	class TextureCmd_Height : public FParameterCommand
	{
	public:	
		virtual String DoGet(const void* pTarget) const
		{
			const Texture* pTexture = static_cast<const Texture*>(pTarget);
			return FUtilString::SaveUInt(pTexture->GetHeight());
		}
		virtual void DoSet(void* pTarget, const String& strValue)
		{
			Texture* pTexture = static_cast<Texture*>(pTarget);
			pTexture->SetHeight(FUtilString::ParserUInt(strValue));
		}
	};
	//TextureCmd_Depth
	class TextureCmd_Depth : public FParameterCommand
	{
	public:	
		virtual String DoGet(const void* pTarget) const
		{
			const Texture* pTexture = static_cast<const Texture*>(pTarget);
			return FUtilString::SaveUInt(pTexture->GetDepth());
		}
		virtual void DoSet(void* pTarget, const String& strValue)
		{
			Texture* pTexture = static_cast<Texture*>(pTarget);
			pTexture->SetDepth(FUtilString::ParserUInt(strValue));
		}
	};
	//TextureCmd_BitDepthInteger
	class TextureCmd_BitDepthInteger : public FParameterCommand
	{
	public:	
		virtual String DoGet(const void* pTarget) const
		{
			const Texture* pTexture = static_cast<const Texture*>(pTarget);
			return FUtilString::SaveUInt16(pTexture->GetBitDepthIntegerDesired());
		}
		virtual void DoSet(void* pTarget, const String& strValue)
		{
			Texture* pTexture = static_cast<Texture*>(pTarget);
			pTexture->SetBitDepthIntegerDesired(FUtilString::ParserUInt16(strValue));
		}
	};
	//TextureCmd_BitDepthFloat
	class TextureCmd_BitDepthFloat : public FParameterCommand
	{
	public:	
		virtual String DoGet(const void* pTarget) const
		{
			const Texture* pTexture = static_cast<const Texture*>(pTarget);
			return FUtilString::SaveUInt16(pTexture->GetBitDepthFloatDesired());
		}
		virtual void DoSet(void* pTarget, const String& strValue)
		{
			Texture* pTexture = static_cast<Texture*>(pTarget);
			pTexture->SetBitDepthFloatDesired(FUtilString::ParserUInt16(strValue));
		}
	};
	//TextureCmd_IsTreatLuminanceAsAlpha
	class TextureCmd_IsTreatLuminanceAsAlpha : public FParameterCommand
	{
	public:	
		virtual String DoGet(const void* pTarget) const
		{
			const Texture* pTexture = static_cast<const Texture*>(pTarget);
			return FUtilString::SaveBool(pTexture->IsTreatLuminanceAsAlpha());
		}
		virtual void DoSet(void* pTarget, const String& strValue)
		{
			Texture* pTexture = static_cast<Texture*>(pTarget);
			pTexture->SetIsTreatLuminanceAsAlpha(FUtilString::ParserBool(strValue));
		}
	};
	//TextureCmd_MipMapsCount
	class TextureCmd_MipMapsCount : public FParameterCommand
	{
	public:	
		virtual String DoGet(const void* pTarget) const
		{
			const Texture* pTexture = static_cast<const Texture*>(pTarget);
			return FUtilString::SaveUInt(pTexture->GetMipMapsCount());
		}
		virtual void DoSet(void* pTarget, const String& strValue)
		{
			Texture* pTexture = static_cast<Texture*>(pTarget);
			pTexture->SetMipMapsCount(FUtilString::ParserUInt(strValue));
		}
	};
	//TextureCmd_IsMipMapsHardwareGenerated
	class TextureCmd_IsMipMapsHardwareGenerated : public FParameterCommand
	{
	public:	
		virtual String DoGet(const void* pTarget) const
		{
			const Texture* pTexture = static_cast<const Texture*>(pTarget);
			return FUtilString::SaveBool(pTexture->IsMipMapsHardwareGenerated());
		}
		virtual void DoSet(void* pTarget, const String& strValue)
		{
			Texture* pTexture = static_cast<Texture*>(pTarget);
			pTexture->SetIsMipMapsHardwareGenerated(FUtilString::ParserBool(strValue));
		}
	};
	//TextureCmd_Gamma
	class TextureCmd_Gamma : public FParameterCommand
	{
	public:	
		virtual String DoGet(const void* pTarget) const
		{
			const Texture* pTexture = static_cast<const Texture*>(pTarget);
			return FUtilString::SaveFloat(pTexture->GetGamma());
		}
		virtual void DoSet(void* pTarget, const String& strValue)
		{
			Texture* pTexture = static_cast<Texture*>(pTarget);
			pTexture->SetGamma(FUtilString::ParserFloat(strValue));
		}
	};
	//TextureCmd_IsGammaHardware
	class TextureCmd_IsGammaHardware : public FParameterCommand
	{
	public:	
		virtual String DoGet(const void* pTarget) const
		{
			const Texture* pTexture = static_cast<const Texture*>(pTarget);
			return FUtilString::SaveBool(pTexture->IsGammaHardware());
		}
		virtual void DoSet(void* pTarget, const String& strValue)
		{
			Texture* pTexture = static_cast<Texture*>(pTarget);
			pTexture->SetIsGammaHardware(FUtilString::ParserBool(strValue));
		}
	};
	

	////////////////////////// Texture /////////////////////////////
	static TextureCmd_Usage s_TextureCmd_Usage;
	static TextureCmd_TextureType s_TextureCmd_TextureType;
	static TextureCmd_TextureFilterType s_TextureCmd_TextureFilterType;
	static TextureCmd_TextureAddressingType s_TextureCmd_TextureAddressingType;
	static TextureCmd_TextureBorderColorType s_TextureCmd_TextureBorderColorType;
	static TextureCmd_MSAASampleCountType s_TextureCmd_MSAASampleCountType;
	static TextureCmd_PixelFormatType s_TextureCmd_PixelFormatType;
	static TextureCmd_Width s_TextureCmd_Width;
	static TextureCmd_Height s_TextureCmd_Height;
	static TextureCmd_Depth s_TextureCmd_Depth;
	static TextureCmd_BitDepthInteger s_TextureCmd_BitDepthInteger;
	static TextureCmd_BitDepthFloat s_TextureCmd_BitDepthFloat;
	static TextureCmd_IsTreatLuminanceAsAlpha s_TextureCmd_IsTreatLuminanceAsAlpha;
	static TextureCmd_MipMapsCount s_TextureCmd_MipMapsCount;
	static TextureCmd_IsMipMapsHardwareGenerated s_TextureCmd_IsMipMapsHardwareGenerated;
	static TextureCmd_Gamma s_TextureCmd_Gamma;
	static TextureCmd_IsGammaHardware s_TextureCmd_IsGammaHardware;


	const String Texture::ms_nameTexture = "Texture";
    Texture::Texture(ResourceManager* pResourceManager,
					 uint32 nGroup, 
					 const String& strName,
					 const String& strGroupName,
					 ResourceHandle nHandle,
					 bool bIsManualLoad /*= false*/,
					 ResourceManualLoader* pResourceManualLoader /*= nullptr*/)
		: Resource(pResourceManager,
				   nGroup, 
				   strName,
				   strGroupName,
				   nHandle,
				   bIsManualLoad,
				   pResourceManualLoader)
		, m_nUsage(TextureManager::ms_nUsage_Default)
		, m_eTexture(TextureManager::ms_eTexture_Default)
		, m_eTextureFilter(TextureManager::ms_eTextureFilter_Default)
		, m_eTextureAddressing(TextureManager::ms_eTextureAddressing_Default)
		, m_eTextureBorderColor(TextureManager::ms_eTextureBorderColor_Default)
		, m_eMSAASampleCount(TextureManager::ms_eMSAASampleCount_Default)
		, m_ePixelFormatDesired(TextureManager::ms_ePixelFormat_Default)
		, m_ePixelFormatSrc(TextureManager::ms_ePixelFormat_Default)
		, m_ePixelFormat(TextureManager::ms_ePixelFormat_Default)
		, m_nWidth(TextureManager::ms_nWidth_Default)
		, m_nHeight(TextureManager::ms_nHeight_Default)
		, m_nDepth(TextureManager::ms_nDepth_Default)
		, m_nWidthSrc(TextureManager::ms_nWidth_Default)
		, m_nHeightSrc(TextureManager::ms_nHeight_Default)
		, m_nDepthSrc(TextureManager::ms_nDepth_Default)
		, m_nBitDepthIntegerDesired(TextureManager::ms_nBitDepthInteger_Default)
		, m_nBitDepthFloatDesired(TextureManager::ms_nBitDepthFloat_Default)
		, m_bIsTreatLuminanceAsAlpha(TextureManager::ms_bIsTreatLuminanceAsAlpha_Default)
		, m_nMipMapsCountRequested(TextureManager::ms_nMipMapsCount_Default)
		, m_nMipMapsCount(TextureManager::ms_nMipMapsCount_Default)
		, m_bIsMipMapsHardwareGenerated(TextureManager::ms_bIsMipMapsHardwareGenerated_Default)
		, m_fGamma(TextureManager::ms_fGamma_Default)
		, m_bIsGammaHardware(TextureManager::ms_bIsGammaHardware_Default)
		, m_bInternalResourcesCreated(false)
	{
		if (createParameterDictionary(ms_nameTexture))
		{
			addParameterBase();
		}
	}
		void Texture::addParameterBase()
		{
			FParameterDictionary* pDictionary = GetParameterDictionary();
			pDictionary->AddParameter(E_GetTextureParamTypeName(E_TextureParam_Usage), TextureManager::GetTextureParamValue(E_TextureParam_Usage), F_Parameter_UInt, &s_TextureCmd_Usage);
			pDictionary->AddParameter(E_GetTextureParamTypeName(E_TextureParam_TextureType), TextureManager::GetTextureParamValue(E_TextureParam_TextureType), F_Parameter_Int, &s_TextureCmd_TextureType);
			pDictionary->AddParameter(E_GetTextureParamTypeName(E_TextureParam_TextureFilterType), TextureManager::GetTextureParamValue(E_TextureParam_TextureFilterType), F_Parameter_Int, &s_TextureCmd_TextureFilterType);
			pDictionary->AddParameter(E_GetTextureParamTypeName(E_TextureParam_TextureAddressingType), TextureManager::GetTextureParamValue(E_TextureParam_TextureAddressingType), F_Parameter_Int, &s_TextureCmd_TextureAddressingType);
			pDictionary->AddParameter(E_GetTextureParamTypeName(E_TextureParam_TextureBorderColorType), TextureManager::GetTextureParamValue(E_TextureParam_TextureBorderColorType), F_Parameter_Int, &s_TextureCmd_TextureBorderColorType);
			pDictionary->AddParameter(E_GetTextureParamTypeName(E_TextureParam_MSAASampleCountType), TextureManager::GetTextureParamValue(E_TextureParam_MSAASampleCountType), F_Parameter_Int, &s_TextureCmd_MSAASampleCountType);
			pDictionary->AddParameter(E_GetTextureParamTypeName(E_TextureParam_PixelFormatType), TextureManager::GetTextureParamValue(E_TextureParam_PixelFormatType), F_Parameter_Int, &s_TextureCmd_PixelFormatType);
			pDictionary->AddParameter(E_GetTextureParamTypeName(E_TextureParam_Width), TextureManager::GetTextureParamValue(E_TextureParam_Width), F_Parameter_UInt, &s_TextureCmd_Width);
			pDictionary->AddParameter(E_GetTextureParamTypeName(E_TextureParam_Height), TextureManager::GetTextureParamValue(E_TextureParam_Height), F_Parameter_UInt, &s_TextureCmd_Height);
			pDictionary->AddParameter(E_GetTextureParamTypeName(E_TextureParam_Depth), TextureManager::GetTextureParamValue(E_TextureParam_Depth), F_Parameter_UInt, &s_TextureCmd_Depth);
			pDictionary->AddParameter(E_GetTextureParamTypeName(E_TextureParam_BitDepthInteger), TextureManager::GetTextureParamValue(E_TextureParam_BitDepthInteger), F_Parameter_UShort, &s_TextureCmd_BitDepthInteger);
			pDictionary->AddParameter(E_GetTextureParamTypeName(E_TextureParam_BitDepthFloat), TextureManager::GetTextureParamValue(E_TextureParam_BitDepthFloat), F_Parameter_UShort, &s_TextureCmd_BitDepthFloat);
			pDictionary->AddParameter(E_GetTextureParamTypeName(E_TextureParam_IsTreatLuminanceAsAlpha), TextureManager::GetTextureParamValue(E_TextureParam_IsTreatLuminanceAsAlpha), F_Parameter_Bool, &s_TextureCmd_IsTreatLuminanceAsAlpha);
			pDictionary->AddParameter(E_GetTextureParamTypeName(E_TextureParam_MipMapsCount), TextureManager::GetTextureParamValue(E_TextureParam_MipMapsCount), F_Parameter_UInt, &s_TextureCmd_MipMapsCount);
			pDictionary->AddParameter(E_GetTextureParamTypeName(E_TextureParam_IsMipMapsHardwareGenerated), TextureManager::GetTextureParamValue(E_TextureParam_IsMipMapsHardwareGenerated), F_Parameter_Bool, &s_TextureCmd_IsMipMapsHardwareGenerated);
			pDictionary->AddParameter(E_GetTextureParamTypeName(E_TextureParam_Gamma), TextureManager::GetTextureParamValue(E_TextureParam_Gamma), F_Parameter_Float, &s_TextureCmd_Gamma);
			pDictionary->AddParameter(E_GetTextureParamTypeName(E_TextureParam_IsGammaHardware), TextureManager::GetTextureParamValue(E_TextureParam_IsGammaHardware), F_Parameter_Bool, &s_TextureCmd_IsGammaHardware);
		}

	Texture::~Texture()
	{

	}

	void Texture::Destroy()
	{

		Resource::Destroy();
	}
	
	uint32 Texture::GetFacesCount() const
	{
		return GetTextureType() == F_Texture_CubeMap ? 6 : 1;
	}

	bool Texture::HasAlpha() const
	{
		return FPixelFormat::HasAlpha(m_ePixelFormat);
	}

	bool Texture::LoadFromRawData(FFileMemory* pInput, uint32 nWidth, uint32 nHeight, FPixelFormatType ePixelFormat)
	{
		Image image;
		if (!image.LoadRawData(pInput, nWidth, nHeight, ePixelFormat))
		{
			F_LogError("*********************** Texture::LoadFromRawData: Load image form raw data failed, [%u - %u] - [%s] !", nWidth, nWidth, FPixelFormat::GetPixelFormatName(ePixelFormat).c_str());
			return false;
		}
		return LoadFromImage(&image);
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

		m_nWidthSrc = m_nWidth  = aImages[0]->GetWidth();
		m_nHeightSrc = m_nHeight = aImages[0]->GetHeight();
		m_nDepthSrc = m_nDepth  = aImages[0]->GetDepth();
		m_ePixelFormatSrc = aImages[0]->GetPixelFormat();
		if (m_bIsTreatLuminanceAsAlpha && m_ePixelFormatSrc == F_PixelFormat_BYTE_L8_UNORM)
		{
			m_ePixelFormatSrc = F_PixelFormat_BYTE_A8_UNORM;
		}

		if (m_ePixelFormatDesired != F_PixelFormat_Unknown)
		{
			m_ePixelFormat = m_ePixelFormatDesired;
		}
		else
		{
			m_ePixelFormat = FPixelFormat::ParsePixelFormatForBitDepths(m_ePixelFormatSrc, m_nBitDepthIntegerDesired, m_nBitDepthFloatDesired);
		}

		uint32 imageMips = aImages[0]->GetMipMapsCount();
		if (imageMips > 0) 
		{
			m_nMipMapsCount = m_nMipMapsCountRequested = imageMips;
			m_nUsage &= ~E_TextureUsage_AutoMipMap;
		}

		createInternalResources();

		uint32 faces;
		bool multiImage;
		if(aImages.size() > 1)
		{
			faces = (uint32)aImages.size();
			multiImage = true;
		}
		else
		{
			faces = aImages[0]->GetFacesCount();
			multiImage = false;
		}

		if(faces > GetFacesCount())
			faces = GetFacesCount();

		std::ostringstream str;
		str << "Texture: '" << GetName() << "': Loading " << faces << " faces"
			<< "(" << FPixelFormat::GetPixelFormatName(aImages[0]->GetPixelFormat()) << "," <<
			aImages[0]->GetWidth() << "x" << aImages[0]->GetHeight() << "x" << aImages[0]->GetDepth() <<
			") with ";
		if (!(m_bIsMipMapsHardwareGenerated && m_nMipMapsCount == 0))
			str << m_nMipMapsCount;
		if (m_nUsage & E_TextureUsage_AutoMipMap)
		{
			if (m_bIsMipMapsHardwareGenerated)
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


		for (uint32 mip = 0; mip <= imageMips; ++mip)
		{
			for(uint32 i = 0; i < faces; ++i)
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
					FFileMemory* pFM = FFileManager::GetSingleton().CreateFileMemory((uint32)FPixelFormat::GetPixelFormatMemorySize(
																					 src.GetWidth(), 
																					 src.GetHeight(), 
																					 src.GetDepth(), 
																					 src.m_ePixelFormat));

					FPixelBox corrected = FPixelBox(src.GetWidth(), src.GetHeight(), src.GetDepth(), src.m_ePixelFormat, pFM->GetBuffer());
					FPixelFormat::BulkPixelConversion(src,corrected);

					Image::ApplyGamma(static_cast<uint8*>(corrected.m_pData), 
									  m_fGamma, 
									  (uint32)corrected.GetConsecutiveSize(), 
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

		m_nSize = GetFacesCount() * (uint32)FPixelFormat::GetPixelFormatMemorySize(m_nWidth, m_nHeight, m_nDepth, m_ePixelFormat);
		return true;
	}

	void Texture::loadImpl()
	{
		createInternalResources();
	}
	void Texture::unloadImpl()
	{
		destroyInternalResources();
	}

	uint32 Texture::calculateSize() const
	{
		 return GetFacesCount() * (uint32)FPixelFormat::GetPixelFormatMemorySize(m_nWidth, m_nHeight, m_nDepth, m_ePixelFormat);
	}

	void Texture::destroyInternalResources()
	{
		if (m_bInternalResourcesCreated)
		{
			destroyInternalResourcesImpl();
			m_bInternalResourcesCreated = false;

			if (m_eResourceLoading.load() != E_ResourceLoading_Unloading)
            {
                m_eResourceLoading.store(E_ResourceLoading_Unloaded);
                _FireUnloadingComplete();
            }
		}
	}

	bool Texture::createInternalResources()
	{
		if (!m_bInternalResourcesCreated)
		{
			createInternalResourcesImpl();
			m_bInternalResourcesCreated = true;

			if (!IsLoading())
            {
                m_eResourceLoading.store(E_ResourceLoading_Loaded);
                _FireLoadingComplete(false);
            }
		}
		return true;
	}

}; //LostPeterEngine