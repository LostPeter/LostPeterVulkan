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

#ifndef _SCENE_MANAGER_ENUMERATOR_H_
#define _SCENE_MANAGER_ENUMERATOR_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport SceneManagerEnumerator : public FSingleton<SceneManagerEnumerator>
                                            , public Base
    {
    public:
        SceneManagerEnumerator();
        virtual ~SceneManagerEnumerator();

    public:
        SceneManagerPtrVector m_aSceneManagers;
        SceneManagerPtrMap m_mapSceneManagers;

    public:
        LP_FORCEINLINE const SceneManagerPtrVector& GetSceneManagerPtrVector() const { return this->m_aSceneManagers; }
        LP_FORCEINLINE SceneManagerPtrVector& GetSceneManagerPtrVector() { return this->m_aSceneManagers; }
        LP_FORCEINLINE const SceneManagerPtrMap& GetSceneManagerPtrMap() const { return this->m_mapSceneManagers; }
        LP_FORCEINLINE SceneManagerPtrMap& GetSceneManagerPtrMap() { return this->m_mapSceneManagers; }

    public:
        static SceneManagerEnumerator& GetSingleton();
		static SceneManagerEnumerator* GetSingletonPtr();

    public:
        void Destroy();
        bool Init();

    public:
        

    protected:

    };

}; //LostPeter

#endif