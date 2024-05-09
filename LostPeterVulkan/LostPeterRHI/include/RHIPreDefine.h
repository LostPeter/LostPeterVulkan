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

	//RHIResourceType
	enum class RHIResourceType : RHIEnumType 
	{
        RHI_Resource_Buffer = 0,						//0: Buffer
        RHI_Resource_Texture,							//1: Texture

		RHI_Resource_Count,
    };

	//RHIBufferState
	enum class RHIBufferStateType : RHIEnumType 
	{
        RHI_BufferState_UnDefined = 0,					//0: UnDefined
        RHI_BufferState_CopySrc,						//1: CopySrc
        RHI_BufferState_CopyDst,						//2: CopyDst
        RHI_BufferState_ShaderReadOnly,					//3: ShaderReadOnly
        RHI_BufferState_Storage,						//4: Storage

        RHI_BufferState_Count,
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

	//RHITextureDimension
	enum class RHITextureDimensionType : RHIEnumType 
	{
        RHI_TextureDimension_1D = 0,					//0: 1D
        RHI_TextureDimension_2D,						//1: 2D
        RHI_TextureDimension_3D,						//2: 3D

        RHI_TextureDimension_Count,
    };

	//RHITextureViewDimensionType
    enum class RHITextureViewDimensionType : RHIEnumType 
	{
        RHI_TextureViewDimension_1D = 0,				//0: 1D
        RHI_TextureViewDimension_2D,					//1: 2D
        RHI_TextureViewDimension_2DArray,				//2: 2DArray
        RHI_TextureViewDimension_Cube,					//3: Cube
        RHI_TextureViewDimension_CubeArray,				//4: CubeArray
        RHI_TextureViewDimension_3D,					//5: 3D

        RHI_TextureViewDimension_Count,
    };

	//RHITextureAspectType
    enum class RHITextureAspectType : RHIEnumType 
	{
        RHI_TextureAspect_Color = 0,					//0: Color
        RHI_TextureAspect_Depth,						//1: Depth
        RHI_TextureAspect_Stencil,						//2: Stencil
        RHI_TextureAspect_DepthStencil,					//3: DepthStencil

        RHI_TextureAspect_Count,
    };

	//RHITextureViewType
    enum class RHITextureViewType : RHIEnumType 
	{
        RHI_TextureView_TextureBinding = 0,				//0: TextureBinding
        RHI_TextureView_StorageBinding,					//1: StorageBinding
        RHI_TextureView_ColorAttachment,				//2: ColorAttachment
        RHI_TextureView_DepthStencil,					//3: DepthStencil

        RHI_TextureView_Count,
    };

	//RHITextureState
	enum class RHITextureStateType : RHIEnumType 
	{
        RHI_TextureState_UnDefined = 0,					//0: UnDefined
        RHI_TextureState_CopySrc,						//1: CopySrc
        RHI_TextureState_CopyDst,						//2: CopyDst
        RHI_TextureState_ShaderReadOnly,				//3: ShaderReadOnly
        RHI_TextureState_RenderTarget,					//4: RenderTarget
        RHI_TextureState_Storage,						//5: Storage
        RHI_TextureState_DepthStencilReadonly,			//6: DepthStencilReadonly
        RHI_TextureState_DepthStencilWrite,				//7: DepthStencilWrite
        RHI_TextureState_Present,						//8: Present

        RHI_TextureState_Count,
    };

	//RHILoadOpType
	enum class RHILoadOpType : RHIEnumType 
	{
        RHI_LoadOp_Load = 0,							//0: Load
        RHI_LoadOp_Clear,								//1: Clear

        RHI_LoadOp_Count,
    };

	//RHIStoreOpType
    enum class RHIStoreOpType : RHIEnumType 
	{
        RHI_StoreOp_Store = 0,							//0: Store
        RHI_StoreOp_Discard,							//1: Discard

        RHI_StoreOp_Count,
    };

	//RHIFlags
	template <typename T = uint32>
    class RHIFlags 
	{
    public:
        using UnderlyingType = T;

	public:
        RHIFlags() 
		{

		}
		RHIFlags(T inValue) 
			: value(inValue) 
		{

		}
        ~RHIFlags()
		{

		}
        
	public:
        template <typename E>
        RHIFlags(E e) 
			: value(static_cast<T>(e)) 
		{

		} 

        T Value() const
        {
            return value;
        }

        explicit operator bool()
        {
            return value;
        }

        bool operator ==(RHIFlags other) const
        {
            return value == other.value;
        }

        bool operator !=(RHIFlags other) const
        {
            return value != other.value;
        }

        bool operator ==(T inValue) const
        {
            return value == inValue;
        }

        bool operator !=(T inValue) const
        {
            return value != inValue;
        }

        template <typename E>
        bool operator==(E e) const
        {
            return value == static_cast<T>(e);
        }

        template <typename E>
        bool operator!=(E e) const
        {
            return value != static_cast<T>(e);
        }

    private:
        T value;
    };

	template <typename T>
    RHIFlags<T> operator&(RHIFlags<T> a, RHIFlags<T> b)
    {
        return RHIFlags<T>(a.Value() & b.Value());
    }

    template <typename T>
    RHIFlags<T> operator|(RHIFlags<T> a, RHIFlags<T> b)
    {
        return RHIFlags<T>(a.Value() | b.Value());
    }

	#define RHI_FLAGS_DECLARE(FlagsType, BitsType) \
		FlagsType operator &(BitsType a, BitsType b); \
		FlagsType operator &(FlagsType a, BitsType b); \
		FlagsType operator |(BitsType a, BitsType b); \
		FlagsType operator |(FlagsType a, BitsType b); \

	
	//RHIBufferUsageBitsType
	using RHIBufferUsageFlags = RHIFlags<>;
    enum class RHIBufferUsageBitsType : RHIBufferUsageFlags::UnderlyingType 
	{
        RHI_BufferUsageBits_MapRead 		= 0x0001,	//0: MapRead
        RHI_BufferUsageBits_MapWrite     	= 0x0002,	//1: MapWrite
        RHI_BufferUsageBits_CopySrc      	= 0x0004,	//2: CopySrc
        RHI_BufferUsageBits_CopyDst      	= 0x0008,	//3: CopyDst
        RHI_BufferUsageBits_Index        	= 0x0010,	//4: Index
        RHI_BufferUsageBits_Vertex       	= 0x0020,	//5: Vertex
        RHI_BufferUsageBits_Uniform      	= 0x0040,	//6: Uniform
        RHI_BufferUsageBits_Storage      	= 0x0080,	//7: Storage
        RHI_BufferUsageBits_Indirect     	= 0x0100,	//8: Indirect
        RHI_BufferUsageBits_QueryResolve 	= 0x0200,	//9: QueryResolve

        RHI_BufferUsageBits_Count,
    };
    RHI_FLAGS_DECLARE(RHIBufferUsageFlags, RHIBufferUsageBitsType)

	//RHITextureUsageBitsType
    using RHITextureUsageFlags = RHIFlags<>;
    enum class RHITextureUsageBitsType : RHITextureUsageFlags::UnderlyingType 
	{
        RHI_TextureUsageBits_CopySrc                 = 0x0001,	//0: CopySrc
        RHI_TextureUsageBits_CopyDst                 = 0x0002,	//1: CopyDst
        RHI_TextureUsageBits_TextureBinding          = 0x0004,	//2: TextureBinding
        RHI_TextureUsageBits_StorageBinding          = 0x0008,  //3: StorageBinding
        RHI_TextureUsageBits_RenderAttachment        = 0x0010,	//4: RenderAttachment
        RHI_TextureUsageBits_DepthStencilAttachment  = 0x0020,	//5: DepthStencilAttachment

        RHI_TextureUsageBits_Count,
    };
    RHI_FLAGS_DECLARE(RHITextureUsageFlags, RHITextureUsageBitsType)

	//RHIShaderStageBitsType
    using RHIShaderStageFlags = RHIFlags<>;
    enum class RHIShaderStageBitsType : RHIShaderStageFlags::UnderlyingType 
	{
        RHI_ShaderStageBits_Vertex   = 0x0001,		//0: Vertex
        RHI_ShaderStageBits_Pixel    = 0x0002,		//1: Pixel 
        RHI_ShaderStageBits_Compute  = 0x0004,		//2: Compute
        RHI_ShaderStageBits_Geometry = 0x0008,		//3: Geometry
        RHI_ShaderStageBits_Domain   = 0x0010,		//4: Domain
        RHI_ShaderStageBits_Hull     = 0x0020,		//5: Hull

        RHI_ShaderStageBits_Count,
    };
    RHI_FLAGS_DECLARE(RHIShaderStageFlags, RHIShaderStageBitsType)

	//RHIColorWriteBitsType
    using RHIColorWriteFlags = RHIFlags<>;
    enum class RHIColorWriteBitsType : RHIColorWriteFlags::UnderlyingType 
	{
        RHI_ColorWriteBits_Red   = 0x1,		//0: Red
        RHI_ColorWriteBits_Green = 0x2,		//1: Green
        RHI_ColorWriteBits_Blue  = 0x4,		//2: Blue
        RHI_ColorWriteBits_Alpha = 0x8,		//3: Alpha

        RHI_ColorWriteBits_Count,
    };
    RHI_FLAGS_DECLARE(RHIColorWriteFlags, RHIColorWriteBitsType)


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
	class RHIInstance;
	class RHIPhysicalDevice;
	class RHIPipeline;
	class RHIPipelineBarriers;
	class RHIPipelineCache;
	class RHIQueue;
	class RHIRenderPassCache;
	class RHIResourceBindings;
	class RHISampler;
	class RHIShaderModule;
	class RHIStreamDecoder;
	class RHIStreamEncoder;
	class RHISurface;
	class RHISwapChain;
	class RHISyncPrimitivesPool;
	class RHITexture;
	class RHITextureView;
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

	//RHIBufferCreateInfo
	struct RHIBufferCreateInfo 
	{
        uint32 nSize;
        RHIBufferUsageFlags eUsages;
       	String strDebugName;
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

	//RHITextureCreateInfo
	struct RHITextureCreateInfo 
	{
		uint32 nWidth;
		uint32 nHeight;
		uint32 nDepth;
        uint8 nMipLevels;
        uint8 nSamples;
        RHITextureDimensionType eTextureDimension;
        FPixelFormatType ePixelFormat;
        RHITextureUsageFlags eUsages;
        RHITextureStateType eState;
      	String strDebugName;
    };

	//RHITextureViewCreateInfo
	struct RHITextureViewCreateInfo 
	{
        RHITextureViewType eTextureView;
        RHITextureViewDimensionType eTextureViewDimension;
        RHITextureAspectType eTextureAspect;
        uint8 nBaseMipLevel;
        uint8 nMipLevelCount;
        uint8 nBaseArrayLayer;
        uint8 nArrayLayerCount;
    };

	//RHISamplerCreateInfo
	struct RHISamplerCreateInfo 
	{
        FTextureAddressingType eAddressModeU;
        FTextureAddressingType eAddressModeV;
        FTextureAddressingType eAddressModeW;
        FTextureFilterPixelType eMagFilter;
        FTextureFilterPixelType eMinFilter;
        FTextureFilterPixelType eMipFilter;
        float fLodMinClamp;
        float fLodMaxClamp;
        FCompareFuncType eComparisonFunc;
        uint8 nMaxAnisotropy;
        String strDebugName;
    };

}; //LostPeterRHI

#endif