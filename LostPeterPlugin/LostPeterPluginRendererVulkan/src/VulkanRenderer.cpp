/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-23
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanRenderer.h"
#include "../include/VulkanInstance.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanRenderWindow.h"
#include "../include/VulkanPlugin.h"
#include "../include/VulkanConverter.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanRenderer::VulkanRenderer()
        : Renderer(VulkanPlugin::GetPluginName())
        , m_pVulkanInstance(nullptr)
    {
        
    }

    VulkanRenderer::~VulkanRenderer()
    {
        
    }

    void VulkanRenderer::Destroy()
    {
        Renderer::Destroy();
        F_DELETE(m_pVulkanInstance)
    }

    RenderWindow* VulkanRenderer::Init(bool bAutoCreateWindow, 
                                       const String& strWndTitle /*= "Render Window"*/)
    {
        F_Assert(!m_bRendererIsInit && "VulkanRenderer::Init")

        //1> VulkanInstance/VulkanDevice
        m_pVulkanInstance = new VulkanInstance();
        if (!m_pVulkanInstance->Init())
        {
            F_LogError("*********************** VulkanRenderer::Init: Failed to init VulkanInstance !");
            return nullptr;
        }
        F_LogInfo("VulkanRenderer::Init: 1> Init vulkan instance/device/physical device success !");
        
        //2> Init render capabilities
        if (!initRenderCapabilities())
        {
            F_LogError("*********************** VulkanRenderer::Init: Failed to init render capabilities !");
            return nullptr;
        }
        initFromRenderCapabilities(nullptr);
        F_LogInfo("VulkanRenderer::Init: 2> Init render capabilities success !");

        //3> Auto CreateWindow
        if (bAutoCreateWindow)
        {
            
        }

        return nullptr;
    }

    FVertexElementDataType VulkanRenderer::GetColorVertexElementType() const
    {
        return F_VertexElementData_Float1;
    }

    RenderWindow* VulkanRenderer::CreateRenderWindow(const String& strName, 
                                                     uint32 nWidth, 
                                                     uint32 nHeight, 
												     const String2StringMap* pParams /*= nullptr*/)
    {
        VulkanRenderWindow* pRenderWindow = new VulkanRenderWindow(strName, m_pVulkanInstance->GetDevice());
        if (!pRenderWindow->Init(nWidth,
                                 nHeight,
                                 pParams))
        {
            F_LogError("*********************** VulkanRenderer::CreateRenderWindow: Failed to init render window, name: [%s] !", strName.c_str());
            F_DELETE(pRenderWindow)
            return nullptr;
        }
        
        AttachRenderTarget(pRenderWindow);
        return pRenderWindow;
    }

    bool VulkanRenderer::initRenderCapabilities()
    {
        m_pRenderCapabilities = new RenderCapabilities;
        VulkanDevice* pDevice = GetDevice();
        F_Assert(pDevice && "VulkanRenderer::initRenderCapabilities")
        VkPhysicalDevice vkPhysicalDevice = pDevice->GetVkPhysicalDevice();
        VkPhysicalDeviceProperties* pVkProperties = pDevice->GetVkPhysicalDevicePropertiesPtr();
        VkPhysicalDeviceProperties& properties = pDevice->GetVkPhysicalDeviceProperties();
        const VkPhysicalDeviceLimits& deviceLimits = pDevice->GetVkPhysicalDeviceLimits();
        VkPhysicalDeviceFeatures& features = pDevice->GetVkPhysicalDeviceFeatures();

        //1> Renderer and Device
        m_pRenderCapabilities->SetNameRenderer(GetName());
        vkGetPhysicalDeviceProperties(vkPhysicalDevice, pVkProperties);
        m_pRenderCapabilities->SetNameDevice(properties.deviceName);

        //2> Vulkan API, Vendor, Device
        String strVersion = FUtilString::SaveUInt(VK_VERSION_MAJOR(properties.apiVersion)) + "." +
                            FUtilString::SaveUInt(VK_VERSION_MINOR(properties.apiVersion)) + "." +
                            FUtilString::SaveUInt(VK_VERSION_PATCH(properties.apiVersion));
        F_LogInfo("VulkanRenderer::initRenderCapabilities: Vulkan API Version: [%s] ", strVersion.c_str());
        String strVendorID = FUtilString::FormatString("%#x", properties.vendorID);
        F_LogInfo("VulkanRenderer::initRenderCapabilities: Vulkan Vendor ID: [%s] ", strVendorID.c_str());
        String strDeviceID = FUtilString::FormatString("%#x", properties.deviceID);
        F_LogInfo("VulkanRenderer::initRenderCapabilities: Vulkan Device ID: [%s] ", strDeviceID.c_str());

        switch (properties.vendorID)
        {
        case 0x10DE:
            {
                m_pRenderCapabilities->SetGpuVendorType(E_GpuVendor_Nvidia);
                
                // 10 bits = major version (up to r1023)
                // 8 bits = minor version (up to 255)
                // 8 bits = secondary branch version/build version (up to 255)
                // 6 bits = tertiary branch/build version (up to 63)
                GpuDriverVersion driverVersion;
                driverVersion.m_nMajor = (properties.driverVersion >> 22u) & 0x3ff;
                driverVersion.m_nMinor = (properties.driverVersion >> 14u) & 0x0ff;
                driverVersion.m_nRelease = (properties.driverVersion >> 6u) & 0x0ff;
                driverVersion.m_nBuild = (properties.driverVersion) & 0x003f;
                m_pRenderCapabilities->SetGpuDriverVersion(driverVersion);
            }
            break;
        case 0x1002:
            {
                m_pRenderCapabilities->SetGpuVendorType(E_GpuVendor_AMD);
            }
            break;
        case 0x8086:
            {
                m_pRenderCapabilities->SetGpuVendorType(E_GpuVendor_Intel);
            }
            break;
        case 0x13B5:
            {
                m_pRenderCapabilities->SetGpuVendorType(E_GpuVendor_Arm);
            }
            break;
        case 0x5143:
            {
                m_pRenderCapabilities->SetGpuVendorType(E_GpuVendor_Qualcomm);
            }
            break;
        case 0x1010:
            {
                m_pRenderCapabilities->SetGpuVendorType(E_GpuVendor_Imagination);
            }
            break;
        }

        if (m_pRenderCapabilities->GetGpuVendorType() != E_GpuVendor_Nvidia)
        {
            // Generic version routine that matches SaschaWillems's VulkanCapsViewer
            GpuDriverVersion driverVersion;
            driverVersion.m_nMajor = (properties.driverVersion >> 22u) & 0x3ff;
            driverVersion.m_nMinor = (properties.driverVersion >> 12u) & 0x3ff;
            driverVersion.m_nRelease = (properties.driverVersion) & 0xfff;
            driverVersion.m_nBuild = 0;
            m_pRenderCapabilities->SetGpuDriverVersion(driverVersion);
        }

        //3> RenderCapability Common
        {
            m_pRenderCapabilities->SetCapability(E_RenderCapability_Common_UserClipPlanes);
            m_pRenderCapabilities->SetCapability(E_RenderCapability_Common_PointSprites);
            m_pRenderCapabilities->SetCapability(E_RenderCapability_Common_PointExtendedParameters);
            m_pRenderCapabilities->SetMaxPointSize(256);
            m_pRenderCapabilities->SetCapability(E_RenderCapability_Common_AlphaToCoverage);
            m_pRenderCapabilities->SetCapability(E_RenderCapability_Common_HWGamma);
            m_pRenderCapabilities->SetCapability(E_RenderCapability_Common_32BitIndex);

            m_pRenderCapabilities->SetStencilBufferBitDepth(8);
            m_pRenderCapabilities->SetNumVertexAttributes(C_CONFIG_MAX_STREAM_SOURCE_COUNT);
            m_pRenderCapabilities->SetNumVertexTextureUnits(C_CONFIG_MAX_TEXTURE_COORD_SETS_COUNT);
            m_pRenderCapabilities->SetNumTextureUnits(C_CONFIG_MAX_TEXTURE_UNIT_COUNT);
            m_pRenderCapabilities->SetNumRenderMultiTargets(C_CONFIG_MAX_RENDER_TARGET_MULTIPLE_COUNT);
        }

        //4> RenderCapability Buffer
        {
            if (features.depthClamp)
                m_pRenderCapabilities->SetCapability(E_RenderCapability_Buffer_DepthClamp);
            m_pRenderCapabilities->SetCapability(E_RenderCapability_Buffer_StencilTwoSided);
            m_pRenderCapabilities->SetCapability(E_RenderCapability_Buffer_StencilWrap);
        }

        //5> RenderCapability Texture
        {
            m_pRenderCapabilities->SetCapability(E_RenderCapability_Texture_1D);
            m_pRenderCapabilities->SetCapability(E_RenderCapability_Texture_2DArray);
            m_pRenderCapabilities->SetCapability(E_RenderCapability_Texture_3D);
            if (features.samplerAnisotropy && deviceLimits.maxSamplerAnisotropy > 1u)
            {
                m_pRenderCapabilities->SetCapability(E_RenderCapability_Texture_Anisotropy);
                m_pRenderCapabilities->SetMaxSupportedAnisotropy(deviceLimits.maxSamplerAnisotropy);
            }
            m_pRenderCapabilities->SetCapability(E_RenderCapability_Texture_HWRenderToTexture);
            m_pRenderCapabilities->SetCapability(E_RenderCapability_Texture_Float);
            m_pRenderCapabilities->SetCapability(E_RenderCapability_Texture_NonPower2);
            m_pRenderCapabilities->SetNonPOW2TexturesLimited(false);
            m_pRenderCapabilities->SetCapability(E_RenderCapability_Texture_Compression);

            VkFormatProperties props;
            //DXT
            vkGetPhysicalDeviceFormatProperties(vkPhysicalDevice,
                                                VulkanConverter::Transform2VkFormat(F_PixelFormat_DXT1), &props);
            if (props.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT)
                m_pRenderCapabilities->SetCapability(E_RenderCapability_Texture_CompressionDXT);

            //PVRTC
            vkGetPhysicalDeviceFormatProperties(vkPhysicalDevice,
                                                VulkanConverter::Transform2VkFormat(F_PixelFormat_PVRTC_RGB2), &props);
            if (props.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT )
                m_pRenderCapabilities->SetCapability(E_RenderCapability_Texture_CompressionPVRTC);

            //Vulkan doesn't allow supporting ETC1 without ETC2
            vkGetPhysicalDeviceFormatProperties(vkPhysicalDevice,
                                                VulkanConverter::Transform2VkFormat(F_PixelFormat_ETC2_RGB8), &props);
            if (props.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT)
            {
                m_pRenderCapabilities->SetCapability(E_RenderCapability_Texture_CompressionETC1);
                m_pRenderCapabilities->SetCapability(E_RenderCapability_Texture_CompressionETC2);
            }
            
            //BC4BC5
            vkGetPhysicalDeviceFormatProperties(vkPhysicalDevice,
                                                VulkanConverter::Transform2VkFormat(F_PixelFormat_BC4_UNORM), &props);
            if (props.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT)
                m_pRenderCapabilities->SetCapability(E_RenderCapability_Texture_CompressionBC4BC5);

            //BC6HBC7
            vkGetPhysicalDeviceFormatProperties(vkPhysicalDevice,
                                                VulkanConverter::Transform2VkFormat(F_PixelFormat_BC6H_UF16), &props);
            if (props.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT)
                m_pRenderCapabilities->SetCapability(E_RenderCapability_Texture_CompressionBC6HBC7);

            //ASTC
            vkGetPhysicalDeviceFormatProperties(vkPhysicalDevice, 
                                                VulkanConverter::Transform2VkFormat(F_PixelFormat_ASTC_RGBA_4X4_LDR), &props);
            if (props.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT)
                m_pRenderCapabilities->SetCapability(E_RenderCapability_Texture_CompressionASTC);
        }

        //6> RenderCapability Shader
        {
            m_pRenderCapabilities->SetCapability(E_RenderCapability_Shader_ProgramVertex);
            m_pRenderCapabilities->SetCapability(E_RenderCapability_Shader_ProgramTesslationHull);
            m_pRenderCapabilities->SetCapability(E_RenderCapability_Shader_ProgramTesslationDomain);
            m_pRenderCapabilities->SetCapability(E_RenderCapability_Shader_ProgramGeometry);
            m_pRenderCapabilities->SetCapability(E_RenderCapability_Shader_ProgramCompute);
            m_pRenderCapabilities->SetCapability(E_RenderCapability_Shader_VertexTextureFetch);
            m_pRenderCapabilities->SetCapability(E_RenderCapability_Shader_HWRenderToVertexBuffer);
            m_pRenderCapabilities->SetCapability(E_RenderCapability_Shader_CanGetCompiledShaderBuffer);
            m_pRenderCapabilities->SetCapability(E_RenderCapability_Shader_VertexBufferInstanceData);

            m_pRenderCapabilities->AddShaderProfile("spirv");
            uint16 maxFloatVectors = uint16(deviceLimits.maxUniformBufferRange / (4 * sizeof(float)));
            m_pRenderCapabilities->SetVertexProgramConstantFloatCount(maxFloatVectors);
            m_pRenderCapabilities->SetGeometryProgramConstantFloatCount(maxFloatVectors);
            m_pRenderCapabilities->SetTessellationHullProgramConstantFloatCount(maxFloatVectors);
            m_pRenderCapabilities->SetTessellationDomainProgramConstantFloatCount(maxFloatVectors);
            m_pRenderCapabilities->SetComputeProgramConstantFloatCount(maxFloatVectors);
            m_pRenderCapabilities->SetFragmentProgramConstantFloatCount(maxFloatVectors);
        }

        //7> RenderCapability Vulkan
        {
            m_pRenderCapabilities->SetCategory(E_RenderCapabilityCategory_Vulkan, true);

            
        }

        m_pRenderCapabilities->LogInfo();
        return true;   
    }
	void VulkanRenderer::initFromRenderCapabilities(RenderTarget* pPrimary)
    {

    }

    VulkanDevice* VulkanRenderer::GetDevice() 
    { 
        return m_pVulkanInstance->GetDevice(); 
    }

    bool VulkanRenderer::BeginFrame()
    {   
        
        return true;
    }
    bool VulkanRenderer::EndFrame()
    {
        return true;
    }

    bool VulkanRenderer::SetViewport(Viewport* pViewport)
    {
        return true;
    }
    bool VulkanRenderer::SetRenderTarget(RenderTarget* pRenderTarget)
    {
        return true;
    }
    bool VulkanRenderer::SetDepthStencil(void* pDepth)
    {
        return true;
    }

    bool VulkanRenderer::ClearFrameBuffer(uint32 nBuffers, 
                                          const FColor& color /*= FMath::ms_clBlack*/, 
                                          float fDepth /*= 1.0f*/, 
                                          uint16 nStencil /*= 0*/)
    {
        return true;
    }
    bool VulkanRenderer::ClearFrameBuffer(const FRectI& rect, 
                                          uint32 nBuffers, 
                                          const FColor& color /*= FMath::ms_clBlack*/, 
                                          float fDepth /*= 1.0f*/, 
                                          uint16 nStencil /*= 0*/)
    {
        return true;
    }

    bool VulkanRenderer::SetWorldMatrix(const FMatrix4& matWorld)
    {
        return true;
    }
    bool VulkanRenderer::SetWorldMatrices(const FMatrix4* pMatWorld, int nCount)
    {
        return true;
    }
    bool VulkanRenderer::SetViewMatrix(const FMatrix4& matView)
    {
        return true;
    }
    bool VulkanRenderer::SetProjectionMatrix(const FMatrix4& matProj)
    {
        return true;
    }

}; //LostPeterPluginRendererVulkan