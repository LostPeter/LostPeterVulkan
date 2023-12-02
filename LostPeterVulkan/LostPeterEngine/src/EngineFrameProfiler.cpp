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

#include "../include/EngineFrameProfiler.h"
#include "../include/Engine.h"
#include "../include/EngineFrameStateAudio.h"
#include "../include/EngineFrameStateMemory.h"
#include "../include/EngineFrameStatePhysics.h"
#include "../include/EngineFrameStateRender.h"

namespace LostPeterEngine
{
    EngineFrameProfiler::EngineFrameProfiler(Engine* pEngine)
        : m_pEngine(pEngine)
    {

    }

    EngineFrameProfiler::~EngineFrameProfiler()
    {
        Destroy();
    }

    void EngineFrameProfiler::Destroy()
    {
        for (int i = 0; i < (int)E_FrameProfiler_Count; i++)
        {
            EngineFrameState* pFrameState = m_aEngineFrameStates[i];
            delete pFrameState;

            m_aEngineFrameStates[i] = nullptr;
        }
        m_pFrameState_Audio = nullptr;
        m_pFrameState_Memory = nullptr;
        m_pFrameState_Physics = nullptr;
        m_pFrameState_Render = nullptr;
    }

    void EngineFrameProfiler::Init()
    {
        m_pFrameState_Audio = CreateFrameStateAudio();
        m_pFrameState_Memory = CreateFrameStateMemory();
        m_pFrameState_Physics = CreateFrameStatePhysics();
        m_pFrameState_Render = CreateFrameStateRender();

        m_aEngineFrameStates[(int)m_pFrameState_Audio->m_eFrameProfiler] = m_pFrameState_Audio;
        m_aEngineFrameStates[(int)m_pFrameState_Memory->m_eFrameProfiler] = m_pFrameState_Memory;
        m_aEngineFrameStates[(int)m_pFrameState_Physics->m_eFrameProfiler] = m_pFrameState_Physics;
        m_aEngineFrameStates[(int)m_pFrameState_Render->m_eFrameProfiler] = m_pFrameState_Render;
    }

    EngineFrameStateAudio* EngineFrameProfiler::CreateFrameStateAudio()
    {
        return new EngineFrameStateAudio(m_pEngine);
    }

    EngineFrameStateMemory* EngineFrameProfiler::CreateFrameStateMemory()
    {
        return new EngineFrameStateMemory(m_pEngine);
    }

    EngineFrameStatePhysics* EngineFrameProfiler::CreateFrameStatePhysics()
    {
        return new EngineFrameStatePhysics(m_pEngine);
    }

    EngineFrameStateRender* EngineFrameProfiler::CreateFrameStateRender()
    {
        return new EngineFrameStateRender(m_pEngine);
    }

    void EngineFrameProfiler::Update()
    {   
        for (int i = 0; i < (int)E_FrameProfiler_Count; i++)
        {
            EngineFrameState* pFrameState = m_aEngineFrameStates[i];
            pFrameState->Update();
        }
    }   
    
}; //LostPeterEngine