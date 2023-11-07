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

#ifndef _SCENE_H_
#define _SCENE_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport Scene : public Base
    {
    public:
        Scene(const String& nameScene);
        virtual ~Scene();

    public:
    ////Viewport
        ViewportPtrVector m_aViewports;
        ViewportPtrMap m_mapViewports;
        Viewport* m_pViewportMain;

    ////SceneNode
        SceneNodePtrVector m_aSceneNodes;
        SceneNodePtrMap m_mapSceneNodes;
        SceneNode* m_pSceneNodeRoot;

    ////Object
        ObjectPtrVector m_aObjects;
        ObjectPtrMap m_mapObjects;

    public:
    public:
        void Destroy();
        bool Init();


    ////Viewport
    public:
        LP_FORCEINLINE const ViewportPtrVector& GetViewportPtrVector() const { return this->m_aViewports; }
        LP_FORCEINLINE ViewportPtrVector& GetViewportPtrVector() { return this->m_aViewports; }
        LP_FORCEINLINE const ViewportPtrMap& GetViewportPtrMap() const { return this->m_mapViewports; }
        LP_FORCEINLINE ViewportPtrMap& GetViewportPtrMap() { return this->m_mapViewports; }
        

    ////SceneNode
    public:
        LP_FORCEINLINE const SceneNodePtrVector& GetSceneNodePtrVector() const { return this->m_aSceneNodes; }
        LP_FORCEINLINE SceneNodePtrVector& GetSceneNodePtrVector() { return this->m_aSceneNodes; }
        LP_FORCEINLINE const SceneNodePtrMap& GetSceneNodePtrMap() const { return this->m_mapSceneNodes; }
        LP_FORCEINLINE SceneNodePtrMap& GetSceneNodePtrMap() { return this->m_mapSceneNodes; }


    ////Object
    public:
        LP_FORCEINLINE const ObjectPtrVector& GetObjectPtrVector() const { return this->m_aObjects; }
        LP_FORCEINLINE ObjectPtrVector& GetObjectPtrVector() { return this->m_aObjects; }
        LP_FORCEINLINE const ObjectPtrMap& GetObjectPtrMap() const { return this->m_mapObjects; }
        LP_FORCEINLINE ObjectPtrMap& GetObjectPtrMap() { return this->m_mapObjects; }

    };

}; //LostPeter

#endif