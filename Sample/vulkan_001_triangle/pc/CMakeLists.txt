# author: LostPeter
# time:   2022-11-05

CMAKE_MINIMUM_REQUIRED(VERSION 3.4)
if (DEBUG)
    PROJECT(vulkan_001_triangle_d)
    SET(CMAKE_BUILD_TYPE "Debug")
    if (!WIN32)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    endif()

    MESSAGE(STATUS "Set Debug Build !")
else ()
    PROJECT(vulkan_001_triangle)
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


#LIB_INCLUDE
if (WIN32)
    SET(LIB_INCLUDE "$ENV{VULKAN_SDK}/Include"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Include/Windows/assimp-5.2.4"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Include/Windows/glfw-3.3.6"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Include/Windows/glm-0.9.9.8"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Include/Windows/imgui-1.85_glfw_vulkan"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Include/Windows/stb-0.02"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../VulkanUtil/Core/include"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../share/Core")

elseif (ANDROID)        


elseif (APPLE)
    SET(LIB_INCLUDE "$ENV{VULKAN_HOME}/macOS/include"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Include/MacOS/assimp-5.2.4"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Include/MacOS/glfw-3.3.6"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Include/MacOS/glm-0.9.9.8"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Include/MacOS/imgui-1.85_glfw_vulkan"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Include/MacOS/stb-0.02"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../../VulkanUtil/Core/include"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../share/Core")

    SET(SYS_FRAMEWORKS_DIR /System/Library/Frameworks)
    SET(COCOA_LIBS ${SYS_FRAMEWORKS_DIR}/Cocoa.framework)
    SET(COREVIDEO_LIBS ${SYS_FRAMEWORKS_DIR}/CoreVideo.framework)
    SET(IOKIT_LIBS ${SYS_FRAMEWORKS_DIR}/IOKit.framework)
    SET(SYS_VULKAN_LIBS "$ENV{VULKAN_HOME}/macOS/Frameworks/vulkan.framework")
   
endif ()
MESSAGE(STATUS "LIB_INCLUDE: ${LIB_INCLUDE}")
INCLUDE_DIRECTORIES(${LIB_INCLUDE})

#LIB_VULKAN_DIR, PLUGINS_DIR, STATIC_DIR
if (WIN32)
    SET(LIB_VULKAN_DIR "$ENV{VULKAN_SDK}/Lib")
    SET(LIB_DEPENDENCIES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Lib/Windows")
    SET(PLUGINS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../Plugins/Windows")
    SET(STATIC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../share/Lib/Windows")

elseif (ANDROID) 


elseif (APPLE)
    SET(LIB_VULKAN_DIR "$ENV{VULKAN_HOME}/macOS/lib")
    SET(LIB_DEPENDENCIES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../Dependencies/Lib/MacOS")
    SET(PLUGINS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../Plugins/MacOS")
    SET(STATIC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../share/Lib/MacOS")

endif ()
MESSAGE(STATUS "LIB_VULKAN_DIR dir: ${LIB_VULKAN_DIR}")
MESSAGE(STATUS "DEPENDENCIES dir: ${LIB_DEPENDENCIES_DIR}")
MESSAGE(STATUS "PLUGINS dir: ${PLUGINS_DIR}")
MESSAGE(STATUS "STATIC dir: ${STATIC_DIR}")

if (WIN32)
    MESSAGE(STATUS "Build ${PROJECT_NAME} platfrom Win32 !")
    
    if(MSVC)
        ADD_DEFINITIONS(/wd4251 /wd4275)
    endif()
    if (DEBUG)
        LINK_LIBRARIES(shlwapi.lib 
                       ${LIB_VULKAN_DIR}/vulkan-1.lib
                       ${LIB_DEPENDENCIES_DIR}/assimp-5.2.4_d.lib
                       ${LIB_DEPENDENCIES_DIR}/glfw-3.3.6_d.lib
                       ${LIB_DEPENDENCIES_DIR}/imgui-1.85_glfw_vulkan_d.lib
                       ${STATIC_DIR}/Share_d.lib 
                       ${PLUGINS_DIR}/VulkanUtil_d.lib)
    else ()
        LINK_LIBRARIES(shlwapi.lib 
                       ${LIB_VULKAN_DIR}/vulkan-1.lib
                       ${LIB_DEPENDENCIES_DIR}/assimp-5.2.4.lib
                       ${LIB_DEPENDENCIES_DIR}/glfw-3.3.6.lib
                       ${LIB_DEPENDENCIES_DIR}/imgui-1.85_glfw_vulkan.lib
                       ${STATIC_DIR}/Share.lib 
                       ${PLUGINS_DIR}/VulkanUtil.lib)
    endif()
    ADD_EXECUTABLE(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES_CPP})

elseif (ANDROID)
    MESSAGE(STATUS "Build ${PROJECT_NAME} platfrom Android !")


elseif (APPLE)
    MESSAGE(STATUS "Build ${PROJECT_NAME} platfrom MacOS !")
    
    ADD_EXECUTABLE(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES_CPP} ${SOURCE_FILES_MM})
    if (DEBUG)
        TARGET_LINK_LIBRARIES(${PROJECT_NAME}
                              ${COCOA_LIBS}
                              ${COREVIDEO_LIBS}
                              ${IOKIT_LIBS}
                              ${LIB_VULKAN_DIR}/libvulkan.1.dylib
                              ${LIB_VULKAN_DIR}/libvulkan.1.2.189.dylib
                              ${SYS_VULKAN_LIBS}
                              ${PLUGINS_DIR}/libVulkanUtil_d.dylib
                              ${STATIC_DIR}/libShare_d.dylib)
    else ()
        TARGET_LINK_LIBRARIES(${PROJECT_NAME}
                              ${COCOA_LIBS}
                              ${COREVIDEO_LIBS}
                              ${IOKIT_LIBS}
                              ${LIB_VULKAN_DIR}/libvulkan.1.dylib
                              ${LIB_VULKAN_DIR}/libvulkan.1.2.189.dylib
                              ${SYS_VULKAN_LIBS}
                              ${PLUGINS_DIR}/libVulkanUtil.dylib
                              ${STATIC_DIR}/libShare.dylib)
    endif()
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup \
        -Wl,-no_pie \
        -Wl,-search_paths_first"
    )
    
endif ()