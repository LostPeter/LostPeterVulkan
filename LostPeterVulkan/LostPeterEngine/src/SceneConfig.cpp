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
        
    }


    //////////////////////////////// ObjectConfig ///////////////////////////////////
    ObjectConfig::ObjectConfig(const String& strName)
        : Base(strName)
    {

    }
    ObjectConfig::~ObjectConfig()
    {
        
    }


    //////////////////////////////// ObjectConfigMesh ///////////////////////////////
    ObjectConfigMesh::ObjectConfigMesh(const String& strName)
        : ObjectConfig(strName)
    {

    }
    ObjectConfigMesh::~ObjectConfigMesh()
    {
        
    }


    //////////////////////////////// ObjectConfigSkinMesh ///////////////////////////
    ObjectConfigSkinMesh::ObjectConfigSkinMesh(const String& strName)
        : ObjectConfig(strName)
    {

    }
    ObjectConfigSkinMesh::~ObjectConfigSkinMesh()
    {
        
    }


    //////////////////////////////// ObjectConfigCamera /////////////////////////////
    ObjectConfigCamera::ObjectConfigCamera(const String& strName)
        : ObjectConfig(strName)
    {

    }
    ObjectConfigCamera::~ObjectConfigCamera()
    {
        
    }


    //////////////////////////////// ObjectConfigLight //////////////////////////////
    ObjectConfigLight::ObjectConfigLight(const String& strName)
        : ObjectConfig(strName)
    {

    }
    ObjectConfigLight::~ObjectConfigLight()
    {
        
    }


    //////////////////////////////// ObjectConfigTerrain ////////////////////////////
    ObjectConfigTerrain::ObjectConfigTerrain(const String& strName)
        : ObjectConfig(strName)
    {

    }
    ObjectConfigTerrain::~ObjectConfigTerrain()
    {
        
    }


    //////////////////////////////// ObjectConfigWater //////////////////////////////
    ObjectConfigWater::ObjectConfigWater(const String& strName)
        : ObjectConfig(strName)
    {

    }
    ObjectConfigWater::~ObjectConfigWater()
    {
        
    }


    //////////////////////////////// ObjectConfigSky ////////////////////////////////
    ObjectConfigSky::ObjectConfigSky(const String& strName)
        : ObjectConfig(strName)
    {

    }
    ObjectConfigSky::~ObjectConfigSky()
    {
        
    }


    //////////////////////////////// ObjectConfigCloud //////////////////////////////
    ObjectConfigCloud::ObjectConfigCloud(const String& strName)
        : ObjectConfig(strName)
    {

    }
    ObjectConfigCloud::~ObjectConfigCloud()
    {
        
    }


    //////////////////////////////// ObjectConfigParticle ///////////////////////////
    ObjectConfigParticle::ObjectConfigParticle(const String& strName)
        : ObjectConfig(strName)
    {

    }
    ObjectConfigParticle::~ObjectConfigParticle()
    {
        
    }


    //////////////////////////////// ObjectConfigPostProcess ////////////////////////
    ObjectConfigPostProcess::ObjectConfigPostProcess(const String& strName)
        : ObjectConfig(strName)
    {

    }
    ObjectConfigPostProcess::~ObjectConfigPostProcess()
    {
        
    }


    //////////////////////////////// ObjectConfigUI /////////////////////////////////
    ObjectConfigUI::ObjectConfigUI(const String& strName)
        : ObjectConfig(strName)
    {

    }
    ObjectConfigUI::~ObjectConfigUI()
    {
        
    }


    //////////////////////////////// ObjectConfigCustom /////////////////////////////
	ObjectConfigCustom::ObjectConfigCustom(const String& strName)
        : ObjectConfig(strName)
    {

    }
    ObjectConfigCustom::~ObjectConfigCustom()
    {
        
    }

}; //LostPeterEngine