/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/FPathManager.h"
#include "../include/FUtil.h"
#include "../include/FUtilString.h"
#include "../include/FFileXML.h"

namespace LostPeterFoundation
{
#define PATH_GAP_DEFAULT	1000

	#define PATH_TAG_PATH_CFG								"cfg_path"
		#define	PATH_TAG_PATH									"path"

#define PATH_TAG_ATTRIBUTE_GROUP		"group"
#define PATH_TAG_ATTRIBUTE_MAX			"max"
#define PATH_TAG_ATTRIBUTE_PATH			"path"
#define PATH_TAG_ATTRIBUTE_RECURSIVE	"recursive"



	template<> FPathManager* FSingleton<FPathManager>::ms_Singleton = nullptr;
	FPathManager* FPathManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}

	FPathManager& FPathManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "FPathManager::GetSingleton")
		return (*ms_Singleton);  
	}
	
	const String FPathManager::ms_strPathCfgPath = "Assets/Config/Cfg_Path.xml";
	FPathManager::FPathManager()
		: m_strWorkFolder("")
	{
		
	}

	FPathManager::~FPathManager()
	{
		Release();
	}

	void FPathManager::Release()
	{
		m_mapGroup.clear();
	}

	bool FPathManager::Init()
	{
		//1> Get working folder 
		m_strWorkFolder = FUtil::GetPathBin();
		if (m_strWorkFolder.size() <= 0)
		{
			F_LogError("FPathManager::Init: Get working folder failed !");
			F_Assert(false && "FPathManager::Init: Get working folder failed !")
			return false;	
		}

		//2> Set Default group id map
		{
			setDefaultGroupID();
		}
		
		//3> Load path config
		{
			if (!LoadCfg())
			{
				F_LogError("FPathManager::Init: Load path cfg file failed !");
				F_Assert(false && "FPathManager::Init: Load path cfg file failed !")
				return false;
			}
		}

		return true;
	}

	String FPathManager::GetFilePath(uint32 nGroup, const String& strName)
	{
		FPathGroupMap::iterator itFind = m_mapGroup.find(nGroup);
		if (itFind == m_mapGroup.end())
		{
			F_LogError("FPathManager::GetFilePath: Can not find path group: [%u] !", nGroup);
			F_Assert(false && "FPathManager::GetFilePath: Can not find path group!")
			return FUtilString::BLANK;
		}
		String2StringMap& mapN2P = itFind->second;
		String2StringMap::iterator itN2P = mapN2P.find(strName);
		if (itN2P == mapN2P.end())
		{
			F_LogError("FPathManager::GetFilePath: Can not find file: [%s], in path group: [%u] !", strName.c_str(), nGroup);
			return FUtilString::BLANK;
		}

		return itN2P->second;
	}

	bool FPathManager::GetGroupMaxID(uint32 nGroup, uint32& nMaxID)
	{
		FPathGroupIDMap::iterator itFind = m_mapGroupID.find(nGroup);
		if (itFind == m_mapGroupID.end())
			return false;

		nMaxID = itFind->second;
		F_Assert(nMaxID >= nGroup && "FPathManager::GetGroupMaxID")
		return true;
	}

	bool FPathManager::GetGroupBasePath(uint32 nGroup,String& strBaseName)
	{
		FPathGroupBaseMap::iterator itFind = m_mapGroupBase.find(nGroup);
		if (itFind == m_mapGroupBase.end())
			return false;

		strBaseName = itFind->second;
		return true;
	}

	bool FPathManager::GetGroupInfo(uint32 nGroup, uint32& nMaxID, String& strBaseName)
	{
		FPathGroupIDMap::iterator itFindID = m_mapGroupID.find(nGroup);
		if (itFindID == m_mapGroupID.end())
			return false;
		FPathGroupBaseMap::iterator itFind = m_mapGroupBase.find(nGroup);
		if (itFind == m_mapGroupBase.end())
			return false;

		nMaxID = itFindID->second;
		F_Assert(nMaxID >= nGroup && "FPathManager::GetGroupInfo")
		strBaseName = itFind->second;
		return true;
	}

	bool FPathManager::RegisterUserGroup(uint32 nBaseGroup, uint32 nGroup, const String& strPath, bool bIsRecursive)
	{
		FPathGroupIDMap::iterator itFindID = m_mapGroupID.find(nBaseGroup);
		if (itFindID == m_mapGroupID.end())
		{
			F_LogError("FPathManager::RegisterUserGroup: Path group: [%u] is not a valid base group !", nBaseGroup);
			F_Assert(false && "FPathManager::RegisterUserGroup: Path group is not a valid base group !")
			return false;
		}
		uint32 nMaxID = itFindID->second;
		if (nGroup <= nBaseGroup || nGroup < nMaxID || nGroup >= nBaseGroup + PATH_GAP_DEFAULT)
		{
			F_LogError("FPathManager::RegisterUserGroup: Path group: [%u] is not a valid group value !", nGroup);
			F_Assert(false && "FPathManager::RegisterUserGroup: Path group is not a valid group value !")
			return false;
		}
		FPathGroupMap::iterator itFind = m_mapGroup.find(nGroup);
		if (itFind != m_mapGroup.end())
		{
			F_LogError("FPathManager::RegisterUserGroup, Path group: [%u] already exist !", nGroup);
			F_Assert(false && "FPathManager::RegisterUserGroup, Path group already exist !")
			return false;
		}
		
		String2StringMap mapN2P;
		if (!FUtil::EnumFiles(strPath, mapN2P, bIsRecursive))
		{
			F_LogError("FPathManager::RegisterUserGroup, Enum file in path group: [%u] failed !", nGroup);
			F_Assert(false && "FPathManager::RegisterUserGroup, Enum file in path group failed !")
			return false;
		}

		m_mapGroup[nGroup] = mapN2P;
		m_mapGroupBase[nGroup] = strPath;
		itFindID->second = nGroup;
		return true;
	}

	bool FPathManager::registerGroup(uint32 nGroup, const String& strPath, bool bIsRecursive)
	{
		FPathGroupMap::iterator itFind = m_mapGroup.find(nGroup);
		if (itFind != m_mapGroup.end())
		{
			F_LogError("FPathManager::registerGroup, Path group: [%u] already exist !", nGroup);
			F_Assert(false && "FPathManager::registerGroup, Path group already exist !")
			return false;
		}
		
		String2StringMap mapN2P;
		if (!FUtil::EnumFiles(strPath, mapN2P, bIsRecursive))
		{
			F_LogError("FPathManager::registerGroup, Enum file in path group: [%u] failed !", nGroup);
			F_Assert(false && "FPathManager::registerGroup, Enum file in path group failed !")
			return false;
		}
		
		m_mapGroup[nGroup] = mapN2P;
		m_mapGroupBase[nGroup] = strPath;
		return true;
	}

	bool FPathManager::ReloadGroup(uint32 nGroup, bool bIsRecursive)
	{
		FPathGroupMap::iterator itFind = m_mapGroup.find(nGroup);
		FPathGroupBaseMap::iterator itFindBase = m_mapGroupBase.find(nGroup);
		if (itFind == m_mapGroup.end() || itFindBase == m_mapGroupBase.end())
		{
			F_LogError("FPathManager::ReloadGroup, Path group: [%u] is not exist !", nGroup);
			F_Assert(false && "FPathManager::ReloadGroup, Path group is not exist !")
			return false;
		}

		String2StringMap mapN2P;
		if (!FUtil::EnumFiles(itFindBase->second, mapN2P, bIsRecursive))
		{
			F_LogError("FPathManager::registerGroup, Enum file in path group: [%u] failed !", nGroup);
			F_Assert(false && "FPathManager::registerGroup, Enum file in path group failed !")
			return false;
		}
		
		itFind->second.swap(mapN2P);
		return true;
	}

	bool FPathManager::LoadCfg()
	{
		Release();

		String strPathCfg(ms_strPathCfgPath);
		FFileXML xmlCfg;
		if (!xmlCfg.LoadXMLIndirect(strPathCfg))
		{
			F_LogError("FPathManager::LoadCfg, Load path cfg file: [%s] failed !", strPathCfg.c_str());
			F_Assert(false && "FPathManager::LoadCfg, Load 'media/config/path_cfg.xml' file failed !")
			return false;
		}
		FXMLDocument* pDoc = xmlCfg.GetXMLDocument();
		FXMLElement* pRoot = pDoc->GetElementRoot();

		int count_child = pRoot->GetElementChildrenCount();
		for (int i = 0; i < count_child; i++)
		{
			FXMLElement* pChild = pRoot->GetElementChild(i);

			uint32 nGroup = 0;
			if (pChild->ParserAttribute_UInt(PATH_TAG_ATTRIBUTE_GROUP, nGroup))
			{
				FPathGroupIDMap::iterator itFind = m_mapGroupID.find(nGroup);
				if (itFind == m_mapGroupID.end())
				{
					F_LogError("FPathManager::LoadCfg: Can not find group: [%u] in group map !", nGroup);
					F_Assert(false && "FPathManager::LoadCfg: Can not find group in group map !")
					continue;
				}

				uint32 nMax = 0;
				if (pChild->ParserAttribute_UInt(PATH_TAG_ATTRIBUTE_MAX, nMax))
				{
					if(nMax > nGroup)
						m_mapGroupID[nGroup] = nMax;
				}
				
				String strPath;
				if (!pChild->ParserAttribute_String(PATH_TAG_ATTRIBUTE_PATH, strPath))
				{
					F_LogError("FPathManager::LoadCfg: Can not find path from group: [%u] !", nGroup);
					F_Assert(false && "FPathManager::LoadCfg: Can not find path from group !")
					continue;
				}

				bool bIsRecursive = false;
				if(pChild->ParserAttribute_Bool(PATH_TAG_ATTRIBUTE_RECURSIVE, bIsRecursive))
				{
					bIsRecursive = true;
				}
				m_mapGroupRecursive[(uint32)nGroup] = bIsRecursive;
				
				if (!registerGroup((uint32)nGroup,strPath,bIsRecursive))
				{
					return false;
				}

				F_LogInfo("FPathManager::LoadCfg, Register path group: [%u], path: [%s], recursive: [%s] success!", nGroup, strPath.c_str(), FUtilString::SaveBool(bIsRecursive).c_str());
			}
		}

		return true;
	}

	bool FPathManager::SaveCfg()
	{
		String strPathCfg(ms_strPathCfgPath);
		FFileXML xmlCfg;
		xmlCfg.CreateXML();

		FXMLDocument* pDoc = xmlCfg.GetXMLDocument();
		F_Assert(pDoc && "FPathManager::SaveCfg")
		pDoc->CreateElementDeclaration();
		FXMLElement* pRoot = pDoc->CreateElementRoot(PATH_TAG_PATH_CFG);

		FPathGroupMap::iterator it, itEnd;
		itEnd = m_mapGroup.end();
		for (it = m_mapGroup.begin(); it != itEnd; ++it)
		{
			uint32 nGroup = it->first;
			FXMLElement* pChild = pRoot->AddElementChild(new FXMLElement(PATH_TAG_PATH));
			pChild->SaveAttribute_UInt(PATH_TAG_ATTRIBUTE_GROUP, nGroup);
			FPathGroupIDMap::iterator itFindID = m_mapGroupID.find(nGroup);
			if (itFindID != m_mapGroupID.end())
			{
				uint nMax = itFindID->second;
				pChild->SaveAttribute_UInt(PATH_TAG_ATTRIBUTE_MAX, nMax);
			}
			FPathGroupBaseMap::iterator itFindBase = m_mapGroupBase.find(nGroup);
			F_Assert(itFindBase != m_mapGroupBase.end() && "FPathManager::SaveCfg")
			pChild->SaveAttribute_String(PATH_TAG_ATTRIBUTE_PATH, itFindBase->second);
			FPathGroupRecursiveMap::iterator itFindRecursive = m_mapGroupRecursive.find(nGroup);
			if (itFindRecursive != m_mapGroupRecursive.end() && itFindRecursive->second)
			{
				bool isRecursive = true;
				pChild->SaveAttribute_Bool(PATH_TAG_ATTRIBUTE_RECURSIVE, isRecursive);
			}
		}

		return xmlCfg.SaveXMLIndirect(strPathCfg);
	}
	
	void FPathManager::setDefaultGroupID()
	{	
	////Config
		m_mapGroupID[PathGroup_Config]				= PathGroup_Config;
	////Data
		m_mapGroupID[PathGroup_Data]				= PathGroup_Data;
	////Editor
		m_mapGroupID[PathGroup_Editor]				= PathGroup_Editor;
	////Font
		m_mapGroupID[PathGroup_Font]				= PathGroup_Font;
	////Shader
		m_mapGroupID[PathGroup_Shader] 				= PathGroup_Shader;
	////Mesh
		m_mapGroupID[PathGroup_Mesh]				= PathGroup_Mesh;
		m_mapGroupID[PathGroup_MeshCommon]			= PathGroup_MeshCommon;
		m_mapGroupID[PathGroup_MeshModel]			= PathGroup_MeshModel;
		m_mapGroupID[PathGroup_MeshCharacter]		= PathGroup_MeshCharacter;
	////Texture
		m_mapGroupID[PathGroup_Texture]				= PathGroup_Texture;
		m_mapGroupID[PathGroup_TextureCommon]		= PathGroup_TextureCommon;
		m_mapGroupID[PathGroup_TextureModel]		= PathGroup_TextureModel;
		m_mapGroupID[PathGroup_TextureCharacter]	= PathGroup_TextureCharacter;
		m_mapGroupID[PathGroup_TextureTerrain]		= PathGroup_TextureTerrain;
		m_mapGroupID[PathGroup_TextureGfx]			= PathGroup_TextureGfx;
		m_mapGroupID[PathGroup_TextureSky]			= PathGroup_TextureSky;
	////Material
		m_mapGroupID[PathGroup_Material]			= PathGroup_Material;
		m_mapGroupID[PathGroup_MaterialCommon]		= PathGroup_MaterialCommon;
		m_mapGroupID[PathGroup_MaterialModel]		= PathGroup_MaterialModel;
		m_mapGroupID[PathGroup_MaterialCharacter]	= PathGroup_MaterialCharacter;
	////Audio
		m_mapGroupID[PathGroup_Audio]				= PathGroup_Audio;
		m_mapGroupID[PathGroup_AudioCommon]			= PathGroup_AudioCommon;
		m_mapGroupID[PathGroup_AudioModel]			= PathGroup_AudioModel;
		m_mapGroupID[PathGroup_AudioCharacter]		= PathGroup_AudioCharacter;
	////Gfx
		m_mapGroupID[PathGroup_Gfx]					= PathGroup_Gfx;
		m_mapGroupID[PathGroup_GfxCommon]			= PathGroup_GfxCommon;
		m_mapGroupID[PathGroup_GfxModel]			= PathGroup_GfxModel;
		m_mapGroupID[PathGroup_GfxCharacter]		= PathGroup_GfxCharacter;
	////Terrain					
		m_mapGroupID[PathGroup_Terrain]				= PathGroup_Terrain;
	////UI
		m_mapGroupID[PathGroup_UI]					= PathGroup_UI;
	////PostEffect
		m_mapGroupID[PathGroup_PostEffect]			= PathGroup_PostEffect;
	////Script	
		m_mapGroupID[PathGroup_Script] 				= PathGroup_Script;
	////Scene	
		m_mapGroupID[PathGroup_Scene] 				= PathGroup_Scene;
	}
    
}; //LostPeterFoundation