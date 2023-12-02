/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-02
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _SYSTEM_CAPABILITIES_H_
#define _SYSTEM_CAPABILITIES_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport SystemCapabilities : public FSingleton<SystemCapabilities>
                                          , public Base
    {
    public:
        SystemCapabilities();
        virtual ~SystemCapabilities();

    public:
    public:
        static SystemCapabilities& GetSingleton();
		static SystemCapabilities* GetSingletonPtr();


    public:
        void Destroy();
        void Init();


    protected:
        

    };

}; //LostPeterEngine

#endif