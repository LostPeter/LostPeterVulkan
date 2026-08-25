/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2026-07-26
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/TerrainSetting.h"
#include "../include/VulkanWindow.h"
#include "../include/TerrainUtil.h"

template<> LostPeterVulkan::TerrainSetting* LostPeterFoundation::FSingleton<LostPeterVulkan::TerrainSetting>::ms_Singleton = nullptr;

namespace LostPeterVulkan
{
	/////////////////////////// TerrainChunkedSetting ////////////////////
	TerrainChunkedSetting::TerrainChunkedSetting()
		: nX(0)
		, nZ(0)
		, nID(-1)
		, nameHeightMap("")
		, pathHeightMap("")
	{

	}
	TerrainChunkedSetting::~TerrainChunkedSetting()
	{

	}


	/////////////////////////// TerrainSetting ///////////////////////////
	TerrainSetting* TerrainSetting::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	TerrainSetting& TerrainSetting::GetSingleton()
	{  
		F_Assert(ms_Singleton && "TerrainSetting::GetSingleton")
		return (*ms_Singleton);     
	}

	#define TERRAIN_FILE_XML_EXT                	".terrain"

	#define TERRAIN_TAG_SETTING                 	"setting"
		#define TERRAIN_TAG_TERRAIN             		"terrain"
			#define TERRAIN_TAG_PARAM               		"param"
			#define TERRAIN_TAG_CHUNKS               		"chunks"
				#define TERRAIN_TAG_CHUNK               		"chunk"

	#define	TERRAIN_TAG_ATTRIBUTE_TYPE	        	"type"
	#define	TERRAIN_TAG_ATTRIBUTE_NAME	        	"name"
	#define	TERRAIN_TAG_ATTRIBUTE_LEAF_QUADS	    "leaf_quads"
	#define	TERRAIN_TAG_ATTRIBUTE_PATCH_QUADS	    "patch_quads"
	#define	TERRAIN_TAG_ATTRIBUTE_START_X	        "start_x"
	#define	TERRAIN_TAG_ATTRIBUTE_START_Z	        "start_z"
	#define	TERRAIN_TAG_ATTRIBUTE_COUNT_X	        "count_x"
	#define	TERRAIN_TAG_ATTRIBUTE_COUNT_Z	        "count_z"
	#define	TERRAIN_TAG_ATTRIBUTE_SIZE_X	       	"size_x"
	#define	TERRAIN_TAG_ATTRIBUTE_SIZE_Z	        "size_z"
	#define	TERRAIN_TAG_ATTRIBUTE_RESOLUTION	    "resolution"
	#define	TERRAIN_TAG_ATTRIBUTE_CELL_SIZE	       	"cell_size"
	#define	TERRAIN_TAG_ATTRIBUTE_X	        		"x"
	#define	TERRAIN_TAG_ATTRIBUTE_Z	        		"z"
	#define	TERRAIN_TAG_ATTRIBUTE_HEIGHT_MAP	    "height_map"


	TerrainSetting::TerrainSetting()
		: Base("TerrainSetting")

		, nameSetting("")
		, pathSetting("")

		, nLeafQuads(16)
		, nPatchQuads(16)

		, fLodPixelError(2.5f)
		, bHeadless(false)
		, nAutoCloseMs(0)

		, nStartX(0)
		, nStartZ(0)
		, nCountX(0)
		, nCountZ(0)
		, fSizeX(0.0f)
		, fSizeZ(0.0f)	
		, nResolution(0)
		, fCellSize(1.0f)

		, bIsInit(false)
		
	{
		
	}
    TerrainSetting::~TerrainSetting()
	{
		Destroy();
	}

	void TerrainSetting::Destroy()
	{
		destroySettings();
	}
		void TerrainSetting::destroySettings()
		{
			for (TerrainChunkedSettingPtrVector::iterator it = this->aChunkedSettings.begin();
				 it != this->aChunkedSettings.end(); ++it) 
			{
				TerrainChunkedSetting* pChunkedSetting = *it;
				F_DELETE(pChunkedSetting)
			}
			this->aChunkedSettings.clear();
			this->mapChunkedSettings.clear();
		}

    bool TerrainSetting::Init(const String& path)
	{
		if (this->pathSetting == path && IsInit())
			return true;
		this->pathSetting = path;

		destroySettings();
		bool bRet = LoadSetting(path);
		if (!bRet)
		{
			F_LogError("*********************** TerrainSetting::Init: failed, path: [%s] !", path.c_str());
			return false;
		}
		F_LogInfo("TerrainSetting::Init: success, path setting: [%s] !", path.c_str());

		SetIsInit(true);
		return bRet;
	}
		
