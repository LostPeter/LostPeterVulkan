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

#ifndef _STREAM_TEMP_H_
#define _STREAM_TEMP_H_

#include "PreDefine.h"

namespace LostPeterEngine
{
    class engineExport StreamTemp
    {
    public:
        StreamTemp();
        virtual ~StreamTemp();

    public:
	protected:
		StreamManager* m_pStreamManager;
	
	public:
		virtual	void DeleteStream(Stream* pStream) = 0;
    };

}; //LostPeterEngine

#endif