# README #

Author: 

    LostPeter

Target: 

    Learning vulkan render api, compile in different platform with CMake and code the samples !

Document Web: 

    https://www.zhihu.com/people/lostpeter

Usage:

    Vulkan SDK: I'm using vulkan version 1.2.189.0, you can change it.    
    MacOS:   Config VULKAN_HOME, we will set vulkan path in CMakeLists.txt "$ENV{VULKAN_HOME}/macOS/include"  
    Windows: Config VULKAN_SDK, we will set vulkan path in CMakeLists.txt "$ENV{VULKAN_SDK}/Include"


    Build Auto: 
        MacOS:      ./Build_MacOS.sh debug rebuild depends
        Windows:    ./Build_Windows.bat debug rebuild depends

        param1: debug or other is release
        param2: rebuild or other is not rebuild
        param3: depends or other is not build depends


    Build Custom:
    1> Compile Dependencies
        cd Dependencies
        cd Compile
        MacOS:      ./Build_MacOS_lib_all.sh debug rebuild
        Windows:    ./Build_Windows_lib_all.bat debug rebuild

        used dependencies libraries：
            assimp-5.2.4
            glfw-3.3.6
            glm-0.9.9.8
            imgui-1.85
            spdlog-1.10.0
            stb-0.02
            zlib-1.2.11

    2> Compile Shader
        cd Bin
        cd Shader
        MacOS:      ./Build_MacOS_Shader_HLSL_All.sh or ./Build_MacOS_Shader_GLSL_All.sh
        Windows:    ./Build_Windows_Shader_HLSL_All.bat or ./Build_Windows_Shader_GLSL_All.bat

    3> Compile VulkanUtil
        cd VulkanUtil
        cd Compile
        MacOS:      ./Build_MacOS_VulkanUtil.sh debug rebuild
        Windows:    ./Build_Windows_VulkanUtil.bat debug rebuild

    4> Compile Samples
        cd Sample
        cd Compile
        MacOS:      ./Build_MacOS_Sample_All.sh debug rebuild
        Windows:    ./Build_Windows_Sample_All.bat debug rebuild


### <000> vulkan_000_window
* sample000：setup a framework, compile all sort of dependencies libraries, show a Window 

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_000_window.png)

### <001> vulkan_001_triangle
* sample001：render a triangle with vulkan api

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_001_triangle.png)

### <002> vulkan_002_imgui
* sample002：integrate imgui to vulkan

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_002_imgui.png)

### <003> vulkan_003_texture
* sample003：render a texture with vulkan api

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_003_texture.png)

### <004> vulkan_004_model
* sample004：load different 3D models and textures nd then render them

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_004_model.png)

### <005> vulkan_005_camera
* sample005: realize a camera and use it in the scene

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_005_camera.png)

### <006> vulkan_006_depth
* sample006: Depth state test: depthTest and depthWrite 

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_006_depth.png)

### <007> vulkan_007_stencil
* sample007: Stencil usage sample

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_007_stencil.png)
