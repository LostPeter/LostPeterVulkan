/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-06
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/FCodecManager.h"
#include "../include/FCodec.h"
#include "../include/FUtilString.h"

namespace LostPeterFoundation
{
    template<> FCodecManager* FSingleton<FCodecManager>::ms_Singleton = nullptr;
    FCodecManager* FCodecManager::GetSingletonPtr()
    {
        return ms_Singleton;
    }
    FCodecManager& FCodecManager::GetSingleton()
    {  
        F_Assert(ms_Singleton && "FCodecManager::GetSingleton")
        return (*ms_Singleton);  
    }

    FCodecManager::CodecMap FCodecManager::ms_mapCodec;
    FCodecManager::FCodecManager()
    {

    }

    FCodecManager::~FCodecManager()
    {
        Destroy();
    }

    void FCodecManager::Destroy()
	{
		for (CodecMap::iterator it = ms_mapCodec.begin(); 
			 it != ms_mapCodec.end(); ++it)
		{
			delete (it->second);
		}
		ms_mapCodec.clear();
	}

	void FCodecManager::RegisterCodec(FCodec* pCodec)
	{
		ms_mapCodec[pCodec->GetCodecType()] = pCodec;
	}

	void FCodecManager::UnRegisterCodec(FCodec* pCodec)
	{
		ms_mapCodec.erase(pCodec->GetCodecType());
	}

	StringVector FCodecManager::GetExtensions()
	{
		StringVector retArray;
		retArray.reserve(ms_mapCodec.size());
		for (CodecMap::const_iterator it = ms_mapCodec.begin();
			 it != ms_mapCodec.end(); ++it)
		{
			retArray.push_back(it->first);
		}
		return retArray;
	}

	FCodec* FCodecManager::GetCodec(const String& strExt)
	{
		String lowercase = strExt;
		FUtilString::ToLowerCase(lowercase);
		CodecMap::iterator itFind = ms_mapCodec.find(lowercase);
		if (itFind == ms_mapCodec.end())
		{
			F_LogError("FCodecManager::GetCodec: Can not find codec for extension: [%s] !", strExt.c_str());
			return nullptr;
		}
		return itFind->second;
	}

	FCodec* FCodecManager::GetCodec(char* szMagicNum, size_t nMaxbytes)
	{
		for (CodecMap::const_iterator it = ms_mapCodec.begin(); 
			 it != ms_mapCodec.end(); ++it)
		{
			String ext = it->second->MagicNumberToFileExt(szMagicNum, nMaxbytes);
			if (!ext.empty())
			{
				if (ext == it->second->GetCodecType())
					return it->second;
				else
					return GetCodec(ext);
			}
		}

		return nullptr;
	}
    
}; //LostPeterFoundation