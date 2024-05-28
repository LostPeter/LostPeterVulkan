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
    rhiExport bool RHI_IsDebug();


    ////////////////////////////// Typedef /////////////////////////////
    typedef uint32 RHIEnumType;

    class rhiExport RHIFlags 
	{
	public:
        RHIFlags() 
            : m_nValue(0)
		{

		}
		RHIFlags(uint32 inValue) 
			: m_nValue(inValue) 
		{

		}
        ~RHIFlags()
		{

		}
        
	public:
        uint32 Value() const
        {
            return m_nValue;
        }

        explicit operator bool()
        {
            return m_nValue != 0;
        }

        bool operator ==(RHIFlags other) const
        {
            return m_nValue == other.m_nValue;
        }

        bool operator !=(RHIFlags other) const
        {
            return m_nValue != other.m_nValue;
        }

        bool operator ==(uint32 inValue) const
        {
            return m_nValue == inValue;
        }

        bool operator !=(uint32 inValue) const
        {
            return m_nValue != inValue;
        }

        RHIFlags operator &(RHIFlags a)
        {
            return RHIFlags(a.Value() & Value());
        }
        RHIFlags operator &(uint32 a)
        {
            return RHIFlags(a & Value());
        }

        RHIFlags operator |(RHIFlags a)
        {
            return RHIFlags(a.Value() | Value());
        }
        RHIFlags operator |(uint32 a)
        {
            return RHIFlags(a | Value());
        }

    private:
        uint32 m_nValue;
    };


    ////////////////////////////// Enum ////////////////////////////////

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
        RHI_PhysicalDevice_UnKnown = 0,                 //0: UnKnown
        RHI_PhysicalDevice_GPUIntegrated,				//1: GPUIntegrated
        RHI_PhysicalDevice_GPUDiscrete,					//2: GPUDiscrete
        RHI_PhysicalDevice_GPUVirtual,					//3: GPUVirtual
        RHI_PhysicalDevice_CPU,                         //4: CPU

        RHI_PhysicalDevice_Count,
    };

	//RHIQueueType
	enum class RHIQueueType : RHIEnumType 
	{
        RHI_Queue_Graphics = 0,							//0: Graphics
        RHI_Queue_Compute,								//1: Compute	
        RHI_Queue_Transfer,								//2: Transfer
        RHI_Queue_Present,								//3: Present
        
		RHI_Queue_Count,
    };

    //RHIMapType
    enum class RHIMapType : RHIEnumType
    {
        RHI_Map_Read = 0,                               //0: Read
        RHI_Map_Write,                                  //1: Write

        RHI_Map_Count,
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

    //RHISampleCountType
    enum RHISampleCountType : RHIEnumType 
    {
        RHI_SampleCount_1_Bit = 0,                      //0: 1_Bit
        RHI_SampleCount_2_Bit,                          //1: 2_Bit
        RHI_SampleCount_4_Bit,                          //2: 4_Bit
        RHI_SampleCount_8_Bit,                          //3: 8_Bit
        RHI_SampleCount_16_Bit,                         //4: 16_Bit
        RHI_SampleCount_32_Bit,                         //5: 32_Bit
        RHI_SampleCount_64_Bit,                         //6: 64_Bit

        RHI_SampleCount_Count,
    };

    //RHIFenceStatusType
    enum class RHIFenceStatusType : RHIEnumType 
    {
        RHI_FenceStatus_Signaled = 0,                   //0: Signaled
        RHI_FenceStatus_NotReady,                       //1: NotReady

        RHI_FenceStatus_Count,
    };

    //RHISwapStatusType
    enum class RHISwapStatusType : RHIEnumType 
    {
        RHI_SwapStatus_Normal = 0,                      //0: Normal
        RHI_SwapStatus_OutOfDate,                       //1: OutOfDate
        RHI_SwapStatus_Lost,                            //2: Lost
        RHI_SwapStatus_Error,                           //3: Error

        RHI_SwapStatus_Count,  
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

	//RHIAddressType
	enum class RHIAddressType : RHIEnumType 
	{
        RHI_Address_ClampToEdge = 0,					//0: ClampToEdge
        RHI_Address_Repeat,								//1: Repeat
        RHI_Address_MirrorRepeat,						//2: MirrorRepeat
        RHI_Address_Border,                             //3: Border

        RHI_Address_Count,
    };

    //RHIBorderColorType
    enum class RHIBorderColorType : RHIEnumType
    {
        RHI_BorderColor_OpaqueBlack = 0,                //0: OpaqueBlack
        RHI_BorderColor_OpaqueWhite,                    //1: OpaqueWhite
        RHI_BorderColor_TransparentBlack,               //2: TransparentBlack

        RHI_BorderColor_Count,
    };

	//RHIFilterType
    enum class RHIFilterType : RHIEnumType 
	{
        RHI_Filter_Nearest = 0,							//0: Nearest
        RHI_Filter_Linear,								//1: Linear

        RHI_Filter_Count,
    };

	//RHIComparisonFuncType
    enum class RHIComparisonFuncType : RHIEnumType 
	{
        RHI_ComparisonFunc_Never = 0,					//0: Never
        RHI_ComparisonFunc_Less,						//1: Less
        RHI_ComparisonFunc_Equal,						//2: Equal
        RHI_ComparisonFunc_LessEqual,					//3: LessEqual
        RHI_ComparisonFunc_Greater,						//4: Greater
        RHI_ComparisonFunc_NotEqual,					//5: NotEqual
        RHI_ComparisonFunc_GreaterEqual,				//6: GreaterEqual
        RHI_ComparisonFunc_Always,						//7: Always

        RHI_ComparisonFunc_Count,
    };

	//RHIHLSLBindingRangeType
    enum class RHIHLSLBindingRangeType : RHIEnumType 
	{
        RHI_HLSLBindingRange_ConstantBuffer = 0,		//0: ConstantBuffer
        RHI_HLSLBindingRange_Texture,					//1: Texture
        RHI_HLSLBindingRange_Sampler,					//2: Sampler
        RHI_HLSLBindingRange_UnorderedAccess,			//3: UnorderedAccess

        RHI_HLSLBindingRange_Count,
    };

	//RHIBindingType
    enum class RHIBindingType : RHIEnumType 
	{
        RHI_Binding_UniformBuffer = 0,					//0: UniformBuffer
        RHI_Binding_StorageBuffer,						//1: StorageBuffer
        RHI_Binding_Sampler,							//2: Sampler
        RHI_Binding_CombinedImageSampler,               //3: CombinedImageSampler
        RHI_Binding_Texture,							//4: Texture
        RHI_Binding_StorageTexture,						//5: StorageTexture

        RHI_Binding_Count,
    };

	//RHISamplerBindingType
    enum class RHISamplerBindingType : RHIEnumType 
	{
        RHI_SamplerBinding_Filtering = 0,				//0: Filtering
        RHI_SamplerBinding_NonFiltering,				//1: NonFiltering
        RHI_SamplerBinding_Comparison,					//2: Comparison

        RHI_SamplerBinding_Count,
    };

	//RHITextureSampleType
    enum class RHITextureSampleType : RHIEnumType 
	{
        RHI_TextureSample_FilterableFloat = 0,			//0: FilterableFloat
        RHI_TextureSample_NonFilterableFloat,			//1: NonFilterableFloat
        RHI_TextureSample_Depth,						//2: Depth
        RHI_TextureSample_SInt,							//3: SInt
        RHI_TextureSample_UInt,							//4: UInt

        RHI_TextureSample_Count,
    };

	//RHIStorageTextureAccessType
    enum class RHIStorageTextureAccessType : RHIEnumType 
	{
        RHI_StorageTextureAccess_WriteOnly = 0,			//0: WriteOnly

        RHI_StorageTextureAccess_Count,
    };

	//RHIVertexStepType
	enum class RHIVertexStepType : RHIEnumType 
	{
        RHI_VertexStep_PerVertex = 0,					//0: PerVertex
        RHI_VertexStep_PerInstance,						//1: PerInstance

        RHI_VertexStep_Count,
    };

	//RHIPrimitiveTopologySortType
    enum class RHIPrimitiveTopologySortType : RHIEnumType 
	{
        RHI_PrimitiveTopologySort_Point = 0,			//0: Point
        RHI_PrimitiveTopologySort_Line,					//1: Line
        RHI_PrimitiveTopologySort_Triangle,				//2: Triangle

        RHI_PrimitiveTopologySort_Count,
    };

	//RHIPrimitiveTopologyType
    enum class RHIPrimitiveTopologyType : RHIEnumType 
	{
        RHI_PrimitiveTopology_PointList = 0,			//0:  PointList
        RHI_PrimitiveTopology_LineList,					//1:  LineList
        RHI_PrimitiveTopology_LineStrip,				//2:  LineStrip
        RHI_PrimitiveTopology_TriangleList,				//3:  TriangleList
        RHI_PrimitiveTopology_TriangleStrip,			//4:  TriangleStrip
        RHI_PrimitiveTopology_TriangleFan,			    //5:  TriangleFan
        RHI_PrimitiveTopology_LineListAdj,				//6:  LineListAdj
        RHI_PrimitiveTopology_LineStripAdj,				//7:  LineStripAdj
        RHI_PrimitiveTopology_TriangleListAdj,			//8:  TriangleListAdj
        RHI_PrimitiveTopology_TriangleStripAdj,			//9:  TriangleStripAdj
        RHI_PrimitiveTopology_PatchList,			    //10: PatchList

        RHI_PrimitiveTopology_Count,
    };

	//RHIIndexFormatType
	enum class RHIIndexFormatType : RHIEnumType
	{
		RHI_IndexFormat_16Bit = 0,                      //0: 16Bit
		RHI_IndexFormat_32Bit,                          //1: 32Bit

		RHI_IndexFormat_Count,
	};

	//RHIFrontFaceType
    enum class RHIFrontFaceType : RHIEnumType 
	{
        RHI_FrontFace_ClockWise = 0,				    //0: ClockWise
        RHI_FrontFace_CounterClockWise,					//1: CounterClockWise

        RHI_FrontFace_Count,
    };

	//RHICullType
    enum class RHICullType : RHIEnumType 
	{
        RHI_Cull_None = 0,								//0: None
        RHI_Cull_Front,									//1: Front
        RHI_Cull_Back,									//2: Back

        RHI_Cull_Count,
    };

    //RHICullType
    enum class RHIPolygonType : RHIEnumType 
	{
        RHI_Polygon_Point = 0,							//0: Point
        RHI_Polygon_WireFrame,							//1: WireFrame
        RHI_Polygon_Solid,								//2: Solid

        RHI_Polygon_Count,
    };

	//RHIStencilOpType
    enum class RHIStencilOpType : RHIEnumType 
	{
        RHI_StencilOp_Keep = 0,							//0: Keep
        RHI_StencilOp_Zero,								//1: Zero
        RHI_StencilOp_Replace,							//2: Replace
        RHI_StencilOp_Invert,							//3: Invert
        RHI_StencilOp_IncrementClamp,					//4: IncrementClamp
        RHI_StencilOp_DecrementClamp,					//5: DecrementClamp
        RHI_StencilOp_IncrementWrap,					//6: IncrementWrap
        RHI_StencilOp_DecrementWrap,					//7: DecrementWrap

        RHI_StencilOp_Count,
    };

	//RHIBlendFactorType
    enum class RHIBlendFactorType : RHIEnumType 
	{
        RHI_BlendFactor_Zero = 0,						//0: Zero
        RHI_BlendFactor_One,							//1: One
        RHI_BlendFactor_Src,							//2: Src
        RHI_BlendFactor_OneMinusSrc,					//3: OneMinusSrc
        RHI_BlendFactor_SrcAlpha,						//4: SrcAlpha
        RHI_BlendFactor_OneMinusSrcAlpha,				//5: OneMinusSrcAlpha
        RHI_BlendFactor_Dst,							//6: Dst
        RHI_BlendFactor_OneMinusDst,					//7: OneMinusDst
        RHI_BlendFactor_DstAlpha,						//8: DstAlpha
        RHI_BlendFactor_OneMinusDstAlpha,				//9: OneMinusDstAlpha

        RHI_BlendFactor_Count,
    };

	//RHIBlendOpType
    enum class RHIBlendOpType : RHIEnumType 
	{
        RHI_BlendOp_Add = 0,							//0: Add
        RHI_BlendOp_Substract,							//1: Substract
        RHI_BlendOp_ReverseSubstract,					//2: ReverseSubstract
        RHI_BlendOp_Min,								//3: Min
        RHI_BlendOp_Max,								//4: Max

        RHI_BlendOp_Count,
    };

    //RHIColorSpaceType
    enum class RHIColorSpaceType : RHIEnumType 
	{
        RHI_ColorSpace_Gamma = 0,					    //0: Gamma
        RHI_ColorSpace_Linear,							//1: Linear

        RHI_ColorSpace_Count,
    };

	//RHIPixelFormatType
	enum class RHIPixelFormatType : RHIEnumType 
	{
        //8-Bits
        RHI_PixelFormat_R8UNorm = 0,					//0:  R8Unorm			
        RHI_PixelFormat_R8SNorm,						//1:  R8SNorm
        RHI_PixelFormat_R8UInt,							//2:  R8UInt
        RHI_PixelFormat_R8SInt,							//3:  R8SInt

        //16-Bits
        RHI_PixelFormat_R16UInt,						//4:  R16UInt
        RHI_PixelFormat_R16SInt,						//5:  R16SInt
        RHI_PixelFormat_R16Float,						//6:  R16Float
        RHI_PixelFormat_RG8UNorm,						//7:  RG8UNorm
        RHI_PixelFormat_RG8SNorm,						//8:  RG8SNorm
        RHI_PixelFormat_RG8UInt,						//9:  RG8UInt
        RHI_PixelFormat_RG8SInt,						//10: RG8SInt

        //32-Bits
        RHI_PixelFormat_R32UInt,						//11: R32UInt
        RHI_PixelFormat_R32SInt,						//12: R32SInt
        RHI_PixelFormat_R32Float,						//13: R32Float
        RHI_PixelFormat_RG16UInt,						//14: RG16UInt
        RHI_PixelFormat_RG16SInt,						//15: RG16SInt
        RHI_PixelFormat_RG16Float,						//16: RG16Float
        RHI_PixelFormat_RGBA8UNorm,						//17: RGBA8UNorm
        RHI_PixelFormat_RGBA8UNormSRGB,					//18: RGBA8UNormSRGB
        RHI_PixelFormat_RGBA8SNorm,						//19: RGBA8SNorm
        RHI_PixelFormat_RGBA8UInt,						//20: RGBA8UInt
        RHI_PixelFormat_RGBA8SInt,						//21: RGBA8SInt
        RHI_PixelFormat_BGRA8UNorm,						//22: BGRA8UNorm
        RHI_PixelFormat_BGRA8UNormSRGB,					//23: BGRA8UNormSRGB
        RHI_PixelFormat_RGB9E5Float,					//24: RGB9E5Float
        RHI_PixelFormat_RGB10A2UNorm,					//25: RGB10A2UNorm
        RHI_PixelFormat_RG11B10Float,					//26: RG11B10Float

        //64-Bits
        RHI_PixelFormat_RG32UInt,						//27: RG32UInt
        RHI_PixelFormat_RG32SInt,						//28: RG32SInt
        RHI_PixelFormat_RG32Float,						//29: RG32Float
        RHI_PixelFormat_RGBA16UInt,						//30: RGBA16UInt
        RHI_PixelFormat_RGBA16SInt,						//31: RGBA16SInt
        RHI_PixelFormat_RGBA16Float,					//32: RGBA16Float

        //128-Bits
        RHI_PixelFormat_RGBA32UInt,						//33: RGBA32UInt
        RHI_PixelFormat_RGBA32SInt,						//34: RGBA32SInt
        RHI_PixelFormat_RGBA32Float,					//35: RGBA32Float

        //Depth-Stencil
        RHI_PixelFormat_D16UNorm,						//36: D16UNorm
        RHI_PixelFormat_D24UNormS8UInt,					//37: D24UNormS8UInt
        RHI_PixelFormat_D32Float,						//38: D32Float
        RHI_PixelFormat_D32FloatS8UInt,					//39: D32FloatS8UInt

        //Features /BC/ETC/ASTC

        RHI_PixelFormat_Unknown,                        //40: Unknown
        RHI_PixelFormat_Count, 
    };
    rhiExport const String& RHI_GetPixelFormatTypeName(RHIPixelFormatType type);
    rhiExport const String& RHI_GetPixelFormatTypeName(int type);
    rhiExport RHIPixelFormatType RHI_ParsePixelFormatType(const String& strName);

	//RHIVertexFormatType
    enum class RHIVertexFormatType : RHIEnumType 
	{
        //8-Bits Channel
        RHI_VertexFormat_UInt8X2 = 0,					//0:  UInt8X2
        RHI_VertexFormat_UInt8X4,						//1:  UInt8X4
        RHI_VertexFormat_SInt8X2,						//2:  SInt8X2
        RHI_VertexFormat_SInt8X4,						//3:  SInt8X4
        RHI_VertexFormat_UNorm8X2,						//4:  UNorm8X2
        RHI_VertexFormat_UNorm8X4,						//5:  UNorm8X4
        RHI_VertexFormat_SNorm8X2,						//6:  SNorm8X2
        RHI_VertexFormat_SNorm8X4,						//7:  SNorm8X4

        //16-Bits Channel
        RHI_VertexFormat_UInt16X2,						//8:  UInt16X2
        RHI_VertexFormat_UInt16X4,						//9:  UInt16X4
        RHI_VertexFormat_SInt16X2,						//10: SInt16X2
        RHI_VertexFormat_SInt16X4,						//11: SInt16X4
        RHI_VertexFormat_UNorm16X2,						//12: UNorm16X2
        RHI_VertexFormat_UNorm16X4,						//13: UNorm16X4
        RHI_VertexFormat_SNorm16X2,						//14: SNorm16X2
        RHI_VertexFormat_SNorm16X4,						//15: SNorm16X4
        RHI_VertexFormat_Float16X2,						//16: Float16X2
        RHI_VertexFormat_Float16X4,						//17: Float16X4

        //32-Bits Channel
        RHI_VertexFormat_Float32X1,						//18: Float32X1
        RHI_VertexFormat_Float32X2,						//19: Float32X2
        RHI_VertexFormat_Float32X3,						//20: Float32X3
        RHI_VertexFormat_Float32X4,						//21: Float32X4
        RHI_VertexFormat_UInt32X1,						//22: UInt32X1
        RHI_VertexFormat_UInt32X2,						//23: UInt32X2
        RHI_VertexFormat_UInt32X3,						//24: UInt32X3
        RHI_VertexFormat_UInt32X4,						//25: UInt32X4
        RHI_VertexFormat_SInt32X1,						//26: SInt32X1
        RHI_VertexFormat_SInt32X2,						//27: SInt32X2
        RHI_VertexFormat_SInt32X3,						//28: SInt32X3
        RHI_VertexFormat_SInt32X4,						//29: SInt32X4

        RHI_VertexFormat_Count,
    };

    //RHIObjectType
    enum class RHIObjectType : RHIEnumType 
    {
        RHI_Object_UnKnown = 0,                         //0:  UnKnown
        RHI_Object_Surface,                             //1:  Surface
        RHI_Object_SwapChain,                           //2:  SwapChain
        RHI_Object_Buffer,                              //3:  Buffer
        RHI_Object_Texture,                             //4:  Texture
        RHI_Object_Sampler,                             //5:  Sampler
        RHI_Object_BindGroupLayoutCache,                //6:  BindGroupLayoutCache
        RHI_Object_BindGroupLayout,                     //7:  BindGroupLayout
        RHI_Object_BindGroupPool,                       //8:  BindGroupPool
        RHI_Object_BindGroupCache,                      //9:  BindGroupCache
        RHI_Object_BindGroup,                           //10: BindGroup
        RHI_Object_ShaderModuleCache,                   //11: ShaderModuleCache
        RHI_Object_ShaderModule,                        //12: ShaderModule
        RHI_Object_PipelineLayoutCache,                 //13: PipelineLayoutCache
        RHI_Object_PipelineLayout,                      //14: PipelineLayout
        RHI_Object_PipelineCache,                       //15: PipelineCache
        RHI_Object_PipelineCompute,                     //16: PipelineCompute
        RHI_Object_PipelineGraphics,                    //17: PipelineGraphics 
        RHI_Object_RenderPassCache,                     //18: RenderPassCache
        RHI_Object_RenderPass,                          //19: RenderPass
        RHI_Object_FrameBuffer,                         //20: FrameBuffer
        RHI_Object_Fence,                               //21: Fence
        RHI_Object_Semaphore,                           //22: Semaphore
        RHI_Object_CommandPool,                         //23: CommandPool
        RHI_Object_CommandBuffer,                       //24: CommandBuffer
        RHI_Object_Queue,                               //25: Queue

        RHI_Object_Count,
    };
    rhiExport const String& RHI_GetObjectTypeName(RHIObjectType type);
    rhiExport const String& RHI_GetObjectTypeName(int type);
    rhiExport RHIObjectType RHI_ParseObjectType(const String& strName);


	#define RHI_FLAGS_DECLARE(FlagsType, BitsType) \
		rhiExport FlagsType operator &(BitsType a, BitsType b); \
		rhiExport FlagsType operator &(FlagsType a, BitsType b); \
		rhiExport FlagsType operator |(BitsType a, BitsType b); \
		rhiExport FlagsType operator |(FlagsType a, BitsType b); \


	//RHIBufferUsageBitsType
    using RHIBufferUsageFlags = RHIFlags;
    enum class rhiExport RHIBufferUsageBitsType : RHIEnumType
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
    using RHITextureUsageFlags = RHIFlags;
    enum class rhiExport RHITextureUsageBitsType : RHIEnumType
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
    using RHIShaderStageFlags = RHIFlags;
    enum class rhiExport RHIShaderStageBitsType : RHIEnumType
	{
        RHI_ShaderStageBits_Vertex   = 0x0001,		//0: Vertex
        RHI_ShaderStageBits_Pixel    = 0x0002,		//1: Pixel 
        RHI_ShaderStageBits_Compute  = 0x0004,		//2: Compute
        RHI_ShaderStageBits_Geometry = 0x0008,		//3: Geometry
        RHI_ShaderStageBits_Domain   = 0x0010,		//4: Domain/TessellationControl)
        RHI_ShaderStageBits_Hull     = 0x0020,		//5: Hull/TessellationEvaluation)

        RHI_ShaderStageBits_Count,
    };
    RHI_FLAGS_DECLARE(RHIShaderStageFlags, RHIShaderStageBitsType)
    rhiExport const String& RHI_GetShaderStageBitsTypeName(RHIShaderStageBitsType type);
    rhiExport const String& RHI_GetShaderStageBitsTypeName(uint32 type);
    rhiExport RHIShaderStageBitsType RHI_ParseShaderStageBitsType(const String& strName);

	//RHIColorWriteBitsType
    using RHIColorWriteFlags = RHIFlags;
    enum class rhiExport RHIColorWriteBitsType : RHIEnumType
	{
        RHI_ColorWriteBits_Red   = 0x1,		//0: Red
        RHI_ColorWriteBits_Green = 0x2,		//1: Green
        RHI_ColorWriteBits_Blue  = 0x4,		//2: Blue
        RHI_ColorWriteBits_Alpha = 0x8,		//3: Alpha

        RHI_ColorWriteBits_Count,
    };
    RHI_FLAGS_DECLARE(RHIColorWriteFlags, RHIColorWriteBitsType)


    ////////////////////////////// Class ///////////////////////////////
    class RHIBarrier;
    class RHIBindGroup;
    class RHIBindGroupCache;
    class RHIBindGroupLayout;
    class RHIBindGroupLayoutCache;
    class RHIBindGroupPool;
	class RHIBuffer;
	class RHIBufferView;
	class RHICommandBuffer;
    class RHICommandDecoder;
    class RHICommandEncoder;
    class RHICommandPool;
	class RHIDevice;
	class RHIFence;
	class RHIFrameBuffer;
	class RHIGraphicsState;
	class RHIInstance;
    class RHIObject;
    class RHIObjectManager;
	class RHIPhysicalDevice;
	class RHIPipeline;
    class RHIPipelineCache;
    class RHIPipelineCompute;
    class RHIPipelineGraphics;
    class RHIPipelineLayout;
    class RHIPipelineLayoutCache;
	class RHIQueue;
    class RHIRenderPass;
    class RHIRenderPassCache;
	class RHISampler;
    class RHISemaphore;
	class RHIShaderModule;
    class RHIShaderModuleCache;
	class RHISurface;
	class RHISwapChain;
	class RHITexture;
	class RHITextureView;
	class RHIUtil;

    typedef std::vector<RHIPixelFormatType> RHIPixelFormatTypeVector;
	
    typedef std::vector<RHIBindGroup*> RHIBindGroupPtrVector;
    typedef std::map<uint32, RHIBindGroup*> RHIBindGroupPtrIDMap;
    typedef std::map<String, RHIBindGroup*> RHIBindGroupPtrNameMap;
    
    typedef std::vector<RHIBindGroupLayout*> RHIBindGroupLayoutPtrVector;
    typedef std::map<uint32, RHIBindGroupLayout*> RHIBindGroupLayoutPtrIDMap;
    typedef std::map<String, RHIBindGroupLayout*> RHIBindGroupLayoutPtrNameMap;
    
    typedef std::vector<RHIBindGroupPool*> RHIBindGroupPoolPtrVector;
    typedef std::map<uint32, RHIBindGroupPool*> RHIBindGroupPoolPtrIDMap;
    typedef std::map<String, RHIBindGroupPool*> RHIBindGroupPoolPtrNameMap;

    typedef std::vector<RHIBuffer*> RHIBufferPtrVector;
    typedef std::map<uint32, RHIBuffer*> RHIBufferPtrIDMap;
    typedef std::map<String, RHIBuffer*> RHIBufferPtrNameMap;
    typedef std::vector<RHIBufferView*> RHIBufferViewPtrVector;
    typedef std::map<uint32, RHIBufferView*> RHIBufferViewPtrIDMap;
    typedef std::map<String, RHIBufferView*> RHIBufferViewPtrNameMap;

    typedef std::vector<RHICommandBuffer*> RHICommandBufferPtrVector;
    typedef std::map<uint32, RHICommandBuffer*> RHICommandBufferPtrIDMap;
    typedef std::map<String, RHICommandBuffer*> RHICommandBufferPtrNameMap;

    typedef std::vector<RHICommandPool*> RHICommandPoolPtrVector;
    typedef std::map<uint32, RHICommandPool*> RHICommandPoolPtrIDMap;
    typedef std::map<String, RHICommandPool*> RHICommandPoolPtrNameMap;

    typedef std::vector<RHIFence*> RHIFencePtrVector;
    typedef std::map<uint32, RHIFence*> RHIFencePtrIDMap;
    typedef std::map<String, RHIFence*> RHIFencePtrNameMap;
    
    typedef std::vector<RHIFrameBuffer*> RHIFrameBufferPtrVector;
    typedef std::map<uint32, RHIFrameBuffer*> RHIFrameBufferPtrIDMap;
    typedef std::map<String, RHIFrameBuffer*> RHIFrameBufferPtrNameMap;

    typedef std::vector<RHIObject*> RHIObjectPtrVector;
    typedef std::map<uint32, RHIObject*> RHIObjectPtrIDMap;
    typedef std::map<String, RHIObject*> RHIObjectPtrNameMap;
    typedef std::map<RHIObjectType, RHIObjectPtrVector> RHIObjectPtrType2VectorMap;

    typedef std::vector<RHIPhysicalDevice*> RHIPhysicalDevicePtrVector;
    typedef std::map<uint32, RHIPhysicalDevice*> RHIPhysicalDevicePtrIDMap;
    typedef std::map<String, RHIPhysicalDevice*> RHIPhysicalDevicePtrNameMap;

    typedef std::vector<RHIPipeline*> RHIPipelinePtrVector;
    typedef std::map<uint32, RHIPipeline*> RHIPipelinePtrIDMap;
    typedef std::map<String, RHIPipeline*> RHIPipelinePtrNameMap;

    typedef std::vector<RHIPipelineCompute*> RHIPipelineComputePtrVector;
    typedef std::map<uint32, RHIPipelineCompute*> RHIPipelineComputePtrIDMap;
    typedef std::map<String, RHIPipelineCompute*> RHIPipelineComputePtrNameMap;

    typedef std::vector<RHIPipelineGraphics*> RHIPipelineGraphicsPtrVector;
    typedef std::map<uint32, RHIPipelineGraphics*> RHIPipelineGraphicsPtrIDMap;
    typedef std::map<String, RHIPipelineGraphics*> RHIPipelineGraphicsPtrNameMap;

    typedef std::vector<RHIPipelineLayout*> RHIPipelineLayoutPtrVector;
    typedef std::map<uint32, RHIPipelineLayout*> RHIPipelineLayoutPtrIDMap;
    typedef std::map<String, RHIPipelineLayout*> RHIPipelineLayoutPtrNameMap;

    typedef std::vector<RHIQueue*> RHIQueuePtrVector;
    typedef std::map<uint32, RHIQueue*> RHIQueuePtrIDMap;
    typedef std::map<String, RHIQueue*> RHIQueuePtrNameMap;

    typedef std::vector<RHIRenderPass*> RHIRenderPassPtrVector;
    typedef std::map<uint32, RHIRenderPass*> RHIRenderPassPtrIDMap;
    typedef std::map<String, RHIRenderPass*> RHIRenderPassPtrNameMap;

    typedef std::vector<RHISampler*> RHISamplerPtrVector;
    typedef std::map<uint32, RHISampler*> RHISamplerPtrIDMap;
    typedef std::map<String, RHISampler*> RHISamplerPtrNameMap;

    typedef std::vector<RHISemaphore*> RHISemaphorePtrVector;
    typedef std::map<uint32, RHISemaphore*> RHISemaphorePtrIDMap;
    typedef std::map<String, RHISemaphore*> RHISemaphorePtrNameMap;

    typedef std::vector<RHIShaderModule*> RHIShaderModulePtrVector;
    typedef std::map<uint32, RHIShaderModule*> RHIShaderModulePtrIDMap;
    typedef std::map<String, RHIShaderModule*> RHIShaderModulePtrNameMap;

    typedef std::vector<RHISurface*> RHISurfacePtrVector;
    typedef std::map<uint32, RHISurface*> RHISurfacePtrIDMap;
    typedef std::map<String, RHISurface*> RHISurfacePtrNameMap;

    typedef std::vector<RHISwapChain*> RHISwapChainPtrVector;
    typedef std::map<uint32, RHISwapChain*> RHISwapChainPtrIDMap;
    typedef std::map<String, RHISwapChain*> RHISwapChainPtrNameMap;

    typedef std::vector<RHIShaderModule*> RHIShaderModulePtrVector;
    typedef std::map<uint32, RHIShaderModule*> RHIShaderModulePtrIDMap;
    typedef std::map<String, RHIShaderModule*> RHIShaderModulePtrNameMap;

    typedef std::vector<RHIBarrier*> RHIBarrierPtrVector;
    typedef std::map<uint32, RHIBarrier*> RHIBarrierPtrIDMap;
    typedef std::map<String, RHIBarrier*> RHIBarrierPtrNameMap;

    typedef std::vector<RHITexture*> RHITexturePtrVector;
    typedef std::map<uint32, RHITexture*> RHITexturePtrIDMap;
    typedef std::map<String, RHITexture*> RHITexturePtrNameMap;
    typedef std::vector<RHITextureView*> RHITextureViewPtrVector;
    typedef std::map<uint32, RHITextureView*> RHITextureViewPtrIDMap;
    typedef std::map<String, RHITextureView*> RHITextureViewPtrNameMap;


    ////////////////////////////// Struct //////////////////////////////
    template <uint8 N>
    struct RHIExtent;

    template <>
    struct RHIExtent<1> 
    {
        uint32 x;
    };

    template <>
    struct RHIExtent<2> 
    {
        uint32 x;
        uint32 y;
    };

    template <>
    struct RHIExtent<3> 
    {
        uint32 x;
        uint32 y;
        uint32 z;
    };

    template <uint8 N>
    struct RHIColorNormalized;

    template <>
    struct RHIColorNormalized<1> 
    {
        float r;
    };

    template <>
    struct RHIColorNormalized<2> 
    {
        float r;
        float g;
    };

    template <>
    struct RHIColorNormalized<3> 
    {
        float r;
        float g;
        float b;
    };

    template <>
    struct RHIColorNormalized<4> 
    {
        float r;
        float g;
        float b;
        float a;
    };

    struct rhiExport RHIPixelFormatInfo
    {
        String strName;
        RHIPixelFormatType ePixelFormat;
        bool bIsSupported;

        String strNameOriginal;
        uint32 nTypeOriginal;

        RHIPixelFormatInfo()
            : strName("")
            , ePixelFormat(RHIPixelFormatType::RHI_PixelFormat_Unknown)
            , bIsSupported(false)
            , strNameOriginal("")
            , nTypeOriginal(-1)
        {

        }
    };
    typedef std::vector<RHIPixelFormatInfo*> RHIPixelFormatInfoPtrVector;
    typedef std::map<uint32, RHIPixelFormatInfo*> RHIPixelFormatInfoPtrMap;

	//RHIPhysicalDeviceProperty
	struct rhiExport RHIPhysicalDeviceProperty
	{
        uint32 nVendorID;
        uint32 nDeviceID;
        RHIPhysicalDeviceType ePhysicalDevice;

        RHIPhysicalDeviceProperty()
            : nVendorID(0)
            , nDeviceID(0)
            , ePhysicalDevice(RHIPhysicalDeviceType::RHI_PhysicalDevice_UnKnown)
        {

        }
    };

	//RHIQueueInfo
	struct rhiExport RHIQueueInfo
	{
        RHIQueueType eQueue;
        uint8 nCount;
        String strDebugName;

        RHIQueueInfo()
            : eQueue(RHIQueueType::RHI_Queue_Graphics)
            , nCount(1)
            , strDebugName("")
        {

        }
    };

	//RHIDeviceCreateInfo
	struct rhiExport RHIDeviceCreateInfo
	{
        int32 nPreferredVendorID;
        uint32 nQueueCreateInfoCount;
        const RHIQueueInfo* pQueueCreateInfos;
        String strDebugName;

        RHIDeviceCreateInfo()
            : nPreferredVendorID(-1)
            , nQueueCreateInfoCount(0)
            , pQueueCreateInfos(nullptr)
            , strDebugName("")
        {
            
        }
    };

	//RHISurfaceCreateInfo
	struct rhiExport RHISurfaceCreateInfo
	{
        void* pWindow;
        String strDebugName;

        RHISurfaceCreateInfo()
            : pWindow(nullptr)
            , strDebugName("")
        {

        }
    };

	//RHISwapChainCreateInfo
	struct rhiExport RHISwapChainCreateInfo
	{
        RHIQueue* pQueuePresent;
        RHISurface* pSurface;
        uint8 nTextureCount;
        RHIPixelFormatType ePixelFormat;
        RHIColorSpaceType eColorSpace;
        RHIPresentType ePresent;
        RHIExtent<2> sExtent;
        String strDebugName;

        RHISwapChainCreateInfo()
            : pQueuePresent(nullptr)
            , pSurface(nullptr)
            , nTextureCount(0)
            , ePixelFormat(RHIPixelFormatType::RHI_PixelFormat_BGRA8UNormSRGB)
            , eColorSpace(RHIColorSpaceType::RHI_ColorSpace_Gamma)
            , ePresent(RHIPresentType::RHI_Present_Immediately)
            , strDebugName("")
        {

        }
    };

	//RHIBufferCreateInfo
	struct rhiExport RHIBufferCreateInfo
	{
        uint32 nSize;
        RHIBufferUsageFlags eUsages;
       	String strDebugName;

        RHIBufferCreateInfo()
            : nSize(0)
            , eUsages(0)
            , strDebugName("")
        {

        }
    };

	//RHIVertexBufferViewInfo
	struct rhiExport RHIVertexBufferViewInfo
	{
        uint32 nStride;

        RHIVertexBufferViewInfo()
            : nStride(0)
        {

        }
    };

	//RHIIndexBufferViewInfo
    struct rhiExport RHIIndexBufferViewInfo
	{
        RHIIndexFormatType eIndexFormat;

        RHIIndexBufferViewInfo()
            : eIndexFormat(RHIIndexFormatType::RHI_IndexFormat_16Bit)
        {

        }
    };

	//RHIBufferViewCreateInfo
    struct rhiExport RHIBufferViewCreateInfo
	{
        RHIBufferViewType eBufferView;
        uint32 nOffset;
        uint32 nSize;
        union 
		{
            RHIVertexBufferViewInfo eVertexBufferViewInfo;
            RHIIndexBufferViewInfo eIndexBufferViewInfo;
        };

        RHIBufferViewCreateInfo()
            : eBufferView(RHIBufferViewType::RHI_BufferView_Vertex)
            , nOffset(0)
            , nSize(0)
        {

        }
    };

	//RHITextureCreateInfo
	struct rhiExport RHITextureCreateInfo
	{
        RHIExtent<3> sExtent;
        uint8 nMipLevels;
        RHITextureDimensionType eTextureDimension;
        RHIPixelFormatType ePixelFormat;
        RHISampleCountType eSampleCount;
        RHITextureUsageFlags eUsages;
        RHITextureStateType eState;
      	String strDebugName;

        RHITextureCreateInfo()
            : nMipLevels(0)
            , eTextureDimension(RHITextureDimensionType::RHI_TextureDimension_2D)
            , ePixelFormat(RHIPixelFormatType::RHI_PixelFormat_BGRA8UNormSRGB)
            , eSampleCount(RHISampleCountType::RHI_SampleCount_1_Bit)
            , eUsages(0)
            , eState(RHITextureStateType::RHI_TextureState_UnDefined)
            , strDebugName("")
        {

        }
    };

	//RHITextureViewCreateInfo
	struct rhiExport RHITextureViewCreateInfo
	{
        RHITextureViewType eTextureView;
        RHITextureViewDimensionType eTextureViewDimension;
        RHITextureAspectType eTextureAspect;
        uint8 nBaseMipLevel;
        uint8 nMipLevelCount;
        uint8 nBaseArrayLayer;
        uint8 nArrayLayerCount;

        RHITextureViewCreateInfo()
            : eTextureView(RHITextureViewType::RHI_TextureView_ColorAttachment)
            , eTextureViewDimension(RHITextureViewDimensionType::RHI_TextureViewDimension_2D)
            , eTextureAspect(RHITextureAspectType::RHI_TextureAspect_Color)
            , nBaseMipLevel(0)
            , nMipLevelCount(1)
            , nBaseArrayLayer(0)
            , nArrayLayerCount(1)
        {

        }
    };

	//RHISamplerCreateInfo
	struct rhiExport RHISamplerCreateInfo
	{
        RHIAddressType eAddressU;
        RHIAddressType eAddressV;
        RHIAddressType eAddressW;
        RHIBorderColorType eBorderColor;
        RHIFilterType eFilterMin;
        RHIFilterType eFilterMag;
        RHIFilterType eFilterMip;
        float fLodMinClamp;
        float fLodMaxClamp;
        float fLodMipBias;
        RHIComparisonFuncType eComparisonFunc;
        uint8 nMaxAnisotropy;
        String strDebugName;

        RHISamplerCreateInfo()
            : eAddressU(RHIAddressType::RHI_Address_ClampToEdge)
            , eAddressV(RHIAddressType::RHI_Address_ClampToEdge)
            , eAddressW(RHIAddressType::RHI_Address_ClampToEdge)
            , eBorderColor(RHIBorderColorType::RHI_BorderColor_OpaqueBlack)
            , eFilterMin(RHIFilterType::RHI_Filter_Linear)
            , eFilterMag(RHIFilterType::RHI_Filter_Linear)
            , eFilterMip(RHIFilterType::RHI_Filter_Linear)
            , fLodMinClamp(0.0f)
            , fLodMaxClamp(0.0f)
            , fLodMipBias(0.0f)
            , eComparisonFunc(RHIComparisonFuncType::RHI_ComparisonFunc_Never)
            , nMaxAnisotropy(1)
            , strDebugName("")
        {

        }
    };

    //RHIHLSLBinding
    struct rhiExport RHIHLSLBinding
    {
        RHIHLSLBindingRangeType eHLSLBindingRange;
        uint8 nIndex;
    };

    //RHIGLSLBinding
    struct rhiExport RHIGLSLBinding
    {
        uint8 nIndex;
    };

    //RHIResourceBinding
    struct rhiExport RHIResourceBinding
    {
        RHIBindingType eBinding;
        union 
        {
            RHIHLSLBinding sBindingHLSL;
            RHIGLSLBinding sBindingGLSL;
        } sBinding;
    };

    //RHIBindGroupLayoutEntry
    struct rhiExport RHIBindGroupLayoutEntry
    {
        RHIResourceBinding sBinding;
        RHIShaderStageFlags flagsShaderVisibility;
        RHISampler* pSampler;

        RHIBindGroupLayoutEntry()
            : flagsShaderVisibility(0)
            , pSampler(nullptr)
        {

        }
    };
    typedef std::vector<RHIBindGroupLayoutEntry> RHIBindGroupLayoutEntryVector;

    //RHIBindGroupLayoutCacheCreateInfo
    struct rhiExport RHIBindGroupLayoutCacheCreateInfo
    {
        String strDebugName;

        RHIBindGroupLayoutCacheCreateInfo()
            : strDebugName("")
        {

        }
    };

    //RHIBindGroupLayoutCreateInfo
    struct rhiExport RHIBindGroupLayoutCreateInfo
    {
        uint8 nLayoutIndex;
        RHIBindGroupLayoutEntryVector aEntries;
        String strDebugName;

        RHIBindGroupLayoutCreateInfo()
            : nLayoutIndex(0)
            , strDebugName("")
        {

        }
    };

    //RHIBindGroupEntry
    struct rhiExport RHIBindGroupEntry
    {
        RHIResourceBinding sBinding;
        union 
        {
            RHISampler* pSampler;
            RHITextureView* pTextureView;
            RHIBufferView* pBufferView;
        };

        RHIBindGroupEntry()
        {

        }
    };
    typedef std::vector<RHIBindGroupEntry> RHIBindGroupEntryVector;

    //RHIBindGroupPoolCreateInfo
    struct rhiExport RHIBindGroupPoolCreateInfo
    {
        uint32 nPoolCount;
        String strDebugName;

        RHIBindGroupPoolCreateInfo()
            : nPoolCount(0)
            , strDebugName("")
        {

        }
    };

    //RHIBindGroupCacheCreateInfo
    struct rhiExport RHIBindGroupCacheCreateInfo
    {
        String strDebugName;

        RHIBindGroupCacheCreateInfo()
            : strDebugName("")
        {

        }
    };

    //RHIBindGroupCreateInfo
    struct rhiExport RHIBindGroupCreateInfo
    {
        RHIBindGroupPool* pBindGroupPool;
        RHIBindGroupLayout* pBindGroupLayout;
        RHIBindGroupEntryVector aEntries;
        String strDebugName;

        RHIBindGroupCreateInfo()
            : pBindGroupPool(nullptr)
            , pBindGroupLayout(nullptr)
            , strDebugName("")
        {

        }
    };

    //RHIPipelineConstantLayout
    struct rhiExport RHIPipelineConstantLayout
    {
        RHIShaderStageFlags flagsShaderStage;
        uint32 nOffset;
        uint32 nSize;

        RHIPipelineConstantLayout()
            : flagsShaderStage(0)
            , nOffset(0)
            , nSize(0)
        {

        }
    };
    typedef std::vector<RHIPipelineConstantLayout> RHIPipelineConstantLayoutVector;

    //RHIPipelineLayoutCacheCreateInfo
    struct rhiExport RHIPipelineLayoutCacheCreateInfo
    {
        String strDebugName;

        RHIPipelineLayoutCacheCreateInfo()
            : strDebugName("")
        {

        }
    };

    //RHIPipelineLayoutCreateInfo
    struct rhiExport RHIPipelineLayoutCreateInfo
    {
        RHIBindGroupLayoutPtrVector aBindGroupLayout;
        RHIPipelineConstantLayoutVector aPipelineConstantLayout;
        String strDebugName;

        RHIPipelineLayoutCreateInfo()
            : strDebugName("")
        {

        }
    };

    //RHIShaderModuleCacheCreateInfo
    struct rhiExport RHIShaderModuleCacheCreateInfo
    {
        String strDebugName;

        RHIShaderModuleCacheCreateInfo()
            : strDebugName("")
        {

        }
    };

    //RHIShaderModuleCreateInfo
    struct rhiExport RHIShaderModuleCreateInfo
    {
        RHIShaderStageBitsType eShaderStageBits;
        String strPath;
        String strByteCode;
        String strNameMain;
        String strDebugName;

        RHIShaderModuleCreateInfo()
            : eShaderStageBits(RHIShaderStageBitsType::RHI_ShaderStageBits_Vertex)
            , strPath("")
            , strByteCode("")
            , strNameMain("main")
            , strDebugName("")
        {

        }
    };

    //RHIRenderPassCacheCreateInfo
    struct rhiExport RHIRenderPassCacheCreateInfo
    {
        String strDebugName;

        RHIRenderPassCacheCreateInfo()
            : strDebugName("")
        {

        }
    };

    //RHIRenderPassCreateInfo
    struct rhiExport RHIRenderPassCreateInfo
    {
        String strDebugName;

        RHIRenderPassCreateInfo()
            : strDebugName("")
        {

        }
    };

    //RHIFrameBufferCreateInfo
    struct rhiExport RHIFrameBufferCreateInfo
    {
        String strDebugName;

        RHIFrameBufferCreateInfo()
            : strDebugName("")
        {

        }
    };

    //RHIFenceCreateInfo
    struct rhiExport RHIFenceCreateInfo
    {
        bool bIsSignaled;
        String strDebugName;

        RHIFenceCreateInfo()
            : bIsSignaled(false)
            , strDebugName("")
        {

        }
    };

    //RHISemaphoreCreateInfo
    struct rhiExport RHISemaphoreCreateInfo
    {
        String strDebugName;

        RHISemaphoreCreateInfo()
            : strDebugName("")
        {

        }
    };

    //RHICommandPoolCreateInfo
    struct rhiExport RHICommandPoolCreateInfo
    {
        RHIQueueType eQueue;
        uint32 nFamilyIndex; 
        String strDebugName;

        RHICommandPoolCreateInfo()
            : eQueue(RHIQueueType::RHI_Queue_Graphics)
            , nFamilyIndex(0)
            , strDebugName("")
        {

        }
    };

    //RHICommandBufferCreateInfo
    struct rhiExport RHICommandBufferCreateInfo
    {
        RHIQueueType eQueue;
        String strDebugName;

        RHICommandBufferCreateInfo()
            : eQueue(RHIQueueType::RHI_Queue_Graphics)
            , strDebugName("")
        {

        }
    };

    //RHIQueueCreateInfo
    struct rhiExport RHIQueueCreateInfo
    {
        RHIQueueType eQueue;
        uint32 nFamilyIndex; 
        String strDebugName;

        RHIQueueCreateInfo()
            : eQueue(RHIQueueType::RHI_Queue_Graphics)
            , nFamilyIndex(0)
            , strDebugName("")
        {

        }
    };

    //RHIVertexAttribute
    struct rhiExport RHIVertexAttribute
    {
        RHIVertexFormatType eVertexFormat;
        uint32 nOffset;

        const char* pSemanticName;
        uint8 nSemanticIndex;

        RHIVertexAttribute()
            : eVertexFormat(RHIVertexFormatType::RHI_VertexFormat_Float32X4)
            , nOffset(0)
            , pSemanticName(nullptr)
            , nSemanticIndex(0)
        {

        }
    };

    //RHIVertexBufferLayout
    struct rhiExport RHIVertexBufferLayout
    {
        uint32 nStride;
        RHIVertexStepType eVertexStep;
        uint32 nAttributeCount;
        const RHIVertexAttribute* pAttributes;

        RHIVertexBufferLayout()
            : nStride(0)
            , eVertexStep(RHIVertexStepType::RHI_VertexStep_PerInstance)
            , nAttributeCount(0)
            , pAttributes(nullptr)
        {

        }
    };

    //RHIVertexState
    struct rhiExport RHIVertexState
    {
        uint32 nBufferLayoutCount;
        const RHIVertexBufferLayout* pVertexBufferLayouts;

        RHIVertexState()
            : nBufferLayoutCount(0)
            , pVertexBufferLayouts(nullptr)
        {

        }
    };

    //RHIPrimitiveState
    struct rhiExport RHIPrimitiveState 
    {
        RHIPrimitiveTopologySortType ePrimitiveTopologySort;
        RHIIndexFormatType eIndexFormat;
        RHIFrontFaceType eFrontFace;
        RHICullType eCull;
        bool bDepthClip;

        RHIPrimitiveState()
            : ePrimitiveTopologySort(RHIPrimitiveTopologySortType::RHI_PrimitiveTopologySort_Triangle)
            , eIndexFormat(RHIIndexFormatType::RHI_IndexFormat_16Bit)
            , eFrontFace(RHIFrontFaceType::RHI_FrontFace_Count)
            , eCull(RHICullType::RHI_Cull_Back)
            , bDepthClip(false)
        {

        }
    };

    //RHIStencilFaceState
    struct rhiExport RHIStencilFaceState 
    {
        RHIComparisonFuncType eComparisonFunc;
        RHIStencilOpType eFailOp;
        RHIStencilOpType eDepthFailOp;
        RHIStencilOpType ePassOp;

        RHIStencilFaceState()
            : eComparisonFunc(RHIComparisonFuncType::RHI_ComparisonFunc_Always)
            , eFailOp(RHIStencilOpType::RHI_StencilOp_Keep)
            , eDepthFailOp(RHIStencilOpType::RHI_StencilOp_Keep)
            , ePassOp(RHIStencilOpType::RHI_StencilOp_Keep)
        {

        }
    };

    //RHIDepthStencilState
    struct rhiExport RHIDepthStencilState 
    {
        bool bDepthEnable;
        bool bStencilEnable;
        RHIPixelFormatType ePixelFormat;
        RHIComparisonFuncType eDepthComparisonFunc;
        RHIStencilFaceState sStencilFront;
        RHIStencilFaceState sStencilBack;
        uint8 nStencilReadMask;
        uint8 nStencilWriteMask;
        int32 nDepthBias;
        float fDepthBiasSlopeScale;
        float fDepthBiasClamp;

        RHIDepthStencilState()
            : bDepthEnable(false)
            , bStencilEnable(false)
            , ePixelFormat(RHIPixelFormatType::RHI_PixelFormat_Unknown)
            , eDepthComparisonFunc(RHIComparisonFuncType::RHI_ComparisonFunc_Always)
            , nStencilReadMask(0xFF)
            , nStencilWriteMask(0xFF)
            , nDepthBias(0)
            , fDepthBiasSlopeScale(0.0f)
            , fDepthBiasClamp(0.0f)
        {

        }
    };

    //RHIMultiSampleState
    struct rhiExport RHIMultiSampleState
    {
        uint8 nCount;
        uint32 nMask;
        bool bAlphaToCoverage;

        RHIMultiSampleState()
            : nCount(1)
            , nMask(0xFFFFFFFF)
            , bAlphaToCoverage(false)
        {

        }
    };

    //RHIBlendComponent
    struct rhiExport RHIBlendComponent 
    {
        RHIBlendOpType eBlendOp;
        RHIBlendFactorType eBlendFactorSrc;
        RHIBlendFactorType eBlendFactorDst;

        RHIBlendComponent()
            : eBlendOp(RHIBlendOpType::RHI_BlendOp_Add)
            , eBlendFactorSrc(RHIBlendFactorType::RHI_BlendFactor_One)
            , eBlendFactorDst(RHIBlendFactorType::RHI_BlendFactor_Zero)
        {

        }
    };

    //RHIBlendState
    struct rhiExport RHIBlendState 
    {
        RHIBlendComponent sBlendColor;
        RHIBlendComponent sBlendAlpha;

        RHIBlendState()
        {

        }
    };

    //RHIColorTargetState
    struct rhiExport RHIColorTargetState 
    {
        RHIPixelFormatType ePixelFormat;
        RHIBlendState sBlendState;
        RHIColorWriteFlags flagsWrite;

        RHIColorTargetState()
            : ePixelFormat(RHIPixelFormatType::RHI_PixelFormat_BGRA8UNorm)
            , flagsWrite(RHIColorWriteBitsType::RHI_ColorWriteBits_Red |
                         RHIColorWriteBitsType::RHI_ColorWriteBits_Green | 
                         RHIColorWriteBitsType::RHI_ColorWriteBits_Blue | 
                         RHIColorWriteBitsType::RHI_ColorWriteBits_Alpha)
        {

        }
    };

    //RHIFragmentState
    struct rhiExport RHIFragmentState 
    {
        uint8 nColorTargetCount;
        const RHIColorTargetState* pColorTargets;

        RHIFragmentState()
            : nColorTargetCount(0)
            , pColorTargets(nullptr)
        {

        }
    };

    //RHIPipelineCacheCreateInfo
    struct rhiExport RHIPipelineCacheCreateInfo
    {
        String strDebugName;

        RHIPipelineCacheCreateInfo()
            : strDebugName("")
        {

        }
    };

    //RHIPipelineComputeCreateInfo
    struct rhiExport RHIPipelineComputeCreateInfo
    {
        RHIPipelineCache* pPipelineCache;
        RHIPipelineLayout* pPipelineLayout;
        RHIShaderModule* pComputeShader;

        String strDebugName;

        RHIPipelineComputeCreateInfo()
            : pPipelineCache(nullptr)
            , pPipelineLayout(nullptr)
            , pComputeShader(nullptr)
        {

        }
    };

    //RHIPipelineGraphicsCreateInfo
    struct rhiExport RHIPipelineGraphicsCreateInfo
    {
        RHIPipelineCache* pPipelineCache;
        RHIPipelineLayout* pPipelineLayout;
        RHIShaderModule* pShaderVertex;
        RHIShaderModule* pShaderPixel;
        RHIShaderModule* pShaderGeometry;
        RHIShaderModule* pShaderDomain;
        RHIShaderModule* pShaderHull;

        RHIVertexState sVertexState;
        RHIPrimitiveState sPrimitiveState;
        RHIDepthStencilState sDepthStencilState;
        RHIMultiSampleState sMultiSampleState;
        RHIFragmentState sFragmentState;

        String strDebugName;

        RHIPipelineGraphicsCreateInfo()
            : pPipelineCache(nullptr)
            , pPipelineLayout(nullptr)
            , pShaderVertex(nullptr)
            , pShaderPixel(nullptr)
            , pShaderGeometry(nullptr)
            , pShaderDomain(nullptr)
            , pShaderHull(nullptr)
        {

        }
    };

    //RHITextureSubResourceInfo
    struct rhiExport RHITextureSubResourceInfo 
    {
        uint8 nMipLevel;
        uint8 nBaseArrayLayer;
        uint8 nArrayLayerCount;
        RHIExtent<3> sExtent;
        RHITextureAspectType eTextureAspect;

        RHITextureSubResourceInfo()
            : eTextureAspect(RHITextureAspectType::RHI_TextureAspect_Color)
        {

        }
    };

    //RHIGraphicsPassColorAttachment
    struct rhiExport RHIGraphicsPassColorAttachment 
    {
        RHITextureView* pView;
        RHITextureView* pResolve;
        RHIColorNormalized<4> sClearValue;
        RHILoadOpType eLoadOp;
        RHIStoreOpType eStoreOp;

        RHIGraphicsPassColorAttachment()
            : pView(nullptr)
            , pResolve(nullptr)
            , eLoadOp(RHILoadOpType::RHI_LoadOp_Clear)
            , eStoreOp(RHIStoreOpType::RHI_StoreOp_Discard)
        {

        }
    };

    //RHIGraphicsPassDepthStencilAttachment
    struct rhiExport RHIGraphicsPassDepthStencilAttachment 
    {
        RHITextureView* pView;
        float fDepthClearValue;
        RHILoadOpType eDepthLoadOp;
        RHIStoreOpType eDepthStoreOp;
        bool bDepthReadOnly;
        uint32 nStencilClearValue;
        RHILoadOpType eStencilLoadOp;
        RHIStoreOpType eStencilStoreOp;
        bool bStencilReadOnly;

        RHIGraphicsPassDepthStencilAttachment()
            : pView(nullptr)
            , fDepthClearValue(0.0f)
            , eDepthLoadOp(RHILoadOpType::RHI_LoadOp_Clear)
            , eDepthStoreOp(RHIStoreOpType::RHI_StoreOp_Discard)
            , bDepthReadOnly(true)
            , nStencilClearValue(0)
            , eStencilLoadOp(RHILoadOpType::RHI_LoadOp_Clear)
            , eStencilStoreOp(RHIStoreOpType::RHI_StoreOp_Discard)
            , bStencilReadOnly(true)
        {

        }
    };

    //RHIGraphicsPassBeginInfo
    struct rhiExport RHIGraphicsPassBeginInfo
    {
        uint32 nColorAttachmentCount;
        const RHIGraphicsPassColorAttachment* pColorAttachments;
        const RHIGraphicsPassDepthStencilAttachment* pDepthStencilAttachment;

        RHIGraphicsPassBeginInfo()
            : nColorAttachmentCount(0)
            , pColorAttachments(nullptr)
            , pDepthStencilAttachment(nullptr)
        {

        }
    };

}; //LostPeterRHI

#endif