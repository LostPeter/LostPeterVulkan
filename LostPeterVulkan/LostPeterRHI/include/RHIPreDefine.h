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

	//RHIType
	enum class RHIType : RHIEnumType 
	{
		RHI_Dummy = 0,									//0: Dummy
		RHI_Vulkan,										//1: Vulkan
		RHI_DirectX12,									//2: DirectX12
		RHI_Metal,										//3: Metal

        RHI_Count,
    };
	rhiExport const String& RHI_GetRHITypeName(RHIType type);
    rhiExport const String& RHI_GetRHITypeName(int type);
    rhiExport RHIType RHI_ParseRHIType(const String& strName);

	//RHIPhysicalDeviceType
	enum class RHIPhysicalDeviceType : RHIEnumType 
	{
        RHI_PhysicalDevice_Hardware = 0,				//0: Hardware
        RHI_PhysicalDevice_Software,					//1: Software

        RHI_PhysicalDevice_Count,
    };

	//RHIQueueType
	enum class RHIQueueType : RHIEnumType 
	{
        RHI_Queue_Graphics = 0,							//0: Graphics
        RHI_Queue_Compute,								//1: Compute	
        RHI_Queue_Transfer,								//2: Transfer
        
		RHI_Queue_Count,
    };

	//RHIPresentType
	enum class RHIPresentType : RHIEnumType 
	{
        RHI_Present_Immediately = 0,					//0: Immediately
        RHI_Present_VSync,								//1: VSync

        RHI_Present_Count,
    };

	//RHIIndexType
	enum class RHIIndexType : RHIEnumType 
	{
		RHI_Index_16Bit = 0,                            //0: 16Bit
		RHI_Index_32Bit,                                //1: 32Bit

		RHI_Index_Count,
	};

	//RHIBufferViewType
	enum class RHIBufferViewType : RHIEnumType 
	{
		RHI_BufferView_Vertex = 0,                    	//0: Vertex
		RHI_BufferView_Index,                           //1: Index
		RHI_BufferView_UniformBinding,					//2: UniformBinding
		RHI_BufferView_StorageBinding,					//3: StorageBinding

		RHI_BufferView_Count,
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
	class RHIFrameBuffer;
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
	class RHISwapChain;
	class RHISyncPrimitivesPool;
	class RHIUtil;
	class RHIVertexInputFormat;

	typedef std::vector<RHIBuffer*> RHIBufferPtrVector;


	
////////////////////////////// Struct //////////////////////////////
	//RHIPhysicalDeviceProperty
	struct RHIPhysicalDeviceProperty
	{
        uint32 nVendorID;
        uint32 nDeviceID;
        RHIPhysicalDeviceType ePhysicalDevice;
    };

	//RHIQueueInfo
	struct RHIQueueInfo 
	{
        RHIQueueType eQueue;
        uint8 nCount;
    };

	//RHIDeviceCreateInfo
	struct RHIDeviceCreateInfo 
	{
        uint32 nQueueCreateInfoCount;
        const RHIQueueInfo* pQueueCreateInfos;
    };

	//RHISurfaceCreateInfo
	struct RHISurfaceCreateInfo 
	{
        void* pWindow;
    };

	//RHISwapChainCreateInfo
	struct RHISwapChainCreateInfo 
	{
        RHIQueue* pQueuePresent;
        RHISurface* pSurface;
        uint8 nTextureCount;
        FPixelFormatType ePixelFormat;
		FSizeI sizeWH;
        RHIPresentType ePresent;
    };

	//RHIVertexBufferViewInfo
	struct RHIVertexBufferViewInfo 
	{
        uint32 nStride;
    };

	//RHIIndexBufferViewInfo
    struct RHIIndexBufferViewInfo 
	{
        RHIIndexType eIndex;
    };

	//RHIBufferViewCreateInfo
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

}; //LostPeterRHI

#endif