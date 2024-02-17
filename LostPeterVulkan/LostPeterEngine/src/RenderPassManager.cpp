/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-21
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RenderPassManager.h"
#include "../include/RenderPass.h"
#include "../include/RenderPassReflection.h"
#include "../include/RenderPassRefraction.h"
#include "../include/RenderPassDepthOnly.h"
#include "../include/RenderPassGBuffer.h"
#include "../include/RenderPassShadowCaster.h"
#include "../include/RenderPassForwardLit.h"
#include "../include/RenderPassDeferredLit.h"
#include "../include/RenderPassSky.h"
#include "../include/RenderPassSkinMesh.h"
#include "../include/RenderPassMesh.h"
#include "../include/RenderPassGrass.h"
#include "../include/RenderPassTree.h"
#include "../include/RenderPassTerrain.h"
#include "../include/RenderPassWater.h"
#include "../include/RenderPassOcean.h"
#include "../include/RenderPassBillboard.h"
#include "../include/RenderPassParticle.h"
#include "../include/RenderPassLensFlare.h"
#include "../include/RenderPassVolumeFog.h"
#include "../include/RenderPassCameraFog.h"
#include "../include/RenderPassPostProcess.h"
#include "../include/RenderPassUI3D.h"
#include "../include/RenderPassUI.h"
#include "../include/RenderPassBackBuffer.h"

template<> LostPeterEngine::RenderPassManager* LostPeterFoundation::FSingleton<LostPeterEngine::RenderPassManager>::ms_Singleton = nullptr;

