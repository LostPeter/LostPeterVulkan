CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
if (DEBUG)
    PROJECT(zthread-2.3.2_d)
    SET(CMAKE_BUILD_TYPE "Debug")
    if (!WIN32)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    endif()

    MESSAGE(STATUS "Set Debug Build !")
else ()
    PROJECT(zthread-2.3.2)
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

if (ANDROID) 
    #HEADER_FILES
    FILE(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/zthread/*.h" 
                           "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
                           "${CMAKE_CURRENT_SOURCE_DIR}/src/vanilla/*.h"
                           "${CMAKE_CURRENT_SOURCE_DIR}/src/linux/*.h")
    MESSAGE(STATUS "HEADER files: ${HEADER_FILES}")

    #SOURCE_FILES
    FILE(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx"
                           "${CMAKE_CURRENT_SOURCE_DIR}/src/vanilla/*.cxx"
                           "${CMAKE_CURRENT_SOURCE_DIR}/src/linux/*.cxx")
    MESSAGE(STATUS "SOURCE .cpp files: ${SOURCE_FILES}")

    #LIB_INCLUDE
    if (WIN32)
        SET(LIB_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")
    else ()
        SET(CMAKE_FIND_ROOT_PATH ${ANDROID_NDK})
        SET(LIB_INCLUDE ${ANDROID_NDK}/sysroot/usr/include
                        ${ANDROID_NDK}/sources/cxx-stl/llvm-libc++/include
                        "${CMAKE_CURRENT_SOURCE_DIR}/include")
    endif ()
    MESSAGE(STATUS "LIB_INCLUDE: ${LIB_INCLUDE}")
    INCLUDE_DIRECTORIES(${LIB_INCLUDE})

    #LIB
    

    #BUILD
    if (ANDROID_ARMV7A)
        MESSAGE(STATUS "Build ${PROJECT_NAME} platfrom Android armv7a !")
    elseif (ANDROID_ARMV8A)
        MESSAGE(STATUS "Build ${PROJECT_NAME} platfrom Android armv8a !")
    else ()
        MESSAGE(FATAL_ERROR "Build ${PROJECT_NAME} platfrom Android error, Unknown arch !")
    endif ()
    
    ADD_DEFINITIONS("--sysroot=${ANDROID_NDK}/sysroot -Wall")
    ADD_LIBRARY(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})
    LINK_DIRECTORIES(${ANDROID_NDK}/sysroot/usr/lib/aarch64-linux-android)

elseif (WIN32)
    #HEADER_FILES
    FILE(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/zthread/*.h" 
                           "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
                           "${CMAKE_CURRENT_SOURCE_DIR}/src/vanilla/*.h"
                           "${CMAKE_CURRENT_SOURCE_DIR}/src/win32/*.h")
    MESSAGE(STATUS "HEADER files: ${HEADER_FILES}")

    #SOURCE_FILES
    FILE(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx"
                           "${CMAKE_CURRENT_SOURCE_DIR}/src/vanilla/*.cxx"
                           "${CMAKE_CURRENT_SOURCE_DIR}/src/win32/*.cxx")
    MESSAGE(STATUS "SOURCE .cpp files: ${SOURCE_FILES}")

    #LIB_INCLUDE
    SET(LIB_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")
    MESSAGE(STATUS "LIB_INCLUDE: ${LIB_INCLUDE}")
    INCLUDE_DIRECTORIES(${LIB_INCLUDE})

    #LIB

    #BUILD
    MESSAGE(STATUS "Build ${PROJECT_NAME} platfrom Win32 !")

    MY_ADD_LIBRARY(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

elseif (APPLE)
    #HEADER_FILES
    FILE(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/zthread/*.h" 
                           "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
                           "${CMAKE_CURRENT_SOURCE_DIR}/src/vanilla/*.h"
                           "${CMAKE_CURRENT_SOURCE_DIR}/src/macos/*.h")
    MESSAGE(STATUS "HEADER files: ${HEADER_FILES}")

    #SOURCE_FILES
    FILE(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx"
                           "${CMAKE_CURRENT_SOURCE_DIR}/src/vanilla/*.cxx"
                           "${CMAKE_CURRENT_SOURCE_DIR}/src/macos/*.cxx")
    MESSAGE(STATUS "SOURCE .cpp files: ${SOURCE_FILES}")

    #LIB_INCLUDE
    SET(LIB_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")
    MESSAGE(STATUS "LIB_INCLUDE: ${LIB_INCLUDE}")
    INCLUDE_DIRECTORIES(${LIB_INCLUDE})

    #LIB
    SET(SYS_FRAMEWORKS_DIR /System/Library/Frameworks)
    SET(CARBON_LIBS ${SYS_FRAMEWORKS_DIR}/Carbon.framework)

    #BUILD
    MESSAGE(STATUS "Build ${PROJECT_NAME} platfrom MacOS !")

    ADD_LIBRARY(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${CARBON_LIBS})
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES BUNDLE TRUE)
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES ENABLE_EXPORTS ON)
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES CMAKE_ENABLE_EXPORTS TRUE)   
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup \
        -Wl,-search_paths_first"
    )

endif ()