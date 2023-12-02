/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-11
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _OBJECT_CAMERA_LISTENER_H_
#define _OBJECT_CAMERA_LISTENER_H_

#include "PreDefine.h"

namespace LostPeterEngine
{
    class engineExport ObjectCameraListener
    {
    public:
        ObjectCameraListener();
        virtual ~ObjectCameraListener();

    public:
        virtual void CameraDestroyed(ObjectCamera* pCamera) { }
    };

}; //LostPeterEngine

#endif