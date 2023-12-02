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

#ifndef _ENGINE_FRAME_STATE_H_
#define _ENGINE_FRAME_STATE_H_

#include "PreDefine.h"

namespace LostPeterEngine
{
    class engineExport EngineFrameState
    {
    public:
        EngineFrameState(Engine* pEngine);
        virtual ~EngineFrameState();

    public:
        Engine* m_pEngine;
        EFrameProfilerType m_eFrameProfiler;

    public:
        virtual void Update() = 0;
    };

}; //LostPeterEngine

#endif