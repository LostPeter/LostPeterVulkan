/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-07
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _SCENE_DATA_SERIALIZER_H_
#define _SCENE_DATA_SERIALIZER_H_

#include "PreDefine.h"

namespace LostPeter
{
    class utilExport SceneDataSerializer
    {
    public:
        SceneDataSerializer(SceneDataManager* pSceneDataManager);
        virtual ~SceneDataSerializer();

    public:
        SceneDataManager* m_pSceneDataManager;

    public:

    protected:

    };

}; //LostPeter

#endif