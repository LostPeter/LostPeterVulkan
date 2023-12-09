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

#ifndef _EDITOR_LINE_FLAT_2D_COLLECTOR_H_
#define _EDITOR_LINE_FLAT_2D_COLLECTOR_H_

#include "EditorBase.h"

namespace LostPeterVulkan
{
    class vulkanExport EditorLineFlat2DCollector : public EditorBase
    {
    public:
        EditorLineFlat2DCollector();
        virtual ~EditorLineFlat2DCollector();

    public:
        static const String s_strNameShader_LineFlat2D_Vert;
        static const String s_strNameShader_LineFlat2D_Frag;

    public:
        LineFlat2DObjectConstants lineFlat2DObjectCB;
        VkBuffer poBuffers_ObjectCB;
        VkDeviceMemory poBuffersMemory_ObjectCB;

        

    public:
        virtual void Destroy();

        virtual void UpdateCBs();

    protected:
        virtual void initConfigs();
        
        virtual void initBufferUniforms();
        
        virtual void initPipelineGraphics();
        virtual void updateDescriptorSets_Graphics();

        virtual void destroyBufferUniforms();
    };

}; //LostPeterVulkan

#endif