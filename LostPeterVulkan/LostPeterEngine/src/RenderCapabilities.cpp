/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-14
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RenderCapabilities.h"

template<> LostPeterEngine::RenderCapabilities* LostPeterFoundation::FSingleton<LostPeterEngine::RenderCapabilities>::ms_Singleton = nullptr;

namespace LostPeterEngine
{
    GpuDriverVersion::GpuDriverVersion()
        : m_nMajor(0)
        , m_nMinor(0)
        , m_nRelease(0)
        , m_nBuild(0)
    {

    }

    String GpuDriverVersion::ToString() const
    {
        return FUtilString::FormatString("%d.%d.%d.%d", m_nMajor, m_nMinor, m_nRelease, m_nBuild);
    }

    void GpuDriverVersion::FromString(const String& strVersion)
    {
        StringVector tokens = FUtilString::Split(strVersion,".");
        if (!tokens.empty())
        {
            m_nMajor = FUtilString::ParserInt(tokens[0]);
            if (tokens.size() > 1)
                m_nMinor = FUtilString::ParserInt(tokens[1]);
            if (tokens.size() > 2)
                m_nRelease = FUtilString::ParserInt(tokens[2]);
            if (tokens.size() > 3)
                m_nBuild = FUtilString::ParserInt(tokens[3]);
        }
    }


    RenderCapabilities* RenderCapabilities::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	RenderCapabilities& RenderCapabilities::GetSingleton()
	{  
		F_Assert(ms_Singleton && "RenderCapabilities::GetSingleton")
		return (*ms_Singleton);     
	}


    RenderCapabilities::RenderCapabilities()
		: Base("RenderCapabilities")
        , m_eGpuVendor(E_GpuVendor_Unknown)
        , m_strNameDevice("")
        , m_strNameRenderer("") 
        , m_nStencilBufferBitDepth(0)
        , m_nNumVertexAttributes(0)
        , m_nNumVertexTextureUnits(0)
        , m_nNumTextureUnits(0)
        , m_nNumMultiRenderTargets(0)
		, m_fMaxPointSize(0.0f)
        , m_fMaxSupportedAnisotropy(0.0f)
		, m_strMaxVertexProgramVersion("")
		, m_strMaxFragmentProgramVersion("")
		, m_nVertexProgramConstantFloatCount(0)     
        , m_nGeometryProgramConstantFloatCount(0) 
        , m_nTessellationHullProgramConstantFloatCount(0) 
        , m_nTessellationDomainProgramConstantFloatCount(0) 
        , m_nComputeProgramConstantFloatCount(0)      
		, m_nFragmentProgramConstantFloatCount(0)   

		, m_bNonPOW2TexturesLimited(false)
		, m_bVertexTextureUnitsShared(false)
		, m_bRenderTargetFormatNull(false)
		, m_bDSTFormatD16(false)
		, m_bDSTFormatD24S8(false)
		, m_bDSTFormatD24X8(false)
	{
        for (int i = 0; i < E_RenderCapabilityCategory_Count; i++)
        {
            m_nCapabilities[i] = 0;
        }
        m_bCapabilities[E_RenderCapabilityCategory_Common] = true;
        m_bCapabilities[E_RenderCapabilityCategory_Buffer] = true;
        m_bCapabilities[E_RenderCapabilityCategory_Texture] = true;
        m_bCapabilities[E_RenderCapabilityCategory_Shader] = true;

        m_bCapabilities[E_RenderCapabilityCategory_DirectX] = false;
        m_bCapabilities[E_RenderCapabilityCategory_Vulkan] = false;
        m_bCapabilities[E_RenderCapabilityCategory_Metal] = false;
        m_bCapabilities[E_RenderCapabilityCategory_OpenGL] = false;
	}

	RenderCapabilities::~RenderCapabilities()
	{

	}

    void RenderCapabilities::AddShaderProfile(const String& profile)
    {
        m_setShaderProfiles.insert(profile);
    }

    void RenderCapabilities::RemoveShaderProfile(const String& profile)
    {
        m_setShaderProfiles.erase(profile);
    }

    bool RenderCapabilities::IsShaderProfileSupported(const String& profile) const
    {
        return (m_setShaderProfiles.end() != m_setShaderProfiles.find(profile));
    }

