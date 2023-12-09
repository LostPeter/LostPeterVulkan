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

#ifndef _ENGINE_FRAME_PROFILER_H_
#define _ENGINE_FRAME_PROFILER_H_

#include "PreDefine.h"

namespace LostPeterEngine
{
    class engineExport EngineFrameProfiler
    {
    public:
        EngineFrameProfiler(Engine* pEngine);
        virtual ~EngineFrameProfiler();

    public:
    protected:
       Engine* m_pEngine;

       EngineFrameStateAudio* m_pFrameState_Audio;
       EngineFrameStateMemory* m_pFrameState_Memory;
       EngineFrameStatePhysics* m_pFrameState_Physics;
       EngineFrameStateRender* m_pFrameState_Render;
       EngineFrameState* m_aEngineFrameStates[(int)E_FrameProfiler_Count];

    public:
        F_FORCEINLINE EngineFrameStateAudio* GetEngineFrameStateAudio() { return m_pFrameState_Audio; }
        F_FORCEINLINE EngineFrameStateMemory* GetEngineFrameStateMemory() { return m_pFrameState_Memory; }
        F_FORCEINLINE EngineFrameStatePhysics* GetEngineFrameStatePhysics() { return m_pFrameState_Physics; }
        F_FORCEINLINE EngineFrameStateRender* GetEngineFrameStateRender() { return m_pFrameState_Render; }
        F_FORCEINLINE EngineFrameState* GetEngineFrameState(EFrameProfilerType eType) { return m_aEngineFrameStates[(int32)eType]; }

    public:
        virtual void Destroy();
        virtual void Init();

        virtual EngineFrameStateAudio* CreateFrameStateAudio();
        virtual EngineFrameStateMemory* CreateFrameStateMemory();
        virtual EngineFrameStatePhysics* CreateFrameStatePhysics();
        virtual EngineFrameStateRender* CreateFrameStateRender();

        virtual void Update();
    };

}; //LostPeterEngine

#endif