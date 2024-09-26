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

#include "../include/EditorLineFlat2DCollector.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
    const String EditorLineFlat2DCollector::s_strNameShader_LineFlat2D_Vert = "vert_editor_line_2d";
    const String EditorLineFlat2DCollector::s_strNameShader_LineFlat2D_Frag = "frag_editor_line_2d";
    EditorLineFlat2DCollector::EditorLineFlat2DCollector()
        : EditorBase("EditorLineFlat2DCollector")

        , poBuffers_ObjectCB(VK_NULL_HANDLE)
        , poBuffersMemory_ObjectCB(VK_NULL_HANDLE)
    {

    }
    EditorLineFlat2DCollector::~EditorLineFlat2DCollector()
    {

    }
    void EditorLineFlat2DCollector::Destroy()
    {

    }
    void EditorLineFlat2DCollector::UpdateCBs()
    {

    }
    void EditorLineFlat2DCollector::initConfigs()
    {

    }
    void EditorLineFlat2DCollector::initBufferUniforms()
    {

    }
    void EditorLineFlat2DCollector::initDescriptorSetLayout()
    {
        
    }   
    void EditorLineFlat2DCollector::initPipelineLayout()
    {

    }
    void EditorLineFlat2DCollector::initPipelineGraphics()
    {

    }
    void EditorLineFlat2DCollector::updateDescriptorSets_Graphics()
    {

    }
    void EditorLineFlat2DCollector::destroyBufferUniforms()
    {

    }

}; //LostPeterVulkan