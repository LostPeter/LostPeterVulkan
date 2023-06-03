/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-05-19
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/PreInclude.h"
#include "../include/VulkanPixelBox.h"
#include "../include/VulkanPixelFormat.h"

namespace LostPeter
{
    VulkanPixelBox::VulkanPixelBox()
	   : m_pData(nullptr)
	   , m_ePixelFormat(Vulkan_PixelFormat_Unknown)
	   , m_nRowPitch(0)
	   , m_nSlicePitch(0)
	{

	}

	VulkanPixelBox::VulkanPixelBox(const FBox& extents, VulkanPixelFormatType ePixelFormat, void* pPixelData /*= nullptr*/)
		: FBox(extents)
		, m_pData((uint8*)pPixelData)
		, m_ePixelFormat(ePixelFormat)
	{
		SetConsecutive();
	}

	VulkanPixelBox::VulkanPixelBox(size_t width, size_t height, size_t depth, VulkanPixelFormatType ePixelFormat, void* pPixelData /*= nullptr*/)
		: FBox(0, 0, 0, width, height, depth)
		, m_pData((uint8*)pPixelData)
		, m_ePixelFormat(ePixelFormat)
	{
		SetConsecutive();
	}

    VulkanPixelBox::~VulkanPixelBox()
    {
        
    }

    size_t VulkanPixelBox::GetConsecutiveSize() const
	{
		return VulkanPixelFormat::GetPixelFormatMemorySize(GetWidth(), GetHeight(), GetDepth(), m_ePixelFormat);
	}

	VulkanPixelBox VulkanPixelBox::GetSubVolume(const FBox& def, bool resetOrigin /*= true*/) const
	{
		if (VulkanPixelFormat::IsCompressed(m_ePixelFormat))
		{
			if (def.m_nLeft == m_nLeft && def.m_nTop == m_nTop && def.m_nFront == m_nFront &&
				def.m_nRight == m_nRight && def.m_nBottom == m_nBottom && def.m_nBack == m_nBack)
			{
				return *this;
			}
			F_LogError("VulkanPixelBox::GetSubVolume: Cannot return subvolume of compressed PixelBuffer");
			assert(false && "VulkanPixelBox::GetSubVolume");
		}

		if (!Contains(def))
		{
			F_LogError("VulkanPixelBox::GetSubVolume: Bounds out of range");
			assert(false && "VulkanPixelBox::GetSubVolume");
		}

		const size_t elemSize = VulkanPixelFormat::GetPixelFormatElemBytes(m_ePixelFormat);
		VulkanPixelBox rval(def, m_ePixelFormat, m_pData);
		rval.m_nRowPitch = m_nRowPitch;
		rval.m_nSlicePitch = m_nSlicePitch;

		if (resetOrigin)
        {
            if (VulkanPixelFormat::IsCompressed(m_ePixelFormat))
            {
                if(rval.m_nFront > 0)
                {
                    rval.m_pData = (uint8*)rval.m_pData + rval.m_nFront * VulkanPixelFormat::GetPixelFormatMemorySize(GetWidth(), GetHeight(), 1, m_ePixelFormat);
                    rval.m_nBack -= rval.m_nFront;
                    rval.m_nFront = 0;
                }
            }
            else
            {
                rval.m_pData = rval.GetTopLeftFrontPixelPtr();
                rval.m_nRight -= rval.m_nLeft;
                rval.m_nBottom -= rval.m_nTop;
                rval.m_nBack -= rval.m_nFront;
                rval.m_nFront = rval.m_nTop = rval.m_nLeft = 0;
            }
        }

		return rval;
	}

	uint8* VulkanPixelBox::GetTopLeftFrontPixelPtr() const
	{
		return m_pData + (m_nLeft + m_nTop * m_nRowPitch + m_nFront * m_nSlicePitch) * VulkanPixelFormat::GetPixelFormatElemBytes(m_ePixelFormat);
	}

	FColor VulkanPixelBox::GetColorAt(size_t x, size_t y, size_t z) const
	{
		FColor cv;
        size_t pixelSize = VulkanPixelFormat::GetPixelFormatElemBytes(m_ePixelFormat);
        size_t pixelOffset = pixelSize * (z * m_nSlicePitch + y * m_nRowPitch + x);
        VulkanPixelFormat::UnpackColor(&cv, m_ePixelFormat, (uint8*)m_pData + pixelOffset);

        return cv;
	}

	void VulkanPixelBox::SetColorAt(size_t x, size_t y, size_t z, const FColor &cv)
	{
		size_t pixelSize = VulkanPixelFormat::GetPixelFormatElemBytes(m_ePixelFormat);
        size_t pixelOffset = pixelSize * (z * m_nSlicePitch + y * m_nRowPitch + x);
        VulkanPixelFormat::PackColor(cv, m_ePixelFormat, (uint8*)m_pData + pixelOffset);
	}

}; //LostPeter