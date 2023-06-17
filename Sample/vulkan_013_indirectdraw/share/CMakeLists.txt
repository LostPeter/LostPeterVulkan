#****************************************************************************
# LostPeterVulkan - Copyright (C) 2022 by LostPeter
# 
# Author:   LostPeter
# Time:     2023-04-01
# Github:   https://github.com/LostPeter/LostPeterVulkan
# Document: https://www.zhihu.com/people/lostpeter/posts
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

CMAKE_MINIMUM_REQUIRED(VERSION 3.4)
SET(NAME_PROJECT "share_vulkan_013_indirectdraw")
if (DEBUG)
    PROJECT(${NAME_PROJECT}_d)
    SET(CMAKE_BUILD_TYPE "Debug")
    if (!WIN32)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    endif()

    MESSAGE(STATUS "Set Debug Build !")
else ()
    PROJECT(${NAME_PROJECT})
endif ()

SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_STANDARD 11)

#MESSAGE(STATUS "HOME dir: $ENV{HOME}")
MESSAGE(STATUS "Binary dir: ${PROJECT_BINARY_DIR}")
MESSAGE(STATUS "SOURCE dir: ${PROJECT_SOURCE_DIR}")
MESSAGE(STATUS "SYSTEM name: ${CMAKE_SYSTEM_NAME}")
MESSAGE(STATUS "SYSTEM version: ${CMAKE_SYSTEM_VERSION}")
MESSAGE(STATUS "SYSTEM processor: ${CMAKE_SYSTEM_PROCESSOR}")

