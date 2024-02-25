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

#ifndef _SCENE_CONFIG_H_
#define _SCENE_CONFIG_H_

#include "Base.h"

namespace LostPeterEngine
{
    //////////////////////////////// SceneConfig ////////////////////////////////////
    class engineExport SceneConfig : public Base
    {
    public:
        SceneConfig(const String& strName);
        virtual ~SceneConfig();

    public:
    protected:


    public:


    };

    //////////////////////////////// SceneConfigSetting /////////////////////////////
    class engineExport SceneConfigSetting
    {
    public:
        SceneConfigSetting();
        virtual ~SceneConfigSetting();

    public:
    protected:
        SceneConfigPtrVector m_aSceneConfig;
        SceneConfigPtrMap m_mapSceneConfig;

    public:



    };


    //////////////////////////////// SceneConfigNode ////////////////////////////////
    class engineExport SceneConfigNode : public Base
    {
    public:
        SceneConfigNode(const String& strName);
        virtual ~SceneConfigNode();

    public:
    protected:
        String m_strNameParent;

        FVector3 m_vPos;
        FVector3 m_vAngle;
        FVector3 m_vScale;

    public:
        F_FORCEINLINE const String& GetNameParent() const { return m_strNameParent; }
        F_FORCEINLINE void SetNameParent(const String& strNameParent) { m_strNameParent = strNameParent; }

        F_FORCEINLINE const FVector3& GetPos() const { return m_vPos; }
        F_FORCEINLINE void SetPos(const FVector3& vPos) { m_vPos = vPos; }
        F_FORCEINLINE const FVector3& GetAngle() const { return m_vAngle; }
        F_FORCEINLINE void SetAngle(const FVector3& vAngle) { m_vAngle = vAngle; }
        F_FORCEINLINE const FVector3& GetScale() const { return m_vScale; }
        F_FORCEINLINE void SetScale(const FVector3& vScale) { m_vScale = vScale; }

    };


    //////////////////////////////// ObjectConfig ///////////////////////////////////
    class engineExport ObjectConfig : public Base
    {
    public:
        ObjectConfig(const String& strName);
        virtual ~ObjectConfig();

    public:
    protected:
        EObjectType m_eObject;

    public:
        F_FORCEINLINE EObjectType GetObjectType() const { return m_eObject; }


    };


    //////////////////////////////// ObjectConfigMesh ///////////////////////////////
    class engineExport ObjectConfigMesh : public ObjectConfig
    {
    public:
        ObjectConfigMesh(const String& strName);
        virtual ~ObjectConfigMesh();

    public:
    protected:


    public:


    };


    //////////////////////////////// ObjectConfigSkinMesh ///////////////////////////
    class engineExport ObjectConfigSkinMesh : public ObjectConfig
    {
    public:
        ObjectConfigSkinMesh(const String& strName);
        virtual ~ObjectConfigSkinMesh();

    public:
    protected:


    public:


    };


    //////////////////////////////// ObjectConfigCamera /////////////////////////////
    class engineExport ObjectConfigCamera : public ObjectConfig
    {
    public:
        ObjectConfigCamera(const String& strName);
        virtual ~ObjectConfigCamera();

    public:
    protected:


    public:


    };


    //////////////////////////////// ObjectConfigLight //////////////////////////////
    class engineExport ObjectConfigLight : public ObjectConfig
    {
    public:
        ObjectConfigLight(const String& strName);
        virtual ~ObjectConfigLight();

    public:
    protected:


    public:


    };


    //////////////////////////////// ObjectConfigTerrain ////////////////////////////
    class engineExport ObjectConfigTerrain : public ObjectConfig
    {
    public:
        ObjectConfigTerrain(const String& strName);
        virtual ~ObjectConfigTerrain();

    public:
    protected:


    public:


    };


    //////////////////////////////// ObjectConfigWater //////////////////////////////
    class engineExport ObjectConfigWater : public ObjectConfig
    {
    public:
        ObjectConfigWater(const String& strName);
        virtual ~ObjectConfigWater();

    public:
    protected:


    public:


    };


    //////////////////////////////// ObjectConfigSky ////////////////////////////////
    class engineExport ObjectConfigSky : public ObjectConfig
    {
    public:
        ObjectConfigSky(const String& strName);
        virtual ~ObjectConfigSky();

    public:
    protected:


    public:


    };


    //////////////////////////////// ObjectConfigCloud //////////////////////////////
    class engineExport ObjectConfigCloud : public ObjectConfig
    {
    public:
        ObjectConfigCloud(const String& strName);
        virtual ~ObjectConfigCloud();

    public:
    protected:


    public:


    };


    //////////////////////////////// ObjectConfigParticle ///////////////////////////
    class engineExport ObjectConfigParticle : public ObjectConfig
    {
    public:
        ObjectConfigParticle(const String& strName);
        virtual ~ObjectConfigParticle();

    public:
    protected:


    public:


    };


    //////////////////////////////// ObjectConfigPostProcess ////////////////////////
    class engineExport ObjectConfigPostProcess : public ObjectConfig
    {
    public:
        ObjectConfigPostProcess(const String& strName);
        virtual ~ObjectConfigPostProcess();

    public:
    protected:


    public:


    };


    //////////////////////////////// ObjectConfigUI /////////////////////////////////
    class engineExport ObjectConfigUI : public ObjectConfig
    {
    public:
        ObjectConfigUI(const String& strName);
        virtual ~ObjectConfigUI();

    public:
    protected:


    public:


    };


    //////////////////////////////// ObjectConfigCustom /////////////////////////////
    class engineExport ObjectConfigCustom : public ObjectConfig
    {
    public:
        ObjectConfigCustom(const String& strName);
        virtual ~ObjectConfigCustom();

    public:
    protected:


    public:


    };


}; //LostPeterEngine

#endif