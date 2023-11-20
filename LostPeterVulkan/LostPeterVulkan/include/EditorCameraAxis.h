/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _EDITOR_CAMERA_AXIS_H_
#define _EDITOR_CAMERA_AXIS_H_

#include "EditorBase.h"

namespace LostPeterVulkan
{
    class utilExport EditorCameraAxis : public EditorBase
    {
    public:
        EditorCameraAxis();
        virtual ~EditorCameraAxis();

    public:
        static size_t s_nMeshConeIndex;
        static size_t s_nMeshAABBIndex;
        static size_t s_nMeshQuadIndex;
        static size_t s_nMeshCameraAxisCount;

        static const String s_strNameShader_CameraAxis_Vert;
        static const String s_strNameShader_CameraAxis_Frag;
        static const String s_strNameShader_QuadBlit_Vert;
        static const String s_strNameShader_QuadBlit_Frag;

        static const float s_fBlitAreaWidth;
        static const float s_fBlitAreaHeight;

        static FMatrix4 s_aMatrix4Transforms[7];

        static float s_fCameraDistance;
        static FVector3 s_vCameraPos;
        static FVector3 s_vCameraLookTarget;
        static FVector3 s_vCameraUp;
        static float s_fCameraFOV;
        static float s_fCameraAspectRatio;
        static float s_fCameraZNear;
        static float s_fCameraZFar;

    //CameraAxis
    public:
        FCamera* pCamera;
        VkViewport poViewport;
        VkRect2D poScissor;
        VkOffset2D poOffset;
        VkExtent2D poExtent;
        FColor poColorBackground;

        PassConstants passCB;
        VkBuffer poBuffers_PassCB;
        VkDeviceMemory poBuffersMemory_PassCB;

        std::vector<CameraAxisObjectConstants> cameraAxisObjectCBs;
        VkBuffer poBuffers_ObjectCB;
        VkDeviceMemory poBuffersMemory_ObjectCB;

    //Quad Blit
    public:
        //DescriptorSetLayouts
        String nameDescriptorSetLayout_CopyBlit; 
        StringVector aNameDescriptorSetLayouts_CopyBlit;
        VkDescriptorSetLayout poDescriptorSetLayout_CopyBlit;

        //PipelineLayout
        VkPipelineLayout poPipelineLayout_CopyBlit;

        //VKPipelineGraphics
        VKPipelineGraphics* pPipelineGraphics_CopyBlit;

        //Uniform Buffer
        CopyBlitObjectConstants copyBlitObjectCB;
        VkBuffer poBuffers_CopyBlitObjectCB;
        VkDeviceMemory poBuffersMemory_CopyBlitObjectCB;

    protected:
        bool isNeedUpdate;
    public:
        LP_FORCEINLINE bool IsNeedUpdate() const { return this->isNeedUpdate; }
        LP_FORCEINLINE void SetIsNeedUpdate(bool b) { this->isNeedUpdate = b; }

    public:
        virtual void Destroy();

        virtual void Init();

        virtual void UpdateCBs();
        virtual void Draw(VkCommandBuffer& commandBuffer);
        virtual void DrawQuad(VkCommandBuffer& commandBuffer);

    public:
        virtual void CleanupSwapChain();
        virtual void RecreateSwapChain();

    protected:
        virtual void initConfigs();
            virtual void initCamera();
            virtual void initViewport();
        virtual void initBufferUniforms();
        virtual void initDescriptorSetLayout();
        virtual void initPipelineLayout();
        virtual void initPipelineGraphics();
        virtual void updateDescriptorSets_Graphics();

        virtual void destroyBufferUniforms();
        virtual void destroyPipelineGraphics();
        virtual void destroyPipelineLayout();
        virtual void destroyDescriptorSetLayout();
    };

}; //LostPeterVulkan

#endif