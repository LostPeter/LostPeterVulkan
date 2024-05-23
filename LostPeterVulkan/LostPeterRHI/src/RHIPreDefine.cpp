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

#include "../include/RHIPreDefine.h"

namespace LostPeterRHI
{
    ////////////////////////////// Define //////////////////////////////
    bool RHI_IsDebug()
    {
        bool isDebug = false;
    #if F_DEBUG == 1
        isDebug = true;
    #endif
        return isDebug;
    }


    ////////////////////////////// Enum ////////////////////////////////
    //RHIType
    static const String s_nameRHITypes[] = 
    {
        "RHI-Dummy",                    //0: RHI-Dummy
        "RHI-Vulkan",                   //1: RHI-Vulkan
        "RHI-DirectX12",                //2: RHI-DirectX12
        "RHI-Metal",                    //3: RHI-Metal
    };
    const String& RHI_GetRHITypeName(RHIType type)
    {
        return s_nameRHITypes[(int)type];
    }
    const String& RHI_GetRHITypeName(int type)
    {
        return s_nameRHITypes[(int)type];
    }
    RHIType RHI_ParseRHIType(const String& strName)
    {
        for (int i = 0; i < (int)RHIType::RHI_Count; i++)
        {
            if (s_nameRHITypes[i] == strName)
                return (RHIType)(i);
        }
        F_Assert(false && "RHI_ParseRHIType: Wrong type name !")
        return RHIType::RHI_Vulkan;
    }


    //RHIPixelFormatType
    static const String s_namePixelFormatTypes[] = 
    {
        //8-Bits
        "R8Unorm",                      //0:    R8Unorm
        "R8SNorm",                      //1:    R8SNorm
        "R8UInt",                       //2:    R8UInt
        "R8SInt",                       //3:    R8SInt

        //16-Bits
        "R16UInt",                      //4:    R16UInt
        "R16SInt",                      //5:    R16SInt
        "R16Float",                     //6:    R16Float
        "RG8UNorm",                     //7:    RG8UNorm
        "RG8SNorm",                     //8:    RG8SNorm
        "RG8UInt",                      //9:    RG8UInt
        "RG8SInt",                      //10:   RG8SInt

        //32-Bits
        "R32UInt",                      //11:   R32UInt
        "R32UInt",                      //12:   R32UInt
        "R32Float",                     //13:   R32Float
        "RG16UInt",                     //14:   RG16UInt
        "RG16SInt",                     //15:   RG16SInt
        "RG16Float",                    //16:   RG16Float
        "RGBA8UNorm",                   //17:   RGBA8UNorm
        "RGBA8UNormSRGB",               //18:   RGBA8UNormSRGB
        "RGBA8SNorm",                   //19:   RGBA8SNorm
        "RGBA8UInt",                    //20:   RGBA8UInt
        "RGBA8SInt",                    //21:   RGBA8SInt
        "BGRA8UNorm",                   //22:   BGRA8UNorm
        "BGRA8UNormSRGB",               //23:   BGRA8UNormSRGB
        "RGB9E5Float",                  //24:   RGB9E5Float
        "RGB10A2UNorm",                 //25:   RGB10A2UNorm
        "RG11B10Float",                 //26:   RG11B10Float

        //64-Bits
        "RG32UInt",                     //27:   RG32UInt
        "RG32SInt",                     //28:   RG32SInt
        "RG32Float",                    //29:   RG32Float
        "RGBA16UInt",                   //30:   RGBA16UInt
        "RGBA16SInt",                   //31:   RGBA16SInt
        "RGBA16Float",                  //32:   RGBA16Float

        //128-Bits
        "RGBA32UInt",                   //33:   RGBA32UInt
        "RGBA32SInt",                   //34:   RGBA32SInt
        "RGBA32Float",                  //35:   RGBA32Float

        //Depth-Stencil
        "D16UNorm",                     //36:   D16UNorm
        "D24UNormS8UInt",               //37:   D24UNormS8UInt
        "D32Float",                     //38:   D32Float
        "D32FloatS8UInt",               //39:   D32FloatS8UInt

        //Features /BC/ETC/ASTC

        "Unknown",                      //40:   Unknown
    };
    const String& RHI_GetPixelFormatTypeName(RHIPixelFormatType type)
    {
        return s_namePixelFormatTypes[(int)type];
    }
    const String& RHI_GetPixelFormatTypeName(int type)
    {
        return s_namePixelFormatTypes[(int)type];
    }
    RHIPixelFormatType RHI_ParsePixelFormatType(const String& strName)
    {
        for (int i = 0; i < (int)RHIPixelFormatType::RHI_PixelFormat_Count; i++)
        {
            if (s_namePixelFormatTypes[i] == strName)
                return (RHIPixelFormatType)(i);
        }
        F_Assert(false && "RHI_ParsePixelFormatType: Wrong type name !")
        return RHIPixelFormatType::RHI_PixelFormat_RGBA8SNorm;
    }


