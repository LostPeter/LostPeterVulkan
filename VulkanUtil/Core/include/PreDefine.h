// author: LostPeter
// time:   2022-10-30

#ifndef _PRE_DEFINE_H_
#define _PRE_DEFINE_H_

#include "PreIncludeStd.h"
#include "PreIncludeThird.h"


namespace LostPeter
{
////Define
    #define MAX_LIGHT_COUNT 16
    #define MAX_OBJECT_COUNT 1024
    #define MAX_MATERIAL_COUNT 128
    #define MAX_INSTANCE_COUNT 1024

////Typedef
    using int8 = std::int8_t;
    using uint8 = std::uint8_t;
    using int16 = std::int16_t;
    using int32 = std::int32_t;
    using uint16 = std::uint16_t;
    using uint32 = std::uint32_t;
    using int64 = std::int64_t;
    using uint64 = std::uint64_t;
    typedef std::chrono::steady_clock::time_point TimePoint;

    #define UTIL_ARRAYSIZE(_ARR)            ((int)(sizeof(_ARR)/sizeof(*_ARR)))    
    #define UTIL_OFFSETOF(_TYPE,_MEMBER)    ((size_t)&(((_TYPE*)0)->_MEMBER))     
    
    #define UTIL_DELETE(p)                  { if(p) { delete p; p=nullptr; }}
    #define UTIL_DELETE_T(p)                { if(p) { delete[] p; p=nullptr; }}

    std::string Utile_VkResult2String(VkResult result);

    #define UTIL_VK_CHECK(vkcall) \
    { \
        VkResult result = vkcall; \
        if (result != VK_SUCCESS) \
        { \
            std::string vkfunc = #vkcall; \
            vkfunc = vkfunc.substr(0, vkfunc.find('(')); \
            Util_LogError("UTIL_VK_CHECK: [%s] failed with: %s", vkfunc.c_str(), Utile_VkResult2String(result).c_str()); \
        } \
    }

    bool Util_CheckVkResult(VkResult result, const std::string& nameFunc);


////Enum
    enum VulkanLogType
    {
        Vulkan_Log_Console = 0,
        Vulkan_Log_File,
    };

    enum VulkanVertexType
    {
        Vulkan_VertexType_Pos2Color4 = 0,
        Vulkan_VertexType_Pos3Normal3,
        Vulkan_VertexType_Pos2Color4Tex2,
        Vulkan_VertexType_Pos3Color4Tex2,
        Vulkan_VertexType_Pos3Color4Normal3Tex2,
        Vulkan_VertexType_Pos3Color4Normal3Tangent3Tex2,

        Count
    };

    enum VulkanSwapStatusType
    {
        Vulkan_SwapStatus_Normal = 0,
        Vulkan_SwapStatus_OutOfDate = -1,
        Vulkan_SwapStatus_Lost = -2,
        Vulkan_SwapStatus_Error = -3,
    };

    enum VulkanPixelFormatType
    {
        Vulkan_PixelFormat_Unknown = 0,

        Vulkan_PixelFormat_BYTE_A8R8G8B8_UNORM,
    };


////Vulkan
    typedef std::vector<VkVertexInputBindingDescription> VkVertexInputBindingDescriptionVector;
    typedef std::vector<VkVertexInputAttributeDescription> VkVertexInputAttributeDescriptionVector;

    const VkVertexInputBindingDescriptionVector& Util_GetVkVertexInputBindingDescriptionVector(VulkanVertexType type);
    const VkVertexInputAttributeDescriptionVector& Util_GetVkVertexInputAttributeDescriptionVector(VulkanVertexType type);
    VkVertexInputBindingDescriptionVector* Util_GetVkVertexInputBindingDescriptionVectorPtr(VulkanVertexType type);
    VkVertexInputAttributeDescriptionVector* Util_GetVkVertexInputAttributeDescriptionVectorPtr(VulkanVertexType type);

    typedef std::vector<VkDescriptorSetLayout> VkDescriptorSetLayoutVector;

    
    typedef std::vector<VkShaderModule> VkShaderModuleVector;
    typedef std::map<std::string, VkShaderModule> VkShaderModuleMap;

    typedef std::vector<VkPipeline> VkPipelineVector;
    typedef std::map<std::string, VkPipeline> VkPipelineMap;


////Class
    class App;
    
    class VulkanBase;
    class VulkanCamera;
    class VulkanDevice;
    class VulkanInstance;
    class VulkanLog;
    class VulkanLogConsole;
    class VulkanLogFile;
    class VulkanLogManager;
    class VulkanManager;
    class VulkanObject;
    class VulkanQueue;
    class VulkanSceneManager;
    class VulkanSceneObject;
    class VulkanSwapChain;
    class VulkanTimer;
    class VulkanWindow;
    
    struct Vertex_Pos2Color4;
    struct Vertex_Pos3Normal3;
    struct Vertex_Pos2Color4Tex2;
    struct Vertex_Pos3Color4Tex2;
    struct Vertex_Pos3Color4Normal3Tex2;
    struct Vertex_Pos3Color4Normal3Tangent3Tex2;
    typedef Vertex_Pos3Color4Normal3Tangent3Tex2 MeshVertex;

    struct MeshData;
    struct SubmeshGeometry;
    struct MeshGeometry;

    struct Light;
    struct Texture;
    struct Material;
    
    struct PassConstants;
    struct ObjectConstants;
    struct MaterialConstants;
    struct InstanceConstants;

    typedef std::vector<VulkanLog*> VulkanLogPtrVector;
    typedef std::map<std::string, VulkanLog*> VulkanLogPtrMap;


}; //LostPeter


#endif