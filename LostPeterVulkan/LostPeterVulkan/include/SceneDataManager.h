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

#ifndef _SCENE_DATA_MANAGER_H_
#define _SCENE_DATA_MANAGER_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport SceneDataManager : public FSingleton<SceneDataManager>
                                      , public Base
    {
        friend class SceneDataSerializer;

    public:
        SceneDataManager();
        virtual ~SceneDataManager();

    public:
    protected:
        SceneDataSerializer* m_pSceneDataSerializer;
		MaterialDataPtrMap m_mapMaterialData;

    public:
        static SceneDataManager& GetSingleton();
		static SceneDataManager* GetSingletonPtr();

    public:
        void Destroy();
        

    public:
        

    protected:


    };

}; //LostPeter

#endif