    void RenderCapabilities::LogInfo()
    {
		F_LogInfo("---------- GPU Identifier & Features ----------");
        {
            F_LogInfo("Name Renderer: [%s] ", GetNameRenderer().c_str());
            F_LogInfo("Name GPU Vendor: [%s] ", GetGpuVendorName(GetGpuVendorType()).c_str());
            F_LogInfo("Name Device: [%s] ", GetNameDevice().c_str());
            F_LogInfo("Driver Version: [%s] ", GetGpuDriverVersion().ToString().c_str());
            //Value
            F_LogInfo("***** Value *****");
            {
                F_LogInfo("StencilBufferBitDepth: [%d] ", (int)GetStencilBufferBitDepth());
                F_LogInfo("NumVertexAttributes: [%d] ", (int)GetNumVertexAttributes());
                F_LogInfo("NumVertexTextureUnits: [%d] ", (int)GetNumVertexTextureUnits());
                F_LogInfo("NumTextureUnits: [%d] ", (int)GetNumTextureUnits());
                F_LogInfo("NumRenderMultiTargets: [%d] ", (int)GetNumRenderMultiTargets());
                F_LogInfo("MaxPointSize: [%f] ", GetMaxPointSize());
                F_LogInfo("MaxSupportedAnisotropy: [%f] ", GetMaxSupportedAnisotropy());

                F_LogInfo("VertexProgramConstantFloatCount: [%d] ", (int)GetVertexProgramConstantFloatCount());
                F_LogInfo("GeometryProgramConstantFloatCount: [%d] ", (int)GetGeometryProgramConstantFloatCount());
                F_LogInfo("TessellationHullProgramConstantFloatCount: [%d] ", (int)GetTessellationHullProgramConstantFloatCount());
                F_LogInfo("TessellationDomainProgramConstantFloatCount: [%d] ", (int)GetTessellationDomainProgramConstantFloatCount());
                F_LogInfo("ComputeProgramConstantFloatCount: [%d] ", (int)GetComputeProgramConstantFloatCount());
                F_LogInfo("FragmentProgramConstantFloatCount: [%d] ", (int)GetFragmentProgramConstantFloatCount());
            }
            //Common
            F_LogInfo("***** Common *****");
            {
                F_LogInfo("Supports Common PrimitiveRestart: [%s] ", HasCapability(E_RenderCapability_Common_PrimitiveRestart) ? "true" : "false");
                F_LogInfo("Supports Common WideLines: [%s] ", HasCapability(E_RenderCapability_Common_WideLines) ? "true" : "false");
                F_LogInfo("Supports Common HWOcclusion: [%s] ", HasCapability(E_RenderCapability_Common_HWOcclusion) ? "true" : "false");
                F_LogInfo("Supports Common UserClipPlanes: [%s] ", HasCapability(E_RenderCapability_Common_UserClipPlanes) ? "true" : "false");
                F_LogInfo("Supports Common PointSprites: [%s] ", HasCapability(E_RenderCapability_Common_PointSprites) ? "true" : "false");
                F_LogInfo("Supports Common PointExtendedParameters: [%s] ", HasCapability(E_RenderCapability_Common_PointExtendedParameters) ? "true" : "false");
                F_LogInfo("Supports Common FixedFunction: [%s] ", HasCapability(E_RenderCapability_Common_FixedFunction) ? "true" : "false");
                F_LogInfo("Supports Common MRTDifferentBitDepths: [%s] ", HasCapability(E_RenderCapability_Common_MRTDifferentBitDepths) ? "true" : "false");
                F_LogInfo("Supports Common AlphaToCoverage: [%s] ", HasCapability(E_RenderCapability_Common_AlphaToCoverage) ? "true" : "false");
                F_LogInfo("Supports Common HWGamma: [%s] ", HasCapability(E_RenderCapability_Common_HWGamma) ? "true" : "false");
                F_LogInfo("Supports Common RTTMainDepthBufferAttachable: [%s] ", HasCapability(E_RenderCapability_Common_RTTMainDepthBufferAttachable) ? "true" : "false");
                F_LogInfo("Supports Common RTTDepthBufferResolutionLessEqual: [%s] ", HasCapability(E_RenderCapability_Common_RTTDepthBufferResolutionLessEqual) ? "true" : "false");
                F_LogInfo("Supports Common 32BitIndex: [%s] ", HasCapability(E_RenderCapability_Common_32BitIndex) ? "true" : "false");
            }
            //Buffer
            F_LogInfo("***** Buffer *****");
            {
                F_LogInfo("Supports Buffer DepthClamp: [%s] ", HasCapability(E_RenderCapability_Buffer_DepthClamp) ? "true" : "false");
                F_LogInfo("Supports Buffer StencilHW: [%s] ", HasCapability(E_RenderCapability_Buffer_StencilHW) ? "true" : "false");
                F_LogInfo("Supports Buffer StencilTwoSided: [%s] ", HasCapability(E_RenderCapability_Buffer_StencilTwoSided) ? "true" : "false");
                F_LogInfo("Supports Buffer StencilWrap: [%s] ", HasCapability(E_RenderCapability_Buffer_StencilWrap) ? "true" : "false");
                F_LogInfo("Supports Buffer ReadWriteBuffers: [%s] ", HasCapability(E_RenderCapability_Buffer_ReadWriteBuffers) ? "true" : "false");
            }
            //Texture
            F_LogInfo("***** Texture *****");
            {
                F_LogInfo("Supports Texture 1D: [%s] ", HasCapability(E_RenderCapability_Texture_1D) ? "true" : "false");
                F_LogInfo("Supports Texture 2DArray: [%s] ", HasCapability(E_RenderCapability_Texture_2DArray) ? "true" : "false");
                F_LogInfo("Supports Texture 3D: [%s] ", HasCapability(E_RenderCapability_Texture_3D) ? "true" : "false");
                F_LogInfo("Supports Texture AutoMipmapCompressed: [%s] ", HasCapability(E_RenderCapability_Texture_AutoMipmapCompresse) ? "true" : "false");
                F_LogInfo("Supports Texture Anisotropy: [%s] ", HasCapability(E_RenderCapability_Texture_Anisotropy) ? "true" : "false");
                F_LogInfo("Supports Texture HWRenderToTexture: [%s] ", HasCapability(E_RenderCapability_Texture_HWRenderToTexture) ? "true" : "false");
                F_LogInfo("Supports Texture Float: [%s] ", HasCapability(E_RenderCapability_Texture_Float) ? "true" : "false");
                F_LogInfo("Supports Texture NonPower2: [%s] ", HasCapability(E_RenderCapability_Texture_NonPower2) ? "true" : "false");
                F_LogInfo("Supports Texture MipmapLodBias: [%s] ", HasCapability(E_RenderCapability_Texture_MipmapLodBias) ? "true" : "false");
                F_LogInfo("Supports Texture Compression: [%s] ", HasCapability(E_RenderCapability_Texture_Compression) ? "true" : "false");
                F_LogInfo("Supports Texture CompressionDXT: [%s] ", HasCapability(E_RenderCapability_Texture_CompressionDXT) ? "true" : "false");
                F_LogInfo("Supports Texture CompressionVTC: [%s] ", HasCapability(E_RenderCapability_Texture_CompressionVTC) ? "true" : "false");
                F_LogInfo("Supports Texture CompressionPVRTC: [%s] ", HasCapability(E_RenderCapability_Texture_CompressionPVRTC) ? "true" : "false");
                F_LogInfo("Supports Texture CompressionATC: [%s] ", HasCapability(E_RenderCapability_Texture_CompressionATC) ? "true" : "false");
                F_LogInfo("Supports Texture CompressionETC1: [%s] ", HasCapability(E_RenderCapability_Texture_CompressionETC1) ? "true" : "false");
                F_LogInfo("Supports Texture CompressionETC2: [%s] ", HasCapability(E_RenderCapability_Texture_CompressionETC2) ? "true" : "false");
                F_LogInfo("Supports Texture CompressionBC4BC5: [%s] ", HasCapability(E_RenderCapability_Texture_CompressionBC4BC5) ? "true" : "false");
                F_LogInfo("Supports Texture CompressionBC6HBC7: [%s] ", HasCapability(E_RenderCapability_Texture_CompressionBC6HBC7) ? "true" : "false");
                F_LogInfo("Supports Texture CompressionASTC: [%s] ", HasCapability(E_RenderCapability_Texture_CompressionASTC) ? "true" : "false");
            }
            //Shader
            F_LogInfo("***** Shader *****");
            {
                F_LogInfo("Supports Shader ProgramVertex: [%s] ", HasCapability(E_RenderCapability_Shader_ProgramVertex) ? "true" : "false");
                F_LogInfo("Supports Shader ProgramTesslationHull: [%s] ", HasCapability(E_RenderCapability_Shader_ProgramTesslationHull) ? "true" : "false");
                F_LogInfo("Supports Shader ProgramTesslationDomain: [%s] ", HasCapability(E_RenderCapability_Shader_ProgramTesslationDomain) ? "true" : "false");
                F_LogInfo("Supports Shader ProgramGeometry: [%s] ", HasCapability(E_RenderCapability_Shader_ProgramGeometry) ? "true" : "false");
                F_LogInfo("Supports Shader ProgramCompute: [%s] ", HasCapability(E_RenderCapability_Shader_ProgramCompute) ? "true" : "false");
                F_LogInfo("Supports Shader VertexTextureFetch: [%s] ", HasCapability(E_RenderCapability_Shader_VertexTextureFetch) ? "true" : "false");
                F_LogInfo("Supports Shader HWRenderToVertexBuffer: [%s] ", HasCapability(E_RenderCapability_Shader_HWRenderToVertexBuffer) ? "true" : "false");
                F_LogInfo("Supports Shader CanGetCompiledShaderBuffer: [%s] ", HasCapability(E_RenderCapability_Shader_CanGetCompiledShaderBuffer) ? "true" : "false");
                F_LogInfo("Supports Shader VertexBufferInstanceData: [%s] ", HasCapability(E_RenderCapability_Shader_VertexBufferInstanceData) ? "true" : "false");
            }
            //DirectX
            if (IsCategory(E_RenderCapabilityCategory_DirectX))
            {
                F_LogInfo("***** DirectX *****");

                F_LogInfo("Supports DirectX PerStageConstant: [%s] ", HasCapability(E_RenderCapability_DirectX_PerStageConstant) ? "true" : "false");
                F_LogInfo("Supports DirectX ReadBackAsTexture: [%s] ", HasCapability(E_RenderCapability_DirectX_ReadBackAsTexture) ? "true" : "false");
                F_LogInfo("Supports DirectX WBuffer: [%s] ", HasCapability(E_RenderCapability_DirectX_WBuffer) ? "true" : "false");
                F_LogInfo("Supports DirectX HWOcclusionAsynchronous: [%s] ", HasCapability(E_RenderCapability_DirectX_HWOcclusionAsynchronous) ? "true" : "false");
                F_LogInfo("Supports DirectX HWRenderToTexture3D: [%s] ", HasCapability(E_RenderCapability_DirectX_HWRenderToTexture3D) ? "true" : "false");
            }
            //Vulkan
            if (IsCategory(E_RenderCapabilityCategory_Vulkan))
            {
                F_LogInfo("***** Vulkan *****");

            }
            //Metal
            if (IsCategory(E_RenderCapabilityCategory_Metal))
            {
                F_LogInfo("***** Metal *****");

            }
            //OpenGL/ES
            if (IsCategory(E_RenderCapabilityCategory_OpenGL))
            {
                F_LogInfo("***** OpenGL/ES *****");

                F_LogInfo("Supports OpenGL/ES PBuffer: ", HasCapability(E_RenderCapability_OpenGL_PBuffer) ? "true" : "false");
                F_LogInfo("Supports OpenGL/ES SeparateShaderObjects: ", HasCapability(E_RenderCapability_OpenGL_SeparateShaderObjects) ? "true" : "false");
                F_LogInfo("Supports OpenGL/ES VAO: ", HasCapability(E_RenderCapability_OpenGL_VAO) ? "true" : "false");
                F_LogInfo("Supports OpenGL/ES GLSLSSORedeclare: ", HasCapability(E_RenderCapability_OpenGL_GLSLSSORedeclare) ? "true" : "false");
                F_LogInfo("Supports OpenGL/ES Debug: ", HasCapability(E_RenderCapability_OpenGL_Debug) ? "true" : "false");
                F_LogInfo("Supports OpenGL/ES MapBuffer: ", HasCapability(E_RenderCapability_OpenGL_MapBuffer) ? "true" : "false");
            }
        }
        F_LogInfo("---------- GPU Identifier & Features ----------");
    }
    
}; //LostPeterEngine