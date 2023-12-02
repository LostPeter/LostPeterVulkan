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

#include "../include/EngineFrameStateAudio.h"
#include "../include/Engine.h"

namespace LostPeterEngine
{
    EngineFrameStateAudio::EngineFrameStateAudio(Engine* pEngine)
        : EngineFrameState(pEngine)
    {
        m_eFrameProfiler = E_FrameProfiler_Audio;
    }

    EngineFrameStateAudio::~EngineFrameStateAudio()
    {

    }

    void EngineFrameStateAudio::Update()
    {

    }
    
}; //LostPeterEngine