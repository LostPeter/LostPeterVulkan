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

#ifndef _F_PATH_MANAGER_H_
#define _F_PATH_MANAGER_H_

#include "FSingleton.h"

namespace LostPeterFoundation
{
    class LPF_Export FPathManager : public FSingleton<FPathManager>
	{
	public:
		FPathManager();
		virtual ~FPathManager();

	public:
		enum PathGroupType
		{
		////Config
			PathGroup_Config				= 0,
		////Data
			PathGroup_Data					= 1000,
		////Editor
			PathGroup_Editor				= 2000,
		////Font
			PathGroup_Font					= 3000,
		////Shader
			PathGroup_Shader				= 4000,
		////Mesh
			PathGroup_Mesh					= 5000,
			PathGroup_MeshCommon			= 5001,
			PathGroup_MeshModel				= 5002,
			PathGroup_MeshCharacter			= 5003,
		////Texture
			PathGroup_Texture				= 6000,
			PathGroup_TextureCommon			= 6001,
			PathGroup_TextureModel			= 6002,
			PathGroup_TextureCharacter		= 6003,
			PathGroup_TextureTerrain		= 6004,
			PathGroup_TextureGfx			= 6005,
			PathGroup_TextureSky			= 6006,
		////Material
			PathGroup_Material				= 7000,
			PathGroup_MaterialCommon		= 7001,
			PathGroup_MaterialModel			= 7002,
			PathGroup_MaterialCharacter		= 7003,
		////Audio
			PathGroup_Audio					= 8000,
			PathGroup_AudioCommon			= 8001,
			PathGroup_AudioModel			= 8002,
			PathGroup_AudioCharacter		= 8003,
		////Gfx
			PathGroup_Gfx					= 9000,
			PathGroup_GfxCommon				= 9001,
			PathGroup_GfxModel				= 9002,
			PathGroup_GfxCharacter			= 9003,
		////Terrain					
			PathGroup_Terrain				= 10000,
		////UI
			PathGroup_UI					= 11000,
		////PostEffect
			PathGroup_PostEffect			= 12000,
		////Script	
			PathGroup_Script				= 13000,
		////Scene	
			PathGroup_Scene					= 14000,

			PathGroup_Count
		};

		static const String ms_strPathCfgPath;

	private:
		FPathGroupMap m_mapGroup;
		FPathGroupIDMap	m_mapGroupID;
		FPathGroupBaseMap m_mapGroupBase;
		FPathGroupRecursiveMap m_mapGroupRecursive;
		
		String m_strWorkFolder;
	
	public:
		static FPathManager& GetSingleton();
		static FPathManager* GetSingletonPtr();
	
	public:
		LP_FORCEINLINE FPathGroupMap& GetPathGroupMap() { return m_mapGroup; }
		LP_FORCEINLINE const String& GetWorkFolder() const { return m_strWorkFolder; }
		
	public:
		String GetFilePath(uint32 nGroup, const String& strName);
		bool GetGroupMaxID(uint32 nGroup, uint32& nMaxID);
		bool GetGroupBasePath(uint32 nGroup, String& strBaseName);
		bool GetGroupInfo(uint32 nGroup, uint32& nMaxID, String& strBaseName);

	public:
		bool Init();
		void Release();

		bool RegisterUserGroup(uint32 nBaseGroup, uint32 nGroup, const String& strPath, bool bIsRecursive);
		bool ReloadGroup(uint32 nGroup, bool bIsRecursive);

	public:
		bool LoadCfg();
		bool SaveCfg();
	
	private:
		bool registerGroup(uint32 nGroup, const String& strPath, bool bIsRecursive);
		void setDefaultGroupID();
	};

}; //LostPeterFoundation

#endif