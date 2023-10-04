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

#ifndef _EDITOR_LINE_FLAT_3D_COLLECTOR_H_
#define _EDITOR_LINE_FLAT_3D_COLLECTOR_H_

#include "EditorBase.h"

namespace LostPeter
{
    class utilExport EditorLineFlat3DCollector : public EditorBase
    {
    public:
        EditorLineFlat3DCollector();
        virtual ~EditorLineFlat3DCollector();

    public:
        static const String s_strNameShader_LineFlat3D_Vert;
        static const String s_strNameShader_LineFlat3D_Frag;
        
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

}; //LostPeter

#endif