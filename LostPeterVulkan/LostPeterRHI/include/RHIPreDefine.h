/****************************************************************************
* LostPeterRHI - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_PRE_DEFINE_H_
#define _RHI_PRE_DEFINE_H_

#include "FPreDefine.h"
#include "FPreInclude.h"
using namespace LostPeterFoundation;



#if F_PLATFORM == F_PLATFORM_WINDOW
	#if defined (RHI_EXPORTS)
		#define rhiExport            	__declspec(dllexport)
	#else
		#define rhiExport            	__declspec(dllimport)
	#endif
#elif F_PLATFORM == F_PLATFORM_ANDROID
	#define rhiExport			    	__attribute__ ((visibility("default")))
#else
	#define rhiExport
#endif


namespace LostPeterRHI
{
////////////////////////////// Define //////////////////////////////


////////////////////////////// Typedef /////////////////////////////


////////////////////////////// Enum ////////////////////////////////
	using RHIEnumType = uint32;

	enum class RHIType : RHIEnumType 
	{
		RHI_Dummy = 0,									//0: Dummy
		RHI_Vulkan,										//1: Vulkan
		RHI_DirectX12,									//2: DirectX12
		RHI_Metal,										//3: Metal

        RHI_Count,
    };


	enum class RHIPhysicalDeviceType : RHIEnumType 
	{
        RHI_PhysicalDevice_Hardware = 0,				//0: Hardware
        RHI_PhysicalDevice_Software,					//1: Software

        RHI_PhysicalDevice_Count,
    };


	enum class RHIIndexType : RHIEnumType 
	{
		RHI_Index_16Bit = 0,                            //0: 16Bit
		RHI_Index_32Bit,                                //1: 32Bit

		RHI_Index_Count,
	};


	enum class RHIBufferViewType : RHIEnumType 
	{
		RHI_BufferView_Vertex = 0,                    	//0: Vertex
		RHI_BufferView_Index,                           //1: Index
		RHI_BufferView_UniformBinding,					//2: UniformBinding
		RHI_BufferView_StorageBinding,					//3: StorageBinding

		RHI_BufferView_Count,
	};




////////////////////////////// Struct //////////////////////////////
	struct RHIPhysicalDeviceProperty
	{
        uint32 nVendorID;
        uint32 nDeviceID;
        RHIPhysicalDeviceType ePhysicalDevice;
    };


	struct RHIVertexBufferViewInfo 
	{
        uint32 nStride;
    };


    struct RHIIndexBufferViewInfo 
	{
        RHIIndexType eIndex;
    };


    struct RHIBufferViewCreateInfo 
	{
        RHIBufferViewType eBufferView;
        uint32 nOffset;
        uint32 nSize;
        union 
		{
            RHIVertexBufferViewInfo eVertexBufferViewInfo;
            RHIIndexBufferViewInfo eIndexBufferViewInfo;
        };
    };

    
////////////////////////////// Class ///////////////////////////////
	class RHIBuffer;
	class RHIBufferView;
	class RHICommandBuffer;
	class RHICommandPool;
	class RHIDescriptorSetLayout;
	class RHIDescriptorSetLayoutCache;
	class RHIDevice;
	class RHIFence;
	class RHIFramebuffer;
	class RHIGraphicsState;
	class RHIImage;
	class RHIImageView;
	class RHIInstance;
	class RHIPhysicalDevice;
	class RHIPipeline;
	class RHIPipelineBarriers;
	class RHIPipelineCache;
	class RHIQueue;
	class RHIRenderPassCache;
	class RHIResourceBindings;
	class RHIShaderModule;
	class RHIStreamDecoder;
	class RHIStreamEncoder;
	class RHISurface;
	class RHISwapchain;
	class RHISyncPrimitivesPool;
	class RHIUtil;
	class RHIVertexInputFormat;

	typedef std::vector<RHIBuffer*> RHIBufferPtrVector;

}; //LostPeterRHI

#endif