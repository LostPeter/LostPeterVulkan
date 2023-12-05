#****************************************************************************
# LostPeterVulkan - Copyright (C) 2022 by LostPeter
# 
# Author: LostPeter
# Time:   2022-10-30
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

CMAKE_MINIMUM_REQUIRED(VERSION 3.4)

SET(PLATFORM_MODE_DX12 "dx12")
SET(PLATFORM_MODE_GL3 "gl3")
SET(PLATFORM_MODE_GLES3_ANDROID "gles3_android")
SET(PLATFORM_MODE_GLES3_IOS "gles3_ios")
SET(PLATFORM_MODE_GLFW_VULKAN "glfw_vulkan")
SET(PLATFORM_MODE_METAL "metal")
MESSAGE(STATUS "Build platform: ${PLATFORM_MODE}")

if (${PLATFORM_MODE} STREQUAL ${PLATFORM_MODE_DX12})
    SET(NAME_PROJ "imgui-1.85_dx12")
elseif (${PLATFORM_MODE} STREQUAL ${PLATFORM_MODE_GL3}) 
    SET(NAME_PROJ "imgui-1.85_gl3")
elseif (${PLATFORM_MODE} STREQUAL ${PLATFORM_MODE_GLES3_ANDROID}) 
    SET(NAME_PROJ "imgui-1.85_gles3_android")
elseif (${PLATFORM_MODE} STREQUAL ${PLATFORM_MODE_GLES3_IOS}) 
    SET(NAME_PROJ "imgui-1.85_gles3_ios")
elseif (${PLATFORM_MODE} STREQUAL ${PLATFORM_MODE_GLFW_VULKAN}) 
    SET(NAME_PROJ "imgui-1.85_glfw_vulkan")
elseif (${PLATFORM_MODE} STREQUAL ${PLATFORM_MODE_METAL}) 
    SET(NAME_PROJ "imgui-1.85_metal")
else ()
    MESSAGE(FATAL_ERROR "Unknown PLATFORM_MODE: ${PLATFORM_MODE}")
endif ()

if (DEBUG)
    PROJECT(${NAME_PROJ}_d)
    SET(CMAKE_BUILD_TYPE "Debug")
    if (!WIN32)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    endif()

    MESSAGE(STATUS "Set Debug Build !")
else ()
    PROJECT(${NAME_PROJ})
endif ()

SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_STANDARD 11)

#MESSAGE(STATUS "HOME dir: $ENV{HOME}")
MESSAGE(STATUS "Binary dir: ${PROJECT_BINARY_DIR}")
MESSAGE(STATUS "SOURCE dir: ${PROJECT_SOURCE_DIR}")
MESSAGE(STATUS "SYSTEM name: ${CMAKE_SYSTEM_NAME}")
MESSAGE(STATUS "SYSTEM version: ${CMAKE_SYSTEM_VERSION}")
MESSAGE(STATUS "SYSTEM processor: ${CMAKE_SYSTEM_PROCESSOR}")


