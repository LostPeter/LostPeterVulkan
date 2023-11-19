/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-18
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/EngineConfig.h"
#include "../include/Engine.h"
#include "../include/RenderEngine.h"

namespace LostPeter
{
#define ENGINE_CFG_TAG_ENGINE								            "cfg_engine"
    #define	ENGINE_CFG_TAG_COMMON									        "cfg_common"
    #define	ENGINE_CFG_TAG_RENDER									        "cfg_render"
    #define	ENGINE_CFG_TAG_AUDIO									        "cfg_audio"
    #define	ENGINE_CFG_TAG_PHYSICS									        "cfg_physics"

#define ENGINE_TAG_ATTRIBUTE_NAME		                "name"
#define ENGINE_TAG_ATTRIBUTE_VALUE		                "val"


    EngineConfig::EngineConfig()
    {

    }
    
    EngineConfig::~EngineConfig()
    {

    }


        static bool	s_parserXML_EngineCommon(FXMLElement* pElementCommon, Engine* pEngine)
        {
            F_Assert(pElementCommon && pEngine && "s_parserXML_EngineCommon")

            ConfigItemMap* pCfgItems = pEngine->GetEngineCommonCfgItems();
            FXMLAttribute* pAttr = nullptr;
            int count_item = pElementCommon->GetElementChildrenCount();
            for (int i = 0; i < count_item; i++)
            {
                FXMLElement* pElement = pElementCommon->GetElementChild(i);

                String strKey(pElement->GetName());
                ConfigItemMap::iterator itFind = pCfgItems->find(strKey);
                if (itFind == pCfgItems->end())
                {
                    F_LogError("*********************** s_parserXML_EngineCommon: Config item: [%s] is not a valid engine common config item !", strKey.c_str());
                    return false;
                }

                String nameValue;
                if (!pElement->ParserAttribute_String(ENGINE_TAG_ATTRIBUTE_VALUE, nameValue))
                {
                    F_LogError("*********************** s_parserXML_EngineCommon: Can not find attribute: 'val', from engine common config !");
                    return false;
                }
                itFind->second.strCurValue = nameValue;
            }   

            return true;
        }

        static bool	s_parserXML_EngineRender(FXMLElement* pElementRender, Engine* pEngine)
        {
            F_Assert(pElementRender && pEngine && "s_parserXML_EngineRender")

            FXMLAttribute* pAttr = nullptr;
            int count_item = pElementRender->GetElementChildrenCount();
            for (int i = 0; i < count_item; i++)
            {
                FXMLElement* pElement = pElementRender->GetElementChild(i);


            }

            return true;
        }

        static bool	s_parserXML_EngineAudio(FXMLElement* pElementAudio, Engine* pEngine)
        {
            F_Assert(pElementAudio && pEngine && "s_parserXML_EngineAudio")

            FXMLAttribute* pAttr = nullptr;
            int count_item = pElementAudio->GetElementChildrenCount();
            for (int i = 0; i < count_item; i++)
            {
                FXMLElement* pElement = pElementAudio->GetElementChild(i);

                
            }

            return true;
        }

        static bool	s_parserXML_EnginePhysics(FXMLElement* pElementPhysics, Engine* pEngine)
        {
            F_Assert(pElementPhysics && pEngine && "s_parserXML_EnginePhysics")

            FXMLAttribute* pAttr = nullptr;
            int count_item = pElementPhysics->GetElementChildrenCount();
            for (int i = 0; i < count_item; i++)
            {
                FXMLElement* pElement = pElementPhysics->GetElementChild(i);

                
            }

            return true;
        }


    bool EngineConfig::Load(const String& strCfgPath, Engine* pEngine)
	{
		if(strCfgPath.empty() || !pEngine)
			return false;

        FFileXML xml;
		if (!xml.LoadXMLIndirect(strCfgPath.c_str()))
        {
            F_LogError("*********************** EngineConfig::Load: Load engine config file: [%s] failed !", strCfgPath.c_str());
			return false;
        }

        FXMLDocument* pXMLDocument = xml.GetXMLDocument();
        FXMLElement* pRoot = pXMLDocument->GetElementRoot();
        int count_item = pRoot->GetElementChildrenCount();
		for (int i = 0; i < count_item; i++)
        {
            FXMLElement* pElementItem = pRoot->GetElementChild(i);
            

            String strCfgType(pElementItem->GetName());
            //cfg_common
			if (strCfgType == ENGINE_CFG_TAG_COMMON)
			{
                if (!s_parserXML_EngineCommon(pElementItem, pEngine))
                {
                    F_LogError("*********************** EngineConfig::Load: Load engine common config failed, file: [%s] !", strCfgPath.c_str());
                    return false;
                }
			}
			//cfg_render
			if (strCfgType == ENGINE_CFG_TAG_RENDER)
			{
                if (!s_parserXML_EngineRender(pElementItem, pEngine))
                {
                    F_LogError("*********************** EngineConfig::Load: Load engine render config failed, file: [%s] !", strCfgPath.c_str());
                    return false;
                }
			}
			//cfg_audio
			if (strCfgType == ENGINE_CFG_TAG_AUDIO)
			{
                if (!s_parserXML_EngineAudio(pElementItem, pEngine))
                {
                    F_LogError("*********************** EngineConfig::Load: Load engine audio config failed, file: [%s] !", strCfgPath.c_str());
                    return false;
                }
			}
			//cfg_physics
			if (strCfgType == ENGINE_CFG_TAG_PHYSICS)
			{
                if (!s_parserXML_EnginePhysics(pElementItem, pEngine))
                {
                    F_LogError("*********************** EngineConfig::Load: Load engine physics config failed, file: [%s] !", strCfgPath.c_str());
                    return false;
                }
			}
        }

		return true;
	}

	bool EngineConfig::Save(const String& strCfgPath, Engine* pEngine)
	{
		if (strCfgPath.empty() || !pEngine)
			return false;

		FFileXML xml;
        xml.CreateXML();
		FXMLDocument* pXMLDocument = xml.GetXMLDocument();
        FXMLElement* pRoot = pXMLDocument->CreateElementRoot(ENGINE_CFG_TAG_ENGINE);
		{
			//1, cfg_common
			{
				FXMLElement* pElement_Common = new FXMLElement(ENGINE_CFG_TAG_COMMON);
				pRoot->AddElementChild(pElement_Common);
			}
			//2, cfg_render
			{
				FXMLElement* pElement_Renderer = new FXMLElement(ENGINE_CFG_TAG_RENDER);
				pRoot->AddElementChild(pElement_Renderer);
			}
			//3, cfg_audio
			{
				FXMLElement* pElement_Audio = new FXMLElement(ENGINE_CFG_TAG_AUDIO);
				pRoot->AddElementChild(pElement_Audio);
			}
			//4, cfg_physics
			{
				FXMLElement* pElement_Physics = new FXMLElement(ENGINE_CFG_TAG_PHYSICS);
				pRoot->AddElementChild(pElement_Physics);
			}
		}

		if (!xml.SaveXML(strCfgPath.c_str()))
		{
			F_LogError("*********************** EngineConfig::Save: Save engine config file: [%s] failed !", strCfgPath.c_str());
			return false;
		}

		return true;
	}
    
}; //LostPeter