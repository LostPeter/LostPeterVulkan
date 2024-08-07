# cmake build file for squish
# by Stefan Roettger (snroettg@gmail.com)
# updated by Simon Brown (si@sjbrown.co.uk)

# features:
#   uses -fopenmp when available
#    use BUILD_SQUISH_WITH_OPENMP to override
#   Xcode: builds universal binaries, uses SSE2 on i386 and Altivec on ppc
#   Unix and VS: SSE2 support is enabled by default
#    use BUILD_SQUISH_WITH_SSE2 and BUILD_SQUISH_WITH_ALTIVEC to override

PROJECT(squish)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

OPTION(BUILD_SQUISH_WITH_OPENMP "Build with OpenMP." ON)

OPTION(BUILD_SQUISH_WITH_SSE2 "Build with SSE2." ON)
OPTION(BUILD_SQUISH_WITH_ALTIVEC "Build with Altivec." OFF)

OPTION(BUILD_SHARED_LIBS "Build shared libraries." OFF)

OPTION(BUILD_SQUISH_EXTRA "Build extra source code." OFF)

IF (BUILD_SQUISH_WITH_OPENMP)
   FIND_PACKAGE(OpenMP)
   IF (OPENMP_FOUND)
       SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
       ADD_DEFINITIONS(-DSQUISH_USE_OPENMP)
   ENDIF()
ENDIF()

IF (CMAKE_GENERATOR STREQUAL "Xcode")
    SET(CMAKE_OSX_ARCHITECTURES "i386;ppc")
ELSE (CMAKE_GENERATOR STREQUAL "Xcode")
    IF (BUILD_SQUISH_WITH_SSE2 AND NOT WIN32)
        ADD_DEFINITIONS(-DSQUISH_USE_SSE=2 -msse2)
    ENDIF (BUILD_SQUISH_WITH_SSE2 AND NOT WIN32)
    IF (BUILD_SQUISH_WITH_ALTIVEC AND NOT WIN32)
        ADD_DEFINITIONS(-DSQUISH_USE_ALTIVEC=1 -maltivec)
    ENDIF (BUILD_SQUISH_WITH_ALTIVEC AND NOT WIN32)
ENDIF (CMAKE_GENERATOR STREQUAL "Xcode")

SET(SQUISH_HDRS
    squish.h
    )

SET(SQUISH_SRCS
    alpha.cpp
    alpha.h
    clusterfit.cpp
    clusterfit.h
    colourblock.cpp
    colourblock.h
    colourfit.cpp
    colourfit.h
    colourset.cpp
    colourset.h
    maths.cpp
    maths.h
    rangefit.cpp
    rangefit.h
    simd.h
    simd_float.h
    simd_sse.h
    simd_ve.h
    singlecolourfit.cpp
    singlecolourfit.h
    singlecolourlookup.inl
    squish.cpp
    )

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

ADD_LIBRARY(squish ${SQUISH_SRCS} ${SQUISH_HDRS})

SET_TARGET_PROPERTIES(
    squish PROPERTIES
    PUBLIC_HEADER "${SQUISH_HDRS}"
    VERSION 0.0
    SOVERSION 0.0
    DEBUG_POSTFIX "d"
    XCODE_ATTRIBUTE_GCC_PREPROCESSOR_DEFINITIONS "$(SQUISH_CPP_$(CURRENT_ARCH))"
    XCODE_ATTRIBUTE_OTHER_CFLAGS "$(SQUISH_CFLAGS_$(CURRENT_ARCH))"
    XCODE_ATTRIBUTE_SQUISH_CPP_i386 "SQUISH_USE_SSE=2"
    XCODE_ATTRIBUTE_SQUISH_CFLAGS_i386 ""
    XCODE_ATTRIBUTE_SQUISH_CPP_ppc "SQUISH_USE_ALTIVEC=1"
    XCODE_ATTRIBUTE_SQUISH_CFLAGS_ppc "-maltivec"
    )

IF (BUILD_SQUISH_EXTRA)
    SET(SQUISHTEST_SRCS extra/squishtest.cpp)

    ADD_EXECUTABLE(squishtest ${SQUISHTEST_SRCS})
    SET_TARGET_PROPERTIES(squishtest PROPERTIES DEBUG_POSTFIX "d")
    TARGET_LINK_LIBRARIES(squishtest squish)

    SET(SQUISHPNG_SRCS extra/squishpng.cpp)

    FIND_PACKAGE(PNG)

    IF (PNG_FOUND)
        SET(CMAKE_PLATFORM_IMPLICIT_INCLUDE_DIRECTORIES)
        INCLUDE_DIRECTORIES(${PNG_INCLUDE_DIR})
        ADD_EXECUTABLE(squishpng ${SQUISHPNG_SRCS})
        SET_TARGET_PROPERTIES(squishpng PROPERTIES DEBUG_POSTFIX "d")
        TARGET_LINK_LIBRARIES(squishpng squish ${PNG_LIBRARIES})
    ENDIF (PNG_FOUND)
ENDIF (BUILD_SQUISH_EXTRA)

INSTALL(
    TARGETS squish
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include
    )
