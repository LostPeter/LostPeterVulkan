#****************************************************************************
# LostPeterFoundation - Copyright (C) 2022 by LostPeter
# 
# Author:   LostPeter
# Time:     2023-06-01
# Github:   https://github.com/LostPeter/LostPeterVulkan
# Document: https://www.zhihu.com/people/lostpeter/posts
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

CMAKE_MINIMUM_REQUIRED(VERSION 3.4)
SET(NAME_PROJECT "LostPeterFoundation")
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
FILE(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
MESSAGE(STATUS "HEADER files: ${HEADER_FILES}")

#SOURCE_FILES_CPP
FILE(GLOB_RECURSE SOURCE_FILES_CPP "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
MESSAGE(STATUS "SOURCE .cpp files: ${SOURCE_FILES_CPP}")

#SOURCE_FILES_C
FILE(GLOB_RECURSE SOURCE_FILES_C "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
MESSAGE(STATUS "SOURCE .c files: ${SOURCE_FILES_C}")

#SOURCE_FILES_MM
if (APPLE)
    FILE(GLOB_RECURSE SOURCE_FILES_MM "${CMAKE_CURRENT_SOURCE_DIR}/src/*.mm")
    MESSAGE(STATUS "SOURCE .mm files: ${SOURCE_FILES_MM}")
endif ()

#LIB_INCLUDE
if (WIN32)
    SET(LIB_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/../../Dependencies/Include/Windows/assimp-5.2.4"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../Dependencies/Include/Windows/glm-0.9.9.8"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../Dependencies/Include/Windows/libnoise-1.0.0"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../Dependencies/include/Windows/rapidxml-1.13"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../Dependencies/Include/Windows/spdlog-1.10.0"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../Dependencies/Include/Windows/stb-0.02"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../Dependencies/Include/Windows/tinyxml-2.6.2"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../Dependencies/Include/Windows/zlib-1.2.11")

elseif (ANDROID) 


elseif (APPLE)
    SET(LIB_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/../../Dependencies/Include/MacOS/assimp-5.2.4"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../Dependencies/Include/MacOS/glm-0.9.9.8"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../Dependencies/Include/MacOS/libnoise-1.0.0"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../Dependencies/include/MacOS/rapidxml-1.13"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../Dependencies/Include/MacOS/spdlog-1.10.0"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../Dependencies/Include/MacOS/stb-0.02"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../Dependencies/Include/MacOS/tinyxml-2.6.2"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../Dependencies/Include/MacOS/zlib-1.2.11")

    SET(SYS_FRAMEWORKS_DIR /System/Library/Frameworks)
    SET(CORE_FOUNDATION_LIBS ${SYS_FRAMEWORKS_DIR}/CoreFoundation.framework)
    SET(COCOA_LIBS ${SYS_FRAMEWORKS_DIR}/Cocoa.framework)
    SET(CORE_SERVICES_LIBS ${SYS_FRAMEWORKS_DIR}/CoreServices.framework)
    SET(CORE_VIDEO_LIBS ${SYS_FRAMEWORKS_DIR}/CoreVideo.framework)
    SET(QUARTZ_CORE_LIBS ${SYS_FRAMEWORKS_DIR}/QuartzCore.framework)
    SET(IOKIT_LIBS ${SYS_FRAMEWORKS_DIR}/IOKit.framework)

endif ()
MESSAGE(STATUS "LIB_INCLUDE: ${LIB_INCLUDE}")
INCLUDE_DIRECTORIES(${LIB_INCLUDE})

#LIB_DEPENDENCIES_DIR
if (WIN32)
    SET(LIB_DEPENDENCIES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../Dependencies/Lib/Windows") 

elseif (ANDROID)  


elseif (APPLE)
    SET(LIB_DEPENDENCIES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../Dependencies/Lib/MacOS")

endif ()
MESSAGE(STATUS "LIB_DEPENDENCIES_DIR dir: ${LIB_DEPENDENCIES_DIR}")


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
    
    ADD_DEFINITIONS(-DFOUNDATION_EXPORTS)
    ADD_DEFINITIONS(-DNOISE_STATIC)
    if(MSVC)
        ADD_DEFINITIONS(/wd4251 /wd4275)
    endif()
    if (DEBUG)
        LINK_LIBRARIES(shlwapi.lib
                       ${LIB_DEPENDENCIES_DIR}/assimp-5.2.4_d.lib
                       ${LIB_DEPENDENCIES_DIR}/libnoise-1.0.0_d.lib
                       ${LIB_DEPENDENCIES_DIR}/libnoiseutils-1.0.0_d.lib
                       ${LIB_DEPENDENCIES_DIR}/tinyxml-2.6.2_d.lib
                       ${LIB_DEPENDENCIES_DIR}/zlib-1.2.11_d.lib)
        
    else ()
        LINK_LIBRARIES(shlwapi.lib
                       ${LIB_DEPENDENCIES_DIR}/assimp-5.2.4.lib
                       ${LIB_DEPENDENCIES_DIR}/libnoise-1.0.0.lib
                       ${LIB_DEPENDENCIES_DIR}/libnoiseutils-1.0.0.lib
                       ${LIB_DEPENDENCIES_DIR}/tinyxml-2.6.2.lib
                       ${LIB_DEPENDENCIES_DIR}/zlib-1.2.11.lib)
        
    endif()
    MY_ADD_LIBRARY(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES_CPP} ${SOURCE_FILES_C})
    
elseif (ANDROID)
    MESSAGE(STATUS "Build ${PROJECT_NAME} platfrom Android !")  


elseif (APPLE)
    MESSAGE(STATUS "Build ${PROJECT_NAME} platfrom MacOS !")

    if (DEBUG)
        LINK_LIBRARIES(${LIB_DEPENDENCIES_DIR}/libassimp-5.2.4_d.a
                       ${LIB_DEPENDENCIES_DIR}/libnoise-1.0.0_d.a
                       ${LIB_DEPENDENCIES_DIR}/libnoiseutils-1.0.0_d.a
                       ${LIB_DEPENDENCIES_DIR}/libtinyxml-2.6.2_d.a
                       ${LIB_DEPENDENCIES_DIR}/libzlib-1.2.11_d.a)
    else ()
        LINK_LIBRARIES(${LIB_DEPENDENCIES_DIR}/libassimp-5.2.4.a
                       ${LIB_DEPENDENCIES_DIR}/libnoise-1.0.0.a
                       ${LIB_DEPENDENCIES_DIR}/libnoiseutils-1.0.0.a
                       ${LIB_DEPENDENCIES_DIR}/libtinyxml-2.6.2.a
                       ${LIB_DEPENDENCIES_DIR}/libzlib-1.2.11.a)
    endif()
    ADD_LIBRARY(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES_CPP} ${SOURCE_FILES_C} ${SOURCE_FILES_MM})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}
                          ${CORE_FOUNDATION_LIBS}
                          ${COCOA_LIBS}
                          ${CORE_SERVICES_LIBS}
                          ${CORE_VIDEO_LIBS}
                          ${QUARTZ_CORE_LIBS}
                          ${IOKIT_LIBS})
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES BUNDLE TRUE)
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES ENABLE_EXPORTS ON)
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES CMAKE_ENABLE_EXPORTS TRUE)   
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup -flat_namespace \
        -Wl,-no_pie \
        -Wl,-search_paths_first"
    )
    
endif ()