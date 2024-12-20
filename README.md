# README #

Author: 

    LostPeter

Target: 

    Learning vulkan render api, compile in different platform with CMake and code the samples !

Github Address:

    LostPeter GitHub: https://github.com/LostPeter/LostPeterVulkan

Document Web: 

    LostPeter 知乎: https://www.zhihu.com/people/lostpeter

Usage:

    Vulkan SDK: 
        I'm using vulkan version 1.3.290.0
        you can change it, but to MacOS, you must replace all libvulkan.1.3.290.dylib in all CMakeLists.txt to your version.

    MacOS:   
        Setup your vulkan sdk and config VULKAN_HOME to your .bash_profile, in CMakeLists.txt we will using it "$ENV{VULKAN_HOME}/macOS/include"  

    Windows: 
        Setup your vulkan sdk and config VULKAN_SDK to your environment %Path%, in CMakeLists.txt we will using it "$ENV{VULKAN_SDK}/Include"
        Check your VisualStudio is setup, and msbuild command tool path in the environment %Path% and can compile VisualStudio .sln project.

    VSCode:
        Config above complete, I'm only using VSCode to edit, compile and debug code in platform Windows/MacOS, maybe later also Android/iOS.
        You can look at .vscode/launch.json file, config your vscode, there are all samples debug config, it is nice and effective to debug direct in vscode.
    

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
            libcpuid-0.5.1
            libnoise-1.0.0
            libsquish-1.15
            rapidxml-1.13
            spdlog-1.10.0
            stb-0.02
            tinyxml-2.6.2
            zlib-1.2.11
            zziplib-0.13.72

    2> Compile Shader
        cd Bin
        cd Shader
        MacOS:      ./Build_MacOS_Shader_HLSL_All.sh or ./Build_MacOS_Shader_GLSL_All.sh
        Windows:    ./Build_Windows_Shader_HLSL_All.bat or ./Build_Windows_Shader_GLSL_All.bat

        Always prefer using hlsl shader, glsl shader maybe delay or not to realize and commit.

    3> Compile LostPeterVulkan
        cd LostPeterVulkan
        cd Compile
        MacOS:      ./Build_MacOS_LostPeterVulkan.sh debug rebuild
        Windows:    ./Build_Windows_LostPeterVulkan.bat debug rebuild

    4> Compile Samples
        cd Sample
        cd Compile
        MacOS:      ./Build_MacOS_Sample_All.sh debug rebuild
        Windows:    ./Build_Windows_Sample_All.bat debug rebuild

License:

    MIT License 


### <000> vulkan_000_window
[Vulkan学习例子000: vulkan_000_window 基础跨平台窗口显示](https://zhuanlan.zhihu.com/p/578806815)
* sample000：Setup a framework, compile all sort of dependencies libraries, show a Window 

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_000_window.png)

### <001> vulkan_001_triangle
[Vulkan学习例子001: vulkan_001_triangle 渲染一个三角形](https://zhuanlan.zhihu.com/p/580793939)
* sample001：Render a triangle with vulkan api

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_001_triangle.png)

### <002> vulkan_002_imgui
[Vulkan学习例子002: vulkan_002_imgui 集成ImGui界面库](https://zhuanlan.zhihu.com/p/581642467)
* sample002：Integrate imgui to vulkan

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_002_imgui.png)

### <003> vulkan_003_texture
[Vulkan学习例子003: vulkan_003_texture 渲染一张纹理](https://zhuanlan.zhihu.com/p/582907670)
* sample003：Render a texture with vulkan api

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_003_texture.png)

### <004> vulkan_004_model
[Vulkan学习例子004: vulkan_004_model 渲染3D模型](https://zhuanlan.zhihu.com/p/583125396)
* sample004：Load different 3D models and textures and then render them

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_004_model.png)

### <005> vulkan_005_camera
[Vulkan学习例子005: vulkan_005_camera 摄像机漫游功能](https://zhuanlan.zhihu.com/p/585204474)
* sample005: Realize a camera and use it in the scene

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_005_camera.png)

### <006> vulkan_006_depth
[Vulkan学习例子006: vulkan_006_depth 深度操作](https://zhuanlan.zhihu.com/p/587243098)
* sample006: Depth state test: depthTest and depthWrite 

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_006_depth.png)

