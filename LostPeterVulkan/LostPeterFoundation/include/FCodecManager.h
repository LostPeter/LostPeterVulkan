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

#ifndef _F_CODEC_MANAGER_H_
#define _F_CODEC_MANAGER_H_

#include "FPreDefine.h"
#include "FSingleton.h"

namespace LostPeterFoundation
{
    class foundationExport FCodecManager : public FSingleton<FCodecManager>
    {
    public:
        FCodecManager();
        ~FCodecManager();

    public:
		typedef std::map<String, FCodec*, std::less<String>> CodecMap;

	protected:
		static CodecMap	ms_mapCodec;

    public:
        static FCodecManager& GetSingleton();
        static FCodecManager* GetSingletonPtr();

    public:
		static void	Destroy();

	public:
		static void	RegisterCodec(FCodec* pCodec);
	    static void	UnRegisterCodec(FCodec* pCodec);
		static StringVector GetExtensions();

		static FCodec* GetCodec(const String& strExt);
		static FCodec* GetCodec(char* szMagicNum, size_t nMaxbytes);
    };

}; //LostPeterFoundation

#endif