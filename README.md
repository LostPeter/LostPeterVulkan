# README #

Author: 

    LostPeter

Target: 

    Learning vulkan render api, compile in different platform with CMake and code the samples !

Document Web: 

    https://www.zhihu.com/people/lostpeter

Usage:
    
    1> Compile Dependencies
        cd Dependencies
        cd Compile
        MacOS:      ./Build_all_lib_MacOS.sh debug
        Windows:    ./Build_all_lib_Windows.bat debug

    2> Compile Shader
        cd Bin
        cd Shader
        MacOS:      ./Build_Shader_HLSL_MacOS_All.sh or ./Build_Shader_GLSL_MacOS_All.sh
        Windows:    ./Build_Shader_HLSL_Windows_All.bat or ./Build_Shader_GLSL_Windows_All.bat

    3> Compile Samples
        cd Sample
        cd Compile
        MacOS:      ./Build_Sample_MacOS_All.sh debug
        Windows:    ./Build_Sample_Windows_All.bat debug


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