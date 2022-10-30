// author: LostPeter
// time:   2022-10-30

#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "predefine.h"

namespace LibUtil
{
    class utilExport SceneManager
    {
    public:
        SceneManager();
        virtual~SceneManager();

    public: 
        static int s_numFrameResources;

    };

};  //LibUtil

#endif