	bool TerrainSetting::LoadSetting(const String& path)
	{
		FFileXML terrainSetting;
		if (!terrainSetting.LoadXML(path))
		{
			F_LogError("*********************** TerrainSetting::LoadSetting, Load terrain setting path file: [%s] failed !", path.c_str());
			return false;
		}
		FXMLDocument* pDoc = terrainSetting.GetXMLDocument();
		//Root
		FXMLElement* pRoot = pDoc->GetElementRoot();
		F_Assert(pRoot != nullptr && "TerrainSetting::LoadSetting")
		
		//Terrain
		FXMLElement* pTerrain = pRoot->FindElementChild(TERRAIN_TAG_TERRAIN);
		F_Assert(pTerrain != nullptr && "TerrainSetting::LoadSetting")
		pTerrain->ParserAttribute_String(TERRAIN_TAG_ATTRIBUTE_NAME, this->nameSetting);

		//Param
		FXMLElement* pParam = pTerrain->FindElementChild(TERRAIN_TAG_PARAM);
		F_Assert(pParam != nullptr && "TerrainSetting::LoadSetting")
		pParam->ParserAttribute_Int(TERRAIN_TAG_ATTRIBUTE_LEAF_QUADS, this->nLeafQuads);
		pParam->ParserAttribute_Int(TERRAIN_TAG_ATTRIBUTE_PATCH_QUADS, this->nPatchQuads);

		//Chunks
		FXMLElement* pChunks = pTerrain->FindElementChild(TERRAIN_TAG_CHUNKS);
		F_Assert(pChunks != nullptr && "TerrainSetting::LoadSetting")
		if (!loadChunkedSettings(pChunks))
		{
			F_LogError("*********************** TerrainSetting::LoadSetting, loadChunkedSettings failed, Load path cfg file: [%s] failed !", path.c_str());
			return false;
		}
		F_LogInfo("TerrainSetting::LoadSetting: Load path cfg file: [%s] success !", path.c_str());

		return true;
	}
		bool TerrainSetting::loadChunkedSettings(FXMLElement* pElement)
		{
			pElement->ParserAttribute_Int(TERRAIN_TAG_ATTRIBUTE_START_X, this->nStartX);
			pElement->ParserAttribute_Int(TERRAIN_TAG_ATTRIBUTE_START_Z, this->nStartZ);
			pElement->ParserAttribute_Int(TERRAIN_TAG_ATTRIBUTE_COUNT_X, this->nCountX);
			pElement->ParserAttribute_Int(TERRAIN_TAG_ATTRIBUTE_COUNT_Z, this->nCountZ);
			pElement->ParserAttribute_Float(TERRAIN_TAG_ATTRIBUTE_SIZE_X, this->fSizeX);
			pElement->ParserAttribute_Float(TERRAIN_TAG_ATTRIBUTE_SIZE_Z, this->fSizeZ);
			pElement->ParserAttribute_Int(TERRAIN_TAG_ATTRIBUTE_RESOLUTION, this->nResolution);
			pElement->ParserAttribute_Float(TERRAIN_TAG_ATTRIBUTE_CELL_SIZE, this->fCellSize);
			
			int count_child = pElement->GetElementChildrenCount();
			for (int i = 0; i < count_child; i++)
			{
				FXMLElement* pChunk = pElement->GetElementChild(i);
				if (!loadChunkedSetting(pChunk))
				{
					F_LogError("*********************** TerrainSetting::LoadSetting, loadChunkedSetting failed, index: [%d] !", i);
					return false;
				}
			}

			return true;
		}
			TerrainChunkedSetting* TerrainSetting::loadChunkedSetting(FXMLElement* pElement)
			{
				TerrainChunkedSetting* pCS = new TerrainChunkedSetting();
				pElement->ParserAttribute_Int(TERRAIN_TAG_ATTRIBUTE_X, pCS->nX);
				pElement->ParserAttribute_Int(TERRAIN_TAG_ATTRIBUTE_Z, pCS->nZ);
				pCS->nID = TerrainUtil::ToChunkedID(pCS->nX, pCS->nZ);
				pElement->ParserAttribute_String(TERRAIN_TAG_ATTRIBUTE_HEIGHT_MAP, pCS->nameHeightMap);
				pCS->pathHeightMap = TerrainUtil::GetTerrainHeightMapRelativePath(pCS->nameHeightMap);
				addChunkedSetting(pCS);	

				return pCS;
			}

