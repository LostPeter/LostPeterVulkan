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

#include "../include/RenderEngine.h"
#include "../include/MeshManager.h"
#include "../include/TextureManager.h"
#include "../include/ShaderManager.h"
#include "../include/MaterialDataManager.h"
#include "../include/MaterialManager.h"
#include "../include/SceneManagerEnumerator.h"
#include "../include/SceneManager.h"
#include "../include/Viewport.h"
#include "../include/Renderer.h"
#include "../include/RenderTarget.h"
#include "../include/RenderPipelineManager.h"
#include "../include/RenderPipeline.h"

template<> LostPeter::RenderEngine* LostPeterFoundation::FSingleton<LostPeter::RenderEngine>::ms_Singleton = nullptr;

namespace LostPeter
{
    RenderEngine* RenderEngine::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	RenderEngine& RenderEngine::GetSingleton()
	{  
		F_Assert(ms_Singleton && "RenderEngine::GetSingleton")
		return (*ms_Singleton);     
	}


    RenderEngine::RenderEngine()
        : Base("RenderEngine")
    //Core
        , m_pMeshManager(nullptr)
        , m_pTextureManager(nullptr)
        , m_pShaderManager(nullptr)
        , m_pMaterialDataManager(nullptr)
        , m_pMaterialManager(nullptr)
    //Object
        , m_pSceneManagerEnumerator(nullptr)
    {

    }

    RenderEngine::~RenderEngine()
    {

    }

    void RenderEngine::Destroy()
    {

    }
    
    bool RenderEngine::Init()
    {

        return true;
    }


    void RenderEngine::RenderScene(SceneManager* pSceneManager, Viewport* pViewport)
    {
        if (!pSceneManager || !pViewport)
            return;

        RenderTarget* pRenderTarget = pViewport->GetRenderTarget();
        if (pRenderTarget)
        {
            
        }
    }

}; //LostPeter