namespace LostPeterEngine
{
    RenderPassManager* RenderPassManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	RenderPassManager& RenderPassManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "RenderPassManager::GetSingleton")
		return (*ms_Singleton);     
	}

    RenderPassManager::RenderPassManager()
        : Base("RenderPassManager")
    {
        memset(m_ppRenderPass, 0, sizeof(RenderPass*) * F_RenderPass_Count);
        Init();
    }
    
    RenderPassManager::~RenderPassManager()
    {
        Destroy();
    }

    void RenderPassManager::Destroy()
    {
        for (int i = 0; i < F_RenderPass_Count; i++)
		{	
			F_DELETE(m_ppRenderPass[i])
		}
        m_mapRenderPass.clear();
    }

    bool RenderPassManager::Init()
    {
        Destroy();

        //Built-In RenderPass
        m_ppRenderPass[F_RenderPass_Reflection	    ] = new RenderPassReflection();         //0		RenderPassReflection			
		m_ppRenderPass[F_RenderPass_Refraction	    ] = new RenderPassRefraction();		    //1		RenderPassRefraction		
		m_ppRenderPass[F_RenderPass_DepthOnly	    ] = new RenderPassDepthOnly();		    //2		RendPass_DepthBuffer		
		m_ppRenderPass[F_RenderPass_GBuffer		    ] = new RenderPassGBuffer();		    //3		RendPass_Gbuffer	
		m_ppRenderPass[F_RenderPass_ShadowCaster	] = new RenderPassShadowCaster();		//4		RendPass_ShadowMask	
		m_ppRenderPass[F_RenderPass_ForwardLit		] = new RenderPassForwardLit();			//5		RenderPassForwardLit	
		m_ppRenderPass[F_RenderPass_DeferredLit		] = new RenderPassDeferredLit();		//6		RenderPassDeferredLit	
		m_ppRenderPass[F_RenderPass_Sky	            ] = new RenderPassSky();				//7		RenderPassSky				
		m_ppRenderPass[F_RenderPass_SkinMesh		] = new RenderPassSkinMesh();			//8		RenderPassSkinMesh					
		m_ppRenderPass[F_RenderPass_Mesh		    ] = new RenderPassMesh();				//9		RenderPassMesh				
		m_ppRenderPass[F_RenderPass_Grass		    ] = new RenderPassGrass();				//10	RenderPassGrass			
		m_ppRenderPass[F_RenderPass_Tree		    ] = new RenderPassTree();				//11	RenderPassTree		
		m_ppRenderPass[F_RenderPass_Terrain		    ] = new RenderPassTerrain();			//12	RenderPassTerrain					
		m_ppRenderPass[F_RenderPass_Water		    ] = new RenderPassWater();				//13	RenderPassWater				
		m_ppRenderPass[F_RenderPass_Ocean	        ] = new RenderPassOcean();			    //14	RenderPassOcean			
		m_ppRenderPass[F_RenderPass_Billboard	    ] = new RenderPassBillboard();		    //15	RenderPassBillboard
		m_ppRenderPass[F_RenderPass_Particle	    ] = new RenderPassParticle();			//16	RenderPassParticle			
		m_ppRenderPass[F_RenderPass_LensFlare	    ] = new RenderPassLensFlare();			//17	RenderPassLensFlare			
		m_ppRenderPass[F_RenderPass_VolumeFog	    ] = new RenderPassVolumeFog();			//18	RenderPassVolumeFog		
		m_ppRenderPass[F_RenderPass_CameraFog	    ] = new RenderPassCameraFog();		    //19	RenderPassCameraFog	
		m_ppRenderPass[F_RenderPass_PostProcess		] = new RenderPassPostProcess();		//20	RenderPassPostProcess	
		m_ppRenderPass[F_RenderPass_UI3D			] = new RenderPassUI3D();				//21	RenderPassUI3D	
		m_ppRenderPass[F_RenderPass_UI	            ] = new RenderPassUI();	                //22	RenderPassUI
        m_ppRenderPass[F_RenderPass_BackBuffer	    ] = new RenderPassBackBuffer();	        //23	RenderPassBackBuffer

        for (int i = 0; i < F_RenderPass_Count; i++)
		{	
            RenderPass* pRenderPass = m_ppRenderPass[i];
			if (pRenderPass)
            {
                m_mapRenderPass[pRenderPass->GetName()] = pRenderPass;
            }
		}

        return true;
    }

    RenderPass* RenderPassManager::GetRenderPassByType(FRenderPassType eRenderPass)
    {
        return m_ppRenderPass[(int)eRenderPass];
    }
    RenderPass* RenderPassManager::GetRenderPassByName(const String& strName)
    {
        RenderPassPtrMap::iterator itFind = m_mapRenderPass.find(strName);
        if (itFind == m_mapRenderPass.end())
            return nullptr;
        return itFind->second;
    }


    void RenderPassManager::SetRenderTarget(RenderTarget* pRenderTarget)
    {

    }
    void RenderPassManager::SetSceneManager(SceneManager* pSceneManager)
    {

    }
    void RenderPassManager::SetRenderTargetAndSceneManager(RenderTarget* pRenderTarget, SceneManager* pSceneManager)
    {
        
    }


    bool RenderPassManager::RegisterRenderPassCustom(RenderPass* pRenerPass)
    {
        F_Assert(pRenerPass && "RenderPassManager::RegisterRenderPassCustom")
		int nType = (int)pRenerPass->GetRenderPassType();
		if (nType < F_RenderPass_Custom1 || nType >= F_RenderPass_Count)
		{
            F_LogError("*********************** RenderPassManager::RegisterRenderPassCustom: Wrong render pass type: [%d] !", nType);
			return false;
		}
		m_ppRenderPass[nType] = pRenerPass;
        m_mapRenderPass[pRenerPass->GetName()] = pRenerPass;
		return true;
    }
	bool RenderPassManager::UnRegisterRenderPassCustom(RenderPass* pRenerPass)
    {
        F_Assert(pRenerPass && "RenderPassManager::UnRegisterRenderPassCustom");
		int nType = pRenerPass->GetRenderPassType();
		if (nType < F_RenderPass_Custom1 || nType >= F_RenderPass_Count)
		{
            F_LogError("*********************** RenderPassManager::UnRegisterRenderPassCustom: Wrong render pass type: [%d] !", nType);
			return false;
		}

		m_ppRenderPass[nType] = nullptr;
        RenderPassPtrMap::iterator itFind = m_mapRenderPass.find(pRenerPass->GetName());
        if (itFind != m_mapRenderPass.end())
            m_mapRenderPass.erase(itFind);
		return true;
    }

}; //LostPeterEngine