#HEADER_FILES
FILE(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
MESSAGE(STATUS "HEADER files: ${HEADER_FILES}")

#SOURCE_FILES_CPP
FILE(GLOB_RECURSE SOURCE_FILES_CPP "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
MESSAGE(STATUS "SOURCE .cpp files: ${SOURCE_FILES_CPP}")

#SOURCE_FILES_C
FILE(GLOB_RECURSE SOURCE_FILES_C "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
MESSAGE(STATUS "SOURCE .c files: ${SOURCE_FILES_C}")

#SOURCE_FILES_MM
if (APPLE)
    FILE(GLOB_RECURSE SOURCE_FILES_MM "${CMAKE_CURRENT_SOURCE_DIR}/*.mm")
    MESSAGE(STATUS "SOURCE .mm files: ${SOURCE_FILES_MM}")
endif ()

#LIB_INCLUDE
if (WIN32)
    SET(LIB_INCLUDE "$ENV{VULKAN_SDK}/Include"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Include/Windows/assimp-5.2.4"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Include/Windows/glfw-3.3.6"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Include/Windows/glm-0.9.9.8"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Include/Windows/imgui-1.85_glfw_vulkan"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Include/Windows/libnoise-1.0.0"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/include/Windows/rapidxml-1.13"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Include/Windows/spdlog-1.10.0"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Include/Windows/stb-0.02"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Include/Windows/tinyxml-2.6.2"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Include/Windows/zlib-1.2.11"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../LostPeterVulkan/LostPeterFoundation/include"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../LostPeterVulkan/LostPeterUI/include"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../LostPeterVulkan/LostPeterVulkan/include")

elseif (ANDROID)     


elseif (APPLE)
    SET(LIB_INCLUDE "$ENV{VULKAN_HOME}/macOS/include"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Include/MacOS/assimp-5.2.4"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Include/MacOS/glfw-3.3.6"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Include/MacOS/glm-0.9.9.8"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Include/MacOS/imgui-1.85_glfw_vulkan"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Include/MacOS/libnoise-1.0.0"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/include/MacOS/rapidxml-1.13"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Include/MacOS/spdlog-1.10.0"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Include/MacOS/stb-0.02"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Include/MacOS/tinyxml-2.6.2"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Include/MacOS/zlib-1.2.11"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../LostPeterVulkan/LostPeterFoundation/include"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../LostPeterVulkan/LostPeterUI/include"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../LostPeterVulkan/LostPeterVulkan/include")

    SET(SYS_FRAMEWORKS_DIR /System/Library/Frameworks)
    SET(COCOA_LIBS ${SYS_FRAMEWORKS_DIR}/Cocoa.framework)
    SET(COREVIDEO_LIBS ${SYS_FRAMEWORKS_DIR}/CoreVideo.framework)
    SET(IOKIT_LIBS ${SYS_FRAMEWORKS_DIR}/IOKit.framework)
    SET(SYS_VULKAN_LIBS "$ENV{VULKAN_HOME}/macOS/Frameworks/vulkan.framework")

endif ()
MESSAGE(STATUS "LIB_INCLUDE: ${LIB_INCLUDE}")
INCLUDE_DIRECTORIES(${LIB_INCLUDE})

#LIB_VULKAN_DIR, LIB_PLUGINS_DIR
if (WIN32)
    SET(LIB_VULKAN_DIR "$ENV{VULKAN_SDK}/Lib")
    SET(LIB_DEPENDENCIES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Lib/Windows")
    SET(LIB_PLUGINS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../Plugins/Windows")

elseif (ANDROID)  


elseif (APPLE)
    SET(LIB_VULKAN_DIR "$ENV{VULKAN_HOME}/macOS/lib")
    SET(LIB_DEPENDENCIES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Lib/MacOS")
    SET(LIB_PLUGINS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../Plugins/MacOS")

endif ()
MESSAGE(STATUS "LIB_VULKAN_DIR dir: ${LIB_VULKAN_DIR}")
MESSAGE(STATUS "LIB_DEPENDENCIES_DIR dir: ${LIB_DEPENDENCIES_DIR}")
MESSAGE(STATUS "LIB_PLUGINS_DIR dir: ${LIB_PLUGINS_DIR}")

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

if (WIN32)
    MESSAGE(STATUS "Build ${PROJECT_NAME} platfrom Win32 !")
    
    ADD_DEFINITIONS(-DNOISE_STATIC)
    if(MSVC)
        ADD_DEFINITIONS(/wd4251 /wd4275)
    endif()
    if (DEBUG)
        LINK_LIBRARIES(shlwapi.lib 
                       ${LIB_VULKAN_DIR}/vulkan-1.lib
                       ${LIB_DEPENDENCIES_DIR}/assimp-5.2.4_d.lib
                       ${LIB_DEPENDENCIES_DIR}/glfw-3.3.6_d.lib
                       ${LIB_DEPENDENCIES_DIR}/imgui-1.85_glfw_vulkan_d.lib
                       ${LIB_DEPENDENCIES_DIR}/libnoise-1.0.0_d.lib
                       ${LIB_DEPENDENCIES_DIR}/libnoiseutils-1.0.0_d.lib
                       ${LIB_DEPENDENCIES_DIR}/tinyxml-2.6.2_d.lib
                       ${LIB_DEPENDENCIES_DIR}/zlib-1.2.11_d.lib
                       ${LIB_PLUGINS_DIR}/LostPeterFoundation_d.lib
                       ${LIB_PLUGINS_DIR}/LostPeterUI_d.lib
                       ${LIB_PLUGINS_DIR}/LostPeterVulkan_d.lib)
    else ()
        LINK_LIBRARIES(shlwapi.lib 
                       ${LIB_VULKAN_DIR}/vulkan-1.lib
                       ${LIB_DEPENDENCIES_DIR}/assimp-5.2.4.lib
                       ${LIB_DEPENDENCIES_DIR}/glfw-3.3.6.lib
                       ${LIB_DEPENDENCIES_DIR}/imgui-1.85_glfw_vulkan.lib
                       ${LIB_DEPENDENCIES_DIR}/libnoise-1.0.0.lib
                       ${LIB_DEPENDENCIES_DIR}/libnoiseutils-1.0.0.lib
                       ${LIB_DEPENDENCIES_DIR}/tinyxml-2.6.2.lib
                       ${LIB_DEPENDENCIES_DIR}/zlib-1.2.11.lib
                       ${LIB_PLUGINS_DIR}/LostPeterFoundation.lib
                       ${LIB_PLUGINS_DIR}/LostPeterUI.lib
                       ${LIB_PLUGINS_DIR}/LostPeterVulkan.lib)
    endif ()
    MY_ADD_LIBRARY(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES_CPP} ${SOURCE_FILES_C})

elseif (ANDROID)
    MESSAGE(STATUS "Build ${PROJECT_NAME} platfrom Android !")  


elseif (APPLE)
    MESSAGE(STATUS "Build ${PROJECT_NAME} platfrom MacOS !")

    ADD_LIBRARY(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES_CPP} ${SOURCE_FILES_C} ${SOURCE_FILES_MM})
    if (DEBUG)
        TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
                              ${LIB_PLUGINS_DIR}/libLostPeterFoundation_d.dylib
                              ${LIB_PLUGINS_DIR}/libLostPeterUI_d.dylib
                              ${LIB_PLUGINS_DIR}/libLostPeterVulkan_d.dylib)
    else ()
        TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
                              ${LIB_PLUGINS_DIR}/libLostPeterFoundation.dylib
                              ${LIB_PLUGINS_DIR}/libLostPeterUI.dylib
                              ${LIB_PLUGINS_DIR}/libLostPeterVulkan.dylib)
    endif ()
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES BUNDLE TRUE)
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES ENABLE_EXPORTS ON)
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup \
        -Wl,-no_pie \
        -Wl,-search_paths_first"
    )

endif ()