	bool TerrainSetting::SaveSetting(const String& path)
	{
		FFileXML terrainSetting;
		terrainSetting.CreateXML();

		FXMLDocument* pDoc = terrainSetting.GetXMLDocument();
		F_Assert(pDoc && "TerrainSetting::SaveSetting")

		//Root
		pDoc->CreateElementDeclaration();
		FXMLElement* pRoot = pDoc->CreateElementRoot(TERRAIN_TAG_SETTING);

		//Terrain
		FXMLElement* pTerrain = pRoot->AddElementChild(new FXMLElement(TERRAIN_TAG_TERRAIN));
		pTerrain->SaveAttribute_String(TERRAIN_TAG_ATTRIBUTE_NAME, this->nameSetting);
		
		//Param
		FXMLElement* pParam = pTerrain->AddElementChild(new FXMLElement(TERRAIN_TAG_PARAM));
		pParam->SaveAttribute_Int(TERRAIN_TAG_ATTRIBUTE_LEAF_QUADS, this->nLeafQuads);
		pParam->SaveAttribute_Int(TERRAIN_TAG_ATTRIBUTE_PATCH_QUADS, this->nPatchQuads);

		//Chunks
		FXMLElement* pChunks = pTerrain->AddElementChild(new FXMLElement(TERRAIN_TAG_CHUNKS));
		pChunks->SaveAttribute_Int(TERRAIN_TAG_ATTRIBUTE_START_X, this->nStartX);
		pChunks->SaveAttribute_Int(TERRAIN_TAG_ATTRIBUTE_START_Z, this->nStartZ);
		pChunks->SaveAttribute_Int(TERRAIN_TAG_ATTRIBUTE_COUNT_X, this->nCountX);
		pChunks->SaveAttribute_Int(TERRAIN_TAG_ATTRIBUTE_COUNT_Z, this->nCountZ);
		pChunks->SaveAttribute_Float(TERRAIN_TAG_ATTRIBUTE_SIZE_X, this->fSizeX);
		pChunks->SaveAttribute_Float(TERRAIN_TAG_ATTRIBUTE_SIZE_Z, this->fSizeZ);
		pChunks->SaveAttribute_Int(TERRAIN_TAG_ATTRIBUTE_RESOLUTION, this->nResolution);
		pChunks->SaveAttribute_Float(TERRAIN_TAG_ATTRIBUTE_CELL_SIZE, this->fCellSize);

		if (!saveChunkedSettings(pChunks, this->aChunkedSettings))
		{
			F_LogError("*********************** TerrainSetting::SaveSetting, saveChunkedSettings failed, Save path cfg file: [%s] failed !", path.c_str());
			return false;
		}
		F_LogInfo("TerrainSetting::SaveSetting: Save path cfg file: [%s] success !", path.c_str());

		return terrainSetting.SaveXMLIndirect(path);
	}
		bool TerrainSetting::saveChunkedSettings(FXMLElement* pElement, const TerrainChunkedSettingPtrVector& aCSs)
		{
			for (TerrainChunkedSettingPtrVector::const_iterator it = aCSs.begin();
				 it != aCSs.end(); ++it)
			{
				TerrainChunkedSetting* pCS = *it;
				FXMLElement* pChild = pElement->AddElementChild(new FXMLElement(TERRAIN_TAG_CHUNK));
				saveChunkedSettings(pChild, pCS);
			}

			return true;
		}
			bool TerrainSetting::saveChunkedSettings(FXMLElement* pElement, TerrainChunkedSetting* pCS)
			{
				pElement->SaveAttribute_Int(TERRAIN_TAG_ATTRIBUTE_X, pCS->nX);
				pElement->SaveAttribute_Int(TERRAIN_TAG_ATTRIBUTE_Z, pCS->nZ);
				pElement->SaveAttribute_String(TERRAIN_TAG_ATTRIBUTE_HEIGHT_MAP, pCS->nameHeightMap);

				return true;
			}

	TerrainChunkedSetting* TerrainSetting::GetChunkedSetting(int x, int z)
	{
		int nID = TerrainUtil::ToChunkedID(x, z);
		return GetChunkedSetting(nID);
	}
	TerrainChunkedSetting* TerrainSetting::GetChunkedSetting(int id)
	{
		TerrainChunkedSettingPtrMap::iterator itFind = this->mapChunkedSettings.find(id);
		if (itFind != this->mapChunkedSettings.end())
			return itFind->second;
		return nullptr;
	}

	
	void TerrainSetting::destroyChunkedSetting(TerrainChunkedSetting* pCS)
	{
		TerrainChunkedSettingPtrMap::iterator itFind = this->mapChunkedSettings.find(pCS->nID);
		if (itFind == this->mapChunkedSettings.end())
			return;
		this->mapChunkedSettings.erase(itFind);
		TerrainChunkedSettingPtrVector::iterator itA = std::find(this->aChunkedSettings.begin(), this->aChunkedSettings.end(), pCS);
		if (itA != this->aChunkedSettings.end())
		{
			this->aChunkedSettings.erase(itA);
		}
		F_DELETE(pCS)
	}
	void TerrainSetting::addChunkedSetting(TerrainChunkedSetting* pCS)
	{
		TerrainChunkedSettingPtrMap::iterator itFind = this->mapChunkedSettings.find(pCS->nID);
		if (itFind != this->mapChunkedSettings.end())
			return;

		this->aChunkedSettings.push_back(pCS);
		this->mapChunkedSettings[pCS->nID] = pCS;
	}


}; //LostPeterVulkan