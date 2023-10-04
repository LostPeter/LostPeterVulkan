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

#ifndef _EDITOR_GRID_H_
#define _EDITOR_GRID_H_

#include "EditorBase.h"

namespace LostPeter
{
    class utilExport EditorGrid : public EditorBase
    {
    public:
        EditorGrid();
        virtual ~EditorGrid();

    public:
        static const String s_strNameShader_Grid_Vert;
        static const String s_strNameShader_Grid_Frag;

    public:
        GridObjectConstants gridObjectCB;
        VkBuffer poBuffers_ObjectCB;
        VkDeviceMemory poBuffersMemory_ObjectCB;

        void SetColor(FColor& color)
        {
            this->gridObjectCB.color = color;
            SetIsNeedUpdate(true);
        }

    protected:
        bool isNeedUpdate;
    public:
        LP_FORCEINLINE bool IsNeedUpdate() const { return this->isNeedUpdate; }
        LP_FORCEINLINE void SetIsNeedUpdate(bool b) { this->isNeedUpdate = b; }

    public:
        virtual void Destroy();

        virtual void Init();

        virtual void UpdateCBs();

    public:
        virtual void CleanupSwapChain();
        virtual void RecreateSwapChain();

    protected:
        virtual void initConfigs();
        virtual void initBufferUniforms();
        virtual void initPipelineGraphics();
        virtual void updateDescriptorSets_Graphics();

        virtual void destroyBufferUniforms();
        virtual void destroyPipelineGraphics();
    };

}; //LostPeter

#endif