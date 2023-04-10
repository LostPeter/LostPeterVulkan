# README #

Author: 

    LostPeter

Target: 

    Learning vulkan render api, compile in different platform with CMake and code the samples !

Github Address:

    https://github.com/LostPeter/LostPeterVulkan

Document Web: 

    https://www.zhihu.com/people/lostpeter

Usage:

    Vulkan SDK: 
        I'm using vulkan version 1.2.189.0
        you can change it, but to MacOS, you must replace all libvulkan.1.2.189.dylib in all CMakeLists.txt to your version.

    MacOS:   
        Setup your vulkan sdk and config VULKAN_HOME to your .bash_profile, in CMakeLists.txt we will using it "$ENV{VULKAN_HOME}/macOS/include"  

    Windows: 
        Setup your vulkan sdk and config VULKAN_SDK to your environment %Path%, in CMakeLists.txt we will using it "$ENV{VULKAN_SDK}/Include"
        Check your VisualStudio is setup, and msbuild command tool path in the environment %Path% and can compile VisualStudio .sln project.

    VSCode:
        Config above complete, I'm only using VSCode to edit, compile and debug code in platform Windows/MacOS, maybe later also Android/iOS.
        You can look at .vscode/launch.json file, config your vscode, there is all samples debug config, it is nice and effective to debug direct in vscode.
    

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

        Always prefer using hlsl shader, glsl shader maybe delay or not to realize and commit.

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

License:

    MIT License 


### <000> vulkan_000_window
* sample000：Setup a framework, compile all sort of dependencies libraries, show a Window 

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_000_window.png)

### <001> vulkan_001_triangle
* sample001：Render a triangle with vulkan api

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_001_triangle.png)

### <002> vulkan_002_imgui
* sample002：Integrate imgui to vulkan

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_002_imgui.png)

### <003> vulkan_003_texture
* sample003：Render a texture with vulkan api

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_003_texture.png)

### <004> vulkan_004_model
* sample004：Load different 3D models and textures and then render them

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_004_model.png)

### <005> vulkan_005_camera
* sample005: Realize a camera and use it in the scene

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_005_camera.png)

### <006> vulkan_006_depth
* sample006: Depth state test: depthTest and depthWrite 

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_006_depth.png)

### <007> vulkan_007_stencil
* sample007: Use stencil buffer to render outline

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_007_stencil.png)

### <008> vulkan_008_blend
* sample008: Use blend state to render transparent effect

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_008_blend.png)

### <009> vulkan_009_instancing
* sample009: Vulkan instance rendering

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_009_instancing.png)

### <010> vulkan_010_lighting
* sample010: Vulkan classic lighting model, Ambient, Lambert Diffuse, Phong Specular, Blinn-Phong Specular

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_010_lighting.png)

### <011> vulkan_011_texturing
* sample011: Vulkan basic textureSampler, texture1D, texture2D, texture2DArray, texture3D, textureCubeMap, textureAnimation, textureBumpMap, textureNormalMap, textureParallaxMap usage

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_011_texturing.png)

### <012> vulkan_012_shadering
* sample012: Vulkan Vertex Shader, Tessellation Control Shader, Tessellation Evaluation Shader, Geometry Shader, Fragment Shader, Compute Shader usage

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_012_shadering.png)
