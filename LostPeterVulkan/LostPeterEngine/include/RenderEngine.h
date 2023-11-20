/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-16
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RENDER_ENGINE_H_
#define _RENDER_ENGINE_H_

#include "Base.h"

namespace LostPeterEngine
{
    class utilExport RenderEngine : public FSingleton<RenderEngine>
                                  , public Base
    {
    public:
        RenderEngine();
        virtual ~RenderEngine();

    public:
    
    //Core
    protected:
        

        MeshManager* m_pMeshManager;
        TextureManager* m_pTextureManager;
        ShaderManager* m_pShaderManager;
        
        MaterialDataManager* m_pMaterialDataManager;
        MaterialManager* m_pMaterialManager;


    //Object
    protected:
        SceneManagerEnumerator* m_pSceneManagerEnumerator;


    public:
        static RenderEngine& GetSingleton();
		static RenderEngine* GetSingletonPtr();

    public:
        void Destroy();
        virtual bool Init();



        
    public:
        void RenderScene(SceneManager* pSceneManager, Viewport* pViewport);

    };

}; //LostPeterEngine

#endif