    //RHIObjectType
    static String s_nameObjectTypes[] = 
    {
        "UnKnown",                      //0:  UnKnown
        "Surface",                      //1:  Surface
        "SwapChain",                    //2:  SwapChain
        "Buffer",                       //3:  Buffer
        "Texture",                      //4:  Texture
        "Sampler",                      //5:  Sampler
        "BindGroupLayoutCache",         //6:  BindGroupLayoutCache
        "BindGroupLayout",              //7:  BindGroupLayout
        "BindGroupPool",                //8:  BindGroupPool
        "BindGroupCache",               //9:  BindGroupCache
        "BindGroup",                    //10: BindGroup
        "ShaderModuleCache",            //11: ShaderModuleCache
        "ShaderModule",                 //12: ShaderModule
        "PipelineLayoutCache",          //13: PipelineLayoutCache
        "PipelineLayout",               //14: PipelineLayout
        "PipelineCache",                //15: PipelineCache
        "PipelineCompute",              //16: PipelineCompute
        "PipelineGraphics",             //17: PipelineGraphics
        "RenderPassCache",              //18: RenderPassCache
        "RenderPass",                   //19: RenderPass
        "FrameBuffer",                  //20: FrameBuffer
        "Fence",                        //21: Fence
        "Semaphore",                    //22: Semaphore
        "CommandPool",                  //23: CommandPool
        "CommandBuffer",                //24: CommandBuffer
        "Queue",                        //25: Queue
    };
    const String& RHI_GetObjectTypeName(RHIObjectType type)
    {
        return s_nameObjectTypes[(int)type];
    }
    const String& RHI_GetObjectTypeName(int type)
    {
        return s_nameObjectTypes[(int)type];
    }
    RHIObjectType RHI_ParseObjectType(const String& strName)
    {
        for (int i = 0; i < (int)RHIObjectType::RHI_Object_Count; i++)
        {
            if (s_nameObjectTypes[i] == strName)
                return (RHIObjectType)(i);
        }
        F_Assert(false && "RHI_ParseObjectType: Wrong type name !")
        return RHIObjectType::RHI_Object_UnKnown;
    }


    //RHIShaderStageBitsType
    static String s_nameShaderStageBitsTypes[] = 
    {
        "Vertex",                   //0: Vertex
        "Pixel",                    //1: Pixel
        "Compute",                  //2: Compute
        "Geometry",                 //3: Geometry
        "Domain",                   //4: Domain
        "Hull",                     //5: Hull
    };
    const String& RHI_GetShaderStageBitsTypeName(RHIShaderStageBitsType type)
    {
        switch (type)
        {
        case RHIShaderStageBitsType::RHI_ShaderStageBits_Vertex:    return s_nameShaderStageBitsTypes[0];
        case RHIShaderStageBitsType::RHI_ShaderStageBits_Pixel:     return s_nameShaderStageBitsTypes[1];
        case RHIShaderStageBitsType::RHI_ShaderStageBits_Compute:   return s_nameShaderStageBitsTypes[2];
        case RHIShaderStageBitsType::RHI_ShaderStageBits_Geometry:  return s_nameShaderStageBitsTypes[3];
        case RHIShaderStageBitsType::RHI_ShaderStageBits_Domain:    return s_nameShaderStageBitsTypes[4];
        case RHIShaderStageBitsType::RHI_ShaderStageBits_Hull:      return s_nameShaderStageBitsTypes[5];
        default:
            F_Assert(false && "RHI_GetShaderStageBitsTypeName: Wrong type !")
        }
        return s_nameShaderStageBitsTypes[0];
    }
    const String& RHI_GetShaderStageBitsTypeName(uint32 type)
    {
        return RHI_GetShaderStageBitsTypeName((RHIShaderStageBitsType)type);
    }
    RHIShaderStageBitsType RHI_ParseShaderStageBitsType(const String& strName)
    {
        if (s_nameShaderStageBitsTypes[0] == strName)
        {
            return RHIShaderStageBitsType::RHI_ShaderStageBits_Vertex;
        }
        else if (s_nameShaderStageBitsTypes[1] == strName)
        {
            return RHIShaderStageBitsType::RHI_ShaderStageBits_Pixel;
        }
        else if (s_nameShaderStageBitsTypes[2] == strName)
        {
            return RHIShaderStageBitsType::RHI_ShaderStageBits_Compute;
        }
        else if (s_nameShaderStageBitsTypes[3] == strName)
        {
            return RHIShaderStageBitsType::RHI_ShaderStageBits_Geometry;
        }
        else if (s_nameShaderStageBitsTypes[4] == strName)
        {
            return RHIShaderStageBitsType::RHI_ShaderStageBits_Domain;
        }
        else if (s_nameShaderStageBitsTypes[5] == strName)
        {
            return RHIShaderStageBitsType::RHI_ShaderStageBits_Hull;
        }
        F_Assert(false && "RHI_ParseShaderStageBitsType: Wrong type name !")
        return RHIShaderStageBitsType::RHI_ShaderStageBits_Vertex;
    }


    #define RHI_FLAGS_IMPL(FlagsType, BitsType) \
        FlagsType operator &(BitsType a, BitsType b) { return FlagsType(static_cast<uint32>(a) & static_cast<uint32>(b)); } \
        FlagsType operator &(FlagsType a, BitsType b) { return FlagsType(a.Value() & static_cast<uint32>(b)); } \
        FlagsType operator |(BitsType a, BitsType b) { return FlagsType(static_cast<uint32>(a) | static_cast<uint32>(b)); } \
        FlagsType operator |(FlagsType a, BitsType b) { return FlagsType(a.Value() | static_cast<uint32>(b)); } \


    RHI_FLAGS_IMPL(RHIBufferUsageFlags, RHIBufferUsageBitsType)
    RHI_FLAGS_IMPL(RHITextureUsageFlags, RHITextureUsageBitsType)
    RHI_FLAGS_IMPL(RHIShaderStageFlags, RHIShaderStageBitsType)
    RHI_FLAGS_IMPL(RHIColorWriteFlags, RHIColorWriteBitsType)

    

}; //LostPeterRHI