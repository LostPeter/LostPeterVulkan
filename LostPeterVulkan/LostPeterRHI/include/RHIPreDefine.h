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
	class RHISwapchain;
	class RHISyncPrimitivesPool;
	class RHIUtil;
	class RHIVertexInputFormat;

	typedef std::vector<RHIBuffer*> RHIBufferPtrVector;

}; //LostPeterRHI

#endif