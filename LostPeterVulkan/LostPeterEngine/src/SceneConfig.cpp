/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-24
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/SceneConfig.h"
#include "../include/SceneDataManager.h"
#include "../include/SceneManager.h"
#include "../include/SceneNode.h"
#include "../include/ObjectMesh.h"
#include "../include/ObjectSkinMesh.h"
#include "../include/ObjectCamera.h"
#include "../include/ObjectLight.h"
#include "../include/ObjectTerrain.h"
#include "../include/ObjectWater.h"
#include "../include/ObjectSky.h"
#include "../include/ObjectCloud.h"
#include "../include/ObjectParticle.h"

namespace LostPeterEngine
{
    //////////////////////////////// SceneConfig ////////////////////////////////////
    SceneConfig::SceneConfig(const String& strName)
        : Base(strName)
    {

    }
    SceneConfig::~SceneConfig()
    {

    }

    //////////////////////////////// SceneConfigSetting /////////////////////////////
    SceneConfigSetting::SceneConfigSetting()
    {

    }
    SceneConfigSetting::~SceneConfigSetting()
    {

    }


    //////////////////////////////// SceneConfigNode ////////////////////////////////
    SceneConfigNode::SceneConfigNode(const String& strName)
        : Base(strName)
        , m_strNameParent("")
        , m_vPos(FMath::ms_v3Zero)
        , m_vAngle(FMath::ms_v3Zero)
        , m_vScale(FMath::ms_v3One)
    {

    }
    SceneConfigNode::~SceneConfigNode()
    {
        DeleteSceneConfigNodeChildAll();
    }
    void SceneConfigNode::AddSceneConfigNodeChild(SceneConfigNode* pChild)
    {
        m_aChild.push_back(pChild);
    }
    void SceneConfigNode::DeleteSceneConfigNodeChild(SceneConfigNode* pChild)
    {
        SceneConfigNodePtrVector::iterator itFind = std::find(m_aChild.begin(), m_aChild.end(), pChild);
        if (itFind != m_aChild.end())
            m_aChild.erase(itFind);
        F_DELETE(pChild)
    }
    void SceneConfigNode::DeleteSceneConfigNodeChildAll()
    {
        for (SceneConfigNodePtrVector::iterator it = m_aChild.begin();
             it != m_aChild.end(); ++it)
        {
            F_DELETE((*it))
        }
        m_aChild.clear();
    }


    //////////////////////////////// ObjectConfig ///////////////////////////////////
    ObjectConfig::ObjectConfig(const String& strName)
        : Base(strName)
        , m_nID(0)
        , m_strNameSceneNode("")
    {

    }
    ObjectConfig::~ObjectConfig()
    {
        
    }


    //////////////////////////////// ObjectConfigMesh ///////////////////////////////
    ObjectConfigMesh::ObjectConfigMesh(const String& strName)
        : ObjectConfig(strName)
    {
        m_eObject = E_Object_Mesh;
    }
    ObjectConfigMesh::~ObjectConfigMesh()
    {
        
    }


    //////////////////////////////// ObjectConfigSkinMesh ///////////////////////////
    ObjectConfigSkinMesh::ObjectConfigSkinMesh(const String& strName)
        : ObjectConfig(strName)
    {
        m_eObject = E_Object_SkinMesh;
    }
    ObjectConfigSkinMesh::~ObjectConfigSkinMesh()
    {
        
    }


    //////////////////////////////// ObjectConfigCamera /////////////////////////////
    ObjectConfigCamera::ObjectConfigCamera(const String& strName)
        : ObjectConfig(strName)
    {
        m_eObject = E_Object_Camera;
    }
    ObjectConfigCamera::~ObjectConfigCamera()
    {
        
    }


    //////////////////////////////// ObjectConfigLight //////////////////////////////
    ObjectConfigLight::ObjectConfigLight(const String& strName)
        : ObjectConfig(strName)
    {
        m_eObject = E_Object_Light;
    }
    ObjectConfigLight::~ObjectConfigLight()
    {
        m_eObject = E_Object_Mesh;
    }


    //////////////////////////////// ObjectConfigTerrain ////////////////////////////
    ObjectConfigTerrain::ObjectConfigTerrain(const String& strName)
        : ObjectConfig(strName)
    {
        m_eObject = E_Object_Terrain;
    }
    ObjectConfigTerrain::~ObjectConfigTerrain()
    {
        
    }


    //////////////////////////////// ObjectConfigWater //////////////////////////////
    ObjectConfigWater::ObjectConfigWater(const String& strName)
        : ObjectConfig(strName)
    {
        m_eObject = E_Object_Water;
    }
    ObjectConfigWater::~ObjectConfigWater()
    {
        
    }


    //////////////////////////////// ObjectConfigSky ////////////////////////////////
    ObjectConfigSky::ObjectConfigSky(const String& strName)
        : ObjectConfig(strName)
    {
        m_eObject = E_Object_Sky;
    }
    ObjectConfigSky::~ObjectConfigSky()
    {
        
    }


    //////////////////////////////// ObjectConfigCloud //////////////////////////////
    ObjectConfigCloud::ObjectConfigCloud(const String& strName)
        : ObjectConfig(strName)
    {
        m_eObject = E_Object_Cloud;
    }
    ObjectConfigCloud::~ObjectConfigCloud()
    {
        
    }


    //////////////////////////////// ObjectConfigParticle ///////////////////////////
    ObjectConfigParticle::ObjectConfigParticle(const String& strName)
        : ObjectConfig(strName)
    {
        m_eObject = E_Object_Particle;
    }   
    ObjectConfigParticle::~ObjectConfigParticle()
    {
        
    }


    //////////////////////////////// ObjectConfigPostProcess ////////////////////////
    ObjectConfigPostProcess::ObjectConfigPostProcess(const String& strName)
        : ObjectConfig(strName)
    {
        m_eObject = E_Object_PostProcess;
    }
    ObjectConfigPostProcess::~ObjectConfigPostProcess()
    {
        
    }


    //////////////////////////////// ObjectConfigUI /////////////////////////////////
    ObjectConfigUI::ObjectConfigUI(const String& strName)
        : ObjectConfig(strName)
    {
        m_eObject = E_Object_UI;
    }
    ObjectConfigUI::~ObjectConfigUI()
    {
        
    }


    //////////////////////////////// ObjectConfigCustom /////////////////////////////
	ObjectConfigCustom::ObjectConfigCustom(const String& strName)
        : ObjectConfig(strName)
    {
        m_eObject = E_Object_Custom;
    }
    ObjectConfigCustom::~ObjectConfigCustom()
    {
        
    }

}; //LostPeterEngine