function(ASSIGN_SOURCE_GROUP)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            FILE(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else ()
            SET(_source_rel "${_source}")
        endif ()
        GET_FILENAME_COMPONENT(_source_path "${_source_rel}" PATH)
        STRING(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        SOURCE_GROUP("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(ASSIGN_SOURCE_GROUP)

function(MY_ADD_LIBRARY)
    foreach(_source IN ITEMS ${ARGN})
        ASSIGN_SOURCE_GROUP(${_source})
    endforeach()
    ADD_LIBRARY(${ARGV})
endfunction(MY_ADD_LIBRARY)


if (${PLATFORM_MODE} STREQUAL ${PLATFORM_MODE_DX12})
    #HEADER_FILES
    FILE(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.h"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/impl_dx12/*.h"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/impl_win32/*.h")
    MESSAGE(STATUS "HEADER files: ${HEADER_FILES}")
    #SOURCE_FILES_CPP
    FILE(GLOB_RECURSE SOURCE_FILES_CPP "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.cpp" 
                                       "${CMAKE_CURRENT_SOURCE_DIR}/impl_dx12/*.cpp"
                                       "${CMAKE_CURRENT_SOURCE_DIR}/impl_win32/*.cpp")
    MESSAGE(STATUS "SOURCE .cpp files: ${SOURCE_FILES_CPP}")
    #SOURCE_FILES_C
    FILE(GLOB_RECURSE SOURCE_FILES_C "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.c"
                                     "${CMAKE_CURRENT_SOURCE_DIR}/impl_win32/*.c")
    MESSAGE(STATUS "SOURCE .c files: ${SOURCE_FILES_C}")

    #LIB_INCLUDE

    #LIB_XXX

    #Build
    MESSAGE(STATUS "Build ${PROJECT_NAME} platfrom Win32 !")
    ADD_DEFINITIONS(-DIMGUI_EXPORTS)
    if(MSVC)
        ADD_DEFINITIONS(/wd4251 /wd4275)
    endif()

    LINK_LIBRARIES(shlwapi.lib
                   d3d12.lib
                   dxgi.lib
                   d3dcompiler.lib)
    MY_ADD_LIBRARY(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES_CPP} ${SOURCE_FILES_C})

elseif (${PLATFORM_MODE} STREQUAL ${PLATFORM_MODE_GL3}) 
    #HEADER_FILES
    FILE(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.h"
                                  "${CMAKE_CURRENT_SOURCE_DIR}/impl_gl3/*.h")
    MESSAGE(STATUS "HEADER files: ${HEADER_FILES}")
    #SOURCE_FILES_CPP
    FILE(GLOB_RECURSE SOURCE_FILES_CPP "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.cpp" 
                                       "${CMAKE_CURRENT_SOURCE_DIR}/impl_gl3/*.cpp")
    MESSAGE(STATUS "SOURCE .cpp files: ${SOURCE_FILES_CPP}")
    #SOURCE_FILES_C
    FILE(GLOB_RECURSE SOURCE_FILES_C "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.c"
                                     "${CMAKE_CURRENT_SOURCE_DIR}/impl_gl3/*.c")
    MESSAGE(STATUS "SOURCE .c files: ${SOURCE_FILES_C}")

    #SOURCE_FILES_MM
    if (APPLE)
        FILE(GLOB_RECURSE SOURCE_FILES_MM "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.mm"
                                          "${CMAKE_CURRENT_SOURCE_DIR}/impl_gl3/*.mm")
        MESSAGE(STATUS "SOURCE .mm files: ${SOURCE_FILES_MM}")
    endif ()

    #LIB_INCLUDE

    #LIB_XXX

    #Build


elseif (${PLATFORM_MODE} STREQUAL ${PLATFORM_MODE_GLES3_ANDROID}) 
    #HEADER_FILES
    FILE(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.h"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/impl_gles3_android/*.h")
    MESSAGE(STATUS "HEADER files: ${HEADER_FILES}")
    #SOURCE_FILES_CPP
    FILE(GLOB_RECURSE SOURCE_FILES_CPP "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.cpp" 
                                       "${CMAKE_CURRENT_SOURCE_DIR}/impl_gles3_android/*.cpp")
    MESSAGE(STATUS "SOURCE .cpp files: ${SOURCE_FILES_CPP}")
    #SOURCE_FILES_C
    FILE(GLOB_RECURSE SOURCE_FILES_C "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.c"
                                     "${CMAKE_CURRENT_SOURCE_DIR}/impl_gles3_android/*.c")
    MESSAGE(STATUS "SOURCE .c files: ${SOURCE_FILES_C}")

    #LIB_INCLUDE

    #LIB_XXX

    #Build


elseif (${PLATFORM_MODE} STREQUAL ${PLATFORM_MODE_GLES3_IOS}) 
    #HEADER_FILES
    FILE(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.h"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/impl_gles3_ios/*.h")
    MESSAGE(STATUS "HEADER files: ${HEADER_FILES}")
    #SOURCE_FILES_CPP
    FILE(GLOB_RECURSE SOURCE_FILES_CPP "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.cpp" 
                                       "${CMAKE_CURRENT_SOURCE_DIR}/impl_gles3_ios/*.cpp")
    MESSAGE(STATUS "SOURCE .cpp files: ${SOURCE_FILES_CPP}")
    #SOURCE_FILES_C
    FILE(GLOB_RECURSE SOURCE_FILES_C "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.c"
                                     "${CMAKE_CURRENT_SOURCE_DIR}/impl_gles3_ios/*.c")
    MESSAGE(STATUS "SOURCE .c files: ${SOURCE_FILES_C}")

    #SOURCE_FILES_MM
    if (APPLE)
        FILE(GLOB_RECURSE SOURCE_FILES_MM "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.mm"
                                          "${CMAKE_CURRENT_SOURCE_DIR}/impl_gles3_ios/*.mm")
        MESSAGE(STATUS "SOURCE .mm files: ${SOURCE_FILES_MM}")
    endif ()

    #LIB_INCLUDE

    #LIB_XXX

    #Build

elseif (${PLATFORM_MODE} STREQUAL ${PLATFORM_MODE_GLFW_VULKAN})
    #HEADER_FILES
    FILE(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.h"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/impl_glfw/*.h"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/impl_vulkan/*.h")
    MESSAGE(STATUS "HEADER files: ${HEADER_FILES}")
    #SOURCE_FILES_CPP
    FILE(GLOB_RECURSE SOURCE_FILES_CPP "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.cpp" 
                                       "${CMAKE_CURRENT_SOURCE_DIR}/impl_glfw/*.cpp"
                                       "${CMAKE_CURRENT_SOURCE_DIR}/impl_vulkan/*.cpp")
    MESSAGE(STATUS "SOURCE .cpp files: ${SOURCE_FILES_CPP}")
    #SOURCE_FILES_C
    FILE(GLOB_RECURSE SOURCE_FILES_C "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.c"
                                     "${CMAKE_CURRENT_SOURCE_DIR}/impl_glfw/*.c"
                                     "${CMAKE_CURRENT_SOURCE_DIR}/impl_vulkan/*.c")
    MESSAGE(STATUS "SOURCE .c files: ${SOURCE_FILES_C}")

    #SOURCE_FILES_MM
    if (APPLE)
        FILE(GLOB_RECURSE SOURCE_FILES_MM "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.mm"
                                          "${CMAKE_CURRENT_SOURCE_DIR}/impl_glfw/*.mm"
                                          "${CMAKE_CURRENT_SOURCE_DIR}/impl_vulkan/*.mm")
        MESSAGE(STATUS "SOURCE .mm files: ${SOURCE_FILES_MM}")
    endif ()

    #LIB_INCLUDE
    if (WIN32)
        SET(LIB_INCLUDE "$ENV{VULKAN_SDK}/Include"
                        "${CMAKE_CURRENT_SOURCE_DIR}/../../Include/Windows/glfw-3.3.6")

    elseif (ANDROID) 


    elseif (APPLE)
        SET(LIB_INCLUDE "$ENV{VULKAN_HOME}/macOS/include"
                        "${CMAKE_CURRENT_SOURCE_DIR}/../../Include/MacOS/glfw-3.3.6")

        SET(SYS_FRAMEWORKS_DIR /System/Library/Frameworks)
        SET(COCOA_LIBS ${SYS_FRAMEWORKS_DIR}/Cocoa.framework)
        SET(COREVIDEO_LIBS ${SYS_FRAMEWORKS_DIR}/CoreVideo.framework)
        SET(IOKIT_LIBS ${SYS_FRAMEWORKS_DIR}/IOKit.framework)
        SET(SYS_VULKAN_LIBS "$ENV{VULKAN_HOME}/macOS/Frameworks/vulkan.framework")

    endif ()
    MESSAGE(STATUS "LIB_INCLUDE: ${LIB_INCLUDE}")
    INCLUDE_DIRECTORIES(${LIB_INCLUDE})

    #LIB_GLFW_DIR, LIB_VULKAN_DIR
    if (WIN32)
        SET(LIB_VULKAN_DIR "$ENV{VULKAN_SDK}/Lib")
        SET(LIB_GLFW_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../Lib/Windows/")

    elseif (ANDROID)  


    elseif (APPLE)
        SET(LIB_VULKAN_DIR "$ENV{VULKAN_HOME}/macOS/lib")
        SET(LIB_GLFW_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../Lib/MacOS/")

    endif ()
    MESSAGE(STATUS "LIB_GLFW_DIR dir: ${LIB_GLFW_DIR}")
    MESSAGE(STATUS "LIB_VULKAN_DIR dir: ${LIB_VULKAN_DIR}")

    #Build
    if (WIN32)
        MESSAGE(STATUS "Build ${PROJECT_NAME} platfrom Win32 !")
        ADD_DEFINITIONS(-DIMGUI_EXPORTS)
        if(MSVC)
            ADD_DEFINITIONS(/wd4251 /wd4275)
        endif()

        if (DEBUG)
            LINK_LIBRARIES(shlwapi.lib
                           ${LIB_GLFW_DIR}/glfw-3.3.6_d.lib 
                           ${LIB_VULKAN_DIR}/vulkan-1.lib)
        else ()
            LINK_LIBRARIES(shlwapi.lib
                           ${LIB_GLFW_DIR}/glfw-3.3.6.lib 
                           ${LIB_VULKAN_DIR}/vulkan-1.lib)
        endif ()
        
        MY_ADD_LIBRARY(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES_CPP} ${SOURCE_FILES_C})

    elseif (ANDROID)
        MESSAGE(STATUS "Build ${PROJECT_NAME} platfrom Android !")  


    elseif (APPLE)
        MESSAGE(STATUS "Build ${PROJECT_NAME} platfrom MacOS !")
        
        if (DEBUG)
            LINK_LIBRARIES(${LIB_GLFW_DIR}/libglfw-3.3.6_d.a)
        else ()
            LINK_LIBRARIES(${LIB_GLFW_DIR}/libglfw-3.3.6.a)
        endif ()
        
        ADD_LIBRARY(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES_CPP} ${SOURCE_FILES_C} ${SOURCE_FILES_MM})
        TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
                            ${LIB_VULKAN_DIR}/libvulkan.1.dylib 
                            ${LIB_VULKAN_DIR}/libvulkan.1.2.189.dylib 
                            ${SYS_VULKAN_LIBS})
        SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES BUNDLE TRUE)
        SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES ENABLE_EXPORTS ON)
        SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES CMAKE_ENABLE_EXPORTS TRUE)   
        SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup \
            -Wl,-no_pie \
            -Wl,-search_paths_first"
        )
        
    endif ()

elseif (${PLATFORM_MODE} STREQUAL ${PLATFORM_MODE_METAL}) 
    #HEADER_FILES
    FILE(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.h"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/impl_metal/*.h")
    MESSAGE(STATUS "HEADER files: ${HEADER_FILES}")
    #SOURCE_FILES_CPP
    FILE(GLOB_RECURSE SOURCE_FILES_CPP "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.cpp" 
                                       "${CMAKE_CURRENT_SOURCE_DIR}/impl_metal/*.cpp")
    MESSAGE(STATUS "SOURCE .cpp files: ${SOURCE_FILES_CPP}")
    #SOURCE_FILES_C
    FILE(GLOB_RECURSE SOURCE_FILES_C "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.c"
                                     "${CMAKE_CURRENT_SOURCE_DIR}/impl_metal/*.c")
    MESSAGE(STATUS "SOURCE .c files: ${SOURCE_FILES_C}")

    #SOURCE_FILES_MM
    if (APPLE)
    FILE(GLOB_RECURSE SOURCE_FILES_MM "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.mm"
                                      "${CMAKE_CURRENT_SOURCE_DIR}/impl_metal/*.mm")
    MESSAGE(STATUS "SOURCE .mm files: ${SOURCE_FILES_MM}")
 endif ()

else () 

endif ()