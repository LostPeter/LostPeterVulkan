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

#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport SceneManager : public Base
    {
    public:
        SceneManager(const String& nameSceneManager);
        virtual ~SceneManager();

    public:
        ScenePtrVector m_aScenes;
        ScenePtrMap m_mapScenes;

    public:
        LP_FORCEINLINE const ScenePtrVector& GetScenePtrVector() const { return this->m_aScenes; }
        LP_FORCEINLINE ScenePtrVector& GetScenePtrVector() { return this->m_aScenes; }
        LP_FORCEINLINE const ScenePtrMap& GetScenePtrMap() const { return this->m_mapScenes; }
        LP_FORCEINLINE ScenePtrMap& GetScenePtrMap() { return this->m_mapScenes; }

    public:
        void Destroy();
        bool Init();

    public:
        

    protected:


    };

}; //LostPeter

#endif