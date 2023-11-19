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

#ifndef _ENGINE_CONFIG_H_
#define _ENGINE_CONFIG_H_

#include "PreDefine.h"

namespace LostPeter
{
    class utilExport EngineConfig
    {
    public:
        EngineConfig();
        virtual ~EngineConfig();

    public:
    protected:


    public:
        bool Load(const String& strCfgPath, Engine* pEngine);
		bool Save(const String& strCfgPath, Engine* pEngine);
    };

}; //LostPeter

#endif