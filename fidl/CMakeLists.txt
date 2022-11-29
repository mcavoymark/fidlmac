cmake_minimum_required(VERSION 3.24.2)

project(fidl)

FIND_PACKAGE(Qt5Core)
IF (Qt5Core_FOUND)
    SET(QT_FOUND TRUE)
    INCLUDE_DIRECTORIES(${Qt5Core_INCLUDE_DIRS})
    SET(LIBS ${LIBS} Qt5::Core)
    #whatever that means
    ADD_DEFINITIONS(-DCIFTILIB_USE_QT)
    SET(CIFTILIB_PKGCONFIG_REQUIRES_LINE "Requires: Qt5Core")
    SET(CIFTILIB_PKGCONFIG_DEFINE "-DCIFTILIB_USE_QT")
ENDIF (Qt5Core_FOUND)

#boost
INCLUDE_DIRECTORIES(/opt/local/include)

#zlib, useful for volume reading
FIND_PACKAGE(ZLIB)
IF (ZLIB_FOUND)
    INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIRS})
    SET(LIBS ${LIBS} ${ZLIB_LIBRARIES})
    ADD_DEFINITIONS("-DCIFTILIB_HAVE_ZLIB")
ENDIF (ZLIB_FOUND)
#OS X has some weirdness in its zlib, so let the preprocessor know
IF (APPLE)
    ADD_DEFINITIONS(-DCIFTILIB_OS_MACOSX)
ENDIF (APPLE)

#cifti
#SET(C0 "/Users/mcavoy/Documents/FIDL/cifti/pkg/include/CiftiLib")
SET(C0 "/Users/mcavoy/Documents/FIDL/cifti/CiftiLib-1.6.0/src")
INCLUDE_DIRECTORIES(${C0} ${C0}/Nifti ${C0}/Common ${C0}/Cifti)
add_library(Cifti STATIC IMPORTED)
#set_property(TARGET Cifti PROPERTY IMPORTED_LOCATION /Users/mcavoy/Documents/FIDL/cifti/pkg/lib/libCifti.a)
set_property(TARGET Cifti PROPERTY IMPORTED_LOCATION /Users/mcavoy/Documents/FIDL/cifti/build/libCifti.a)

SET(CMAKE_CXX_FLAGS "-W -Wall -Wextra -O3 -std=c++20")
SET(CMAKE_C_FLAGS "-W -Wall -Wextra -O3 -std=c17")

#openmp provides a fast mutex implementation, faster than QT (and probably faster than glibmm)
FIND_PACKAGE(OpenMP)
IF (OPENMP_FOUND)

    #START221122 need omp.h
    #INCLUDE_DIRECTORIES(${OPENMP_INCLUDE_DIRS})
    INCLUDE_DIRECTORIES(/usr/local/Cellar/libomp/14.0.6/include)

    #START221122 need _omp_destroy_lock etc. 
    #SET(LIBS ${LIBS} ${(OPENMP_LIBRARIES})

    add_library(Openmp STATIC IMPORTED)
    set_property(TARGET Openmp PROPERTY IMPORTED_LOCATION /usr/local/Cellar/libomp/14.0.6/lib/libomp.a)


#libomp.a        libomp.dylib
#mcavoy@3452-AD-05001 lib % pwd
#/usr/local/Cellar/libomp/14.0.6/lib


    SET(CMAKE_CXX_FLAGS "${OpenMP_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")
ENDIF (OPENMP_FOUND)


SET(L0 "/Users/mcavoy/Documents/FIDL/fidl/src/c")

ADD_EXECUTABLE(fidl_vox_ass 
${L0}/fidl_vox_ass.cxx
${L0}/file.cxx
${L0}/stack.cxx
${L0}/header.cxx
${L0}/nifti_flipslice.cxx

#${L0}/filetype.cxx
${L0}/filetype.c

${L0}/subs_util.c
${L0}/subs_nifti.cxx
${L0}/shouldiswap.c
${L0}/cifti_getDimensions.cxx
${L0}/interfile.c
${L0}/strutil.c
${L0}/d2charvar.c
${L0}/nifti_gethdr.cxx)

TARGET_LINK_LIBRARIES(fidl_vox_ass Cifti Openmp ${LIBS})

#https://cmake.org/cmake/help/latest/guide/tutorial/A%20Basic%20Starting%20Point.html
