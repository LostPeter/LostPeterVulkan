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

#ifndef _TERRAIN_SETTING_H_
#define _TERRAIN_SETTING_H_

#include "Base.h"

namespace LostPeterVulkan
{
	/////////////////////////// TerrainChunkedSetting ////////////////////
	class vulkanExport TerrainChunkedSetting
	{
	public:
		TerrainChunkedSetting();
		~TerrainChunkedSetting();

	public:
		int nX;
		int nZ;
		int nID;
		String nameHeightMap;
		String pathHeightMap;
		int nResolution;
		int nSize;
		
		String strTextureDiffuse;
		String strTextureNormal;
		String strTextureControl;
		StringVector aPathTextureDiffuse;
		StringVector aPathTextureNormal;
		StringVector aPathTextureControl;

	public:
		StringVector ToPathTextures(const String& names);
	};


	/////////////////////////// TerrainSetting ///////////////////////////
	class vulkanExport TerrainSetting : public FSingleton<TerrainSetting>
                                   	  , public Base
    {
    public:
        TerrainSetting();
        virtual ~TerrainSetting();

	public:
		String nameSetting;
		String pathSetting;

		int nLeafQuads;
		int nPatchQuads;
		int nLodCount;
		float fLodUpdateDis;
		float fLodRadius0;
		float fLodRadius1;
		float fLodRadius2;
		float fLodRadius3;

		float fLodPixelError;
		bool bHeadless;
		uint32 nAutoCloseMs;

		int nStartX;
		int nStartZ;
		int nCountX;
		int nCountZ;
		float fSizeX;
		float fSizeZ;
		int nResolution;
		float fCellSize;
		int nSize;
		TerrainChunkedSettingPtrVector aChunkedSettings;
		TerrainChunkedSettingPtrMap mapChunkedSettings;

		bool bIsInit;

	public:
        static TerrainSetting& GetSingleton();
		static TerrainSetting* GetSingletonPtr();

	public:
		F_FORCEINLINE int GetLeafQuads() const { return this->nLeafQuads; }
		F_FORCEINLINE int GetPatchQuads() const { return this->nPatchQuads; }
		F_FORCEINLINE int GetLodCount() const { return this->nLodCount; }

		F_FORCEINLINE int GetStartX() const { return this->nStartX; }
		F_FORCEINLINE int GetStartZ() const { return this->nStartZ; }
		F_FORCEINLINE int GetCountX() const { return this->nCountX; }
		F_FORCEINLINE int GetCountZ() const { return this->nCountZ; }
		F_FORCEINLINE float GetSizeX() const { return this->fSizeX; }
		F_FORCEINLINE float GetSizeZ() const { return this->fSizeZ; }

		F_FORCEINLINE const TerrainChunkedSettingPtrVector& GetChunkedSettingPtrVector() const { return this->aChunkedSettings; }
		F_FORCEINLINE const TerrainChunkedSettingPtrMap& GetChunkedSettingPtrMap() const { return this->mapChunkedSettings; }

		F_FORCEINLINE bool IsInit() const { return this->bIsInit; }
		F_FORCEINLINE void SetIsInit(bool b) { this->bIsInit = b; }

	public:
		void Destroy();
        bool Init(const String& path);

		bool LoadSetting(const String& path);
		bool SaveSetting(const String& path);

		TerrainChunkedSetting* GetChunkedSetting(int x, int z);
		TerrainChunkedSetting* GetChunkedSetting(int id);

		
	protected:
	////destroy
		void destroySettings();

	////create
		void destroyChunkedSetting(TerrainChunkedSetting* pCS);
		void addChunkedSetting(TerrainChunkedSetting* pCS);
		
	////load/save
		bool loadChunkedSettings(FXMLElement* pElement);
			TerrainChunkedSetting* loadChunkedSetting(FXMLElement* pElement);

		bool saveChunkedSettings(FXMLElement* pElement, const TerrainChunkedSettingPtrVector& aCSs);
			bool saveChunkedSettings(FXMLElement* pElement, TerrainChunkedSetting* pCS);
	};

}; //LostPeterVulkan

#endif