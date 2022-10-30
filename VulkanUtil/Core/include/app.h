// author: LostPeter
// time:   2022-10-30

#ifndef _APP_H_
#define _APP_H_

#include "predefine.h"

namespace LibUtil
{
    class utilExport App
    {
    public:
        static GLFWwindow* s_pWindow;
        static VulkanBase* s_pBase;

    public:
        static int Run(VulkanBase* pBase, int vulkanVersionMajor, int vulkanVersionMinor);
        static GLFWwindow* GetGLFWwindow() { return s_pWindow; }
        
    };

}; //LibUtil

#endif