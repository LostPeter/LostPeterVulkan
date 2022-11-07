# README #

Author: 

    LostPeter

Target: 

    Learning vulkan render api, compile in different platform with CMake and code the samples !

Usage:
    
    1> Compile Dependencies
        cd Dependencies
        cd Compile
        MacOS:      ./Build_all_lib_MacOS.sh debug
        Windows:    ./Build_all_lib_Windows.bat debug

    2> Compile Samples
        cd Sample
        cd Compile
        MacOS:      ./Build_Shader_MacOS_All.sh   
                    ./Build_Sample_MacOS_All.sh debug
        Windows:    ./Build_Shader_Windows_All.bat
                    ./Build_Sample_Windows_All.bat debug


### <000> vulkan_000_window
* sample000：setup a framework, compile all sort of dependencies libraries, show a Window 

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_000_window.png)

### <001> vulkan_001_triangle
* sample001：render a triangle with vulkan api

![image](https://github.com/LostPeter/LostPeterVulkan/blob/main/Images/vulkan_001_triangle.png)
