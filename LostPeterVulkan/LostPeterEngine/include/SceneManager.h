/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
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

namespace LostPeterEngine
{
    class utilExport SceneManager : public Base
    {
    public:
        SceneManager(const String& nameSceneManager, Scene* pScene);
        virtual ~SceneManager();

    public:
        Scene* m_pScene;

    public:
        LP_FORCEINLINE Scene* GetScene() const { return this->m_pScene; }

    public:
        void Destroy();
        bool Init();

    public:
        virtual void UpdateScene(Viewport* pViewport);
        
        virtual void RenderScene(Scene* pScene);

    protected:
        

    };

}; //LostPeterEngine

#endif