### <007> vulkan_007_stencil
[Vulkan学习例子007: vulkan_007_stencil 模板操作](https://zhuanlan.zhihu.com/p/590091632)
* sample007: Use stencil buffer to render outline

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_007_stencil.png)

### <008> vulkan_008_blend
[Vulkan学习例子008: vulkan_008_blend 混合操作](https://zhuanlan.zhihu.com/p/592389162)
* sample008: Use blend state to render transparent effect

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_008_blend.png)

### <009> vulkan_009_instancing
[Vulkan学习例子009: vulkan_009_instancing 实例渲染](https://zhuanlan.zhihu.com/p/593020588)
* sample009: Vulkan instance rendering

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_009_instancing.png)

### <010> vulkan_010_lighting
[Vulkan学习例子010: vulkan_010_lighting 经典Lambert/Phong/Blinn-Phong光照](https://zhuanlan.zhihu.com/p/593263259)
* sample010: Vulkan classic lighting model, Ambient, Lambert Diffuse, Phong Specular, Blinn-Phong Specular

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_010_lighting.png)

### <011> vulkan_011_texturing
[Vulkan学习例子011: vulkan_011_texturing 基础-高级纹理使用集合](https://zhuanlan.zhihu.com/p/596036603)
* sample011: Vulkan basic textureSampler, texture1D, texture2D, texture2DArray, texture3D, textureCubeMap, textureAnimation, textureBumpMap, textureNormalMap, textureParallaxMap, textureDisplacementMap usage

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_011_texturing.png)

### <012> vulkan_012_shadering
[Vulkan学习例子012: vulkan_012_shadering 各种类型Shader使用集合](https://zhuanlan.zhihu.com/p/600795865)
* sample012: Vulkan Vertex Shader, Tessellation Control Shader, Tessellation Evaluation Shader, Geometry Shader, Fragment Shader, Compute Shader usage

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_012_shadering.png)

### <013> vulkan_013_indirectdraw
[Vulkan学习例子013: vulkan_013_indirectdraw DrawIndirect/MultiDrawIndirect渲染](https://zhuanlan.zhihu.com/p/620624592)
* sample013: Vulkan DrawIndirect /MultiDrawIndirect usage

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_013_indirectdraw.png)

### <014> vulkan_014_multirenderpass
[Vulkan学习例子014: vulkan_014_multirenderpass 多Pass渲染](https://zhuanlan.zhihu.com/p/627517312)
* sample014: Vulkan RenderPass，Multi RenderPass usage

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_014_multirenderpass.png)

### <015> vulkan_015_multiview
[Vulkan学习例子015: vulkan_015_multiview 多视口渲染](https://zhuanlan.zhihu.com/p/624939246)
* sample015: Vulkan RenderPass，Multi RenderPass usage

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_015_multiview.png)

### <016> vulkan_016_geometry
[Vulkan学习例子016: vulkan_016_geometry 常用几何体渲染及其应用-EditorGrid/EditorCameraAxis/EditorCoordinateAxis的实现](https://zhuanlan.zhihu.com/p/632896372)
* sample016: All sort of Geometry Rendering, EditorGrid/EditorCameraAxis/EditorCoordinateAxis Rendering

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_016_geometry.png)

### <017> vulkan_017_collision
[Vulkan学习例子017: vulkan_017_collision 坐标变换详解，RayTrace(线/面/球/包围盒/圆柱/胶囊/圆锥/圆环)碰撞检测算法及应用-坐标轴选择/移动/旋转/缩放操作](https://zhuanlan.zhihu.com/p/645408433)
* sample017: ScreenCoord to WorldCoord, 2D/3D Ray-Collision, EditorGrid/EditorCameraAxis/EditorCoordinateAxis Hover/Mouse Hit Operation

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_017_collision.png)

### <018> vulkan_018_subpass
[Vulkan学习例子018: vulkan_018_subpass SubPass详解](https://zhuanlan.zhihu.com/p/655453644)
* sample018: Vulkan SubPass Usage 

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_018_subpass.png)

### <019> vulkan_019_shadowmap
[Vulkan学习例子019: vulkan_019_shadowmap 阴影](https://zhuanlan.zhihu.com/p/706263110)
* sample019: Vulkan Shadow Effect

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_019_shadowmap.png)