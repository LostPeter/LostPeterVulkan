// author: LostPeter
// time:   2022-10-30

#ifndef _APP_H_
#define _APP_H_

#include "predefine.h"

namespace LostPeter
{
    class utilExport App
    {
    public:
        static GLFWwindow* s_pWindow;
        static VulkanBase* s_pBase;

    public:
        static int Run(VulkanBase* pBase, int vulkanVersionMajor, int vulkanVersionMinor);
        static GLFWwindow* GetGLFWwindow() { return s_pWindow; }
        
    protected:
        static void error_callback(int error, const char* description);
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        static void cursor_position_callback(GLFWwindow* window, double x, double y);
        static void scroll_callback(GLFWwindow* window, double x, double y);
    };

}; //LostPeter

#endif