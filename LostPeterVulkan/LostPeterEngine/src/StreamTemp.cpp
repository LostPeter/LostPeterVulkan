/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-03
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/StreamTemp.h"
#include "../include/StreamManager.h"

namespace LostPeterEngine
{
    StreamTemp::StreamTemp()
    {
        m_pStreamManager = StreamManager::GetSingletonPtr();
		F_Assert(m_pStreamManager && "StreamTemp::StreamTemp !")
    }

    StreamTemp::~StreamTemp()
    {
        
    }

}; //LostPeterEngine