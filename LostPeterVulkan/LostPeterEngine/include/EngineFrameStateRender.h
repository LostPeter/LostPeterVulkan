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

#ifndef _ENGINE_FRAME_STATE_RENDER_H_
#define _ENGINE_FRAME_STATE_RENDER_H_

#include "EngineFrameState.h"

namespace LostPeterEngine
{
    class engineExport EngineFrameStateRender : public EngineFrameState
    {
    public:
        EngineFrameStateRender(Engine* pEngine);
        virtual ~EngineFrameStateRender();

    public:

    public:
        virtual void Update();
    };

}; //LostPeterEngine

#endif