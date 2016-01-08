#
# Copyright (c) 2008-2015 the ALPHAENGINE project.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

# Find ALPHAENGINE include directories and library in source & build tree or installed ALPHAENGINE SDK.
#
#  ALPHAENGINE_FOUND
#  ALPHAENGINE_INCLUDE_DIRS
#  ALPHAENGINE_LIBRARIES
#  ALPHAENGINE_LIBRARIES_REL (WIN32 only)
#  ALPHAENGINE_LIBRARIES_DBG (WIN32 only)
#  ALPHAENGINE_DLL (WIN32 only)
#  ALPHAENGINE_DLL_REL (WIN32 only)
#  ALPHAENGINE_DLL_DBG (WIN32 only)
#
#
# For internal ALPHAENGINE project, the ALPHAENGINE "build tree" path is already known.
#
# For external project that attempts to use the ALPHAENGINE build tree or installed ALPHAENGINE SDK,
# use ALPHAENGINE_HOME environment variable or build option to specify the path (not needed when the path is a system-wide default location).
# When setting ALPHAENGINE_HOME variable, it should be set to a parent directory containing both the "include" or "lib" subdirectories.
# For example: set ALPHAENGINE_HOME=/home/john/usr/local, if the SDK is installed using DESTDIR=/home/john and CMAKE_INSTALL_PREFIX=/usr/local
#

if (ALPHAENGINE_FOUND)
    # All the subprojects should use the same ALPHAENGINE library, so only need to search on the first (sub)project that requires ALPHAENGINE library
    return ()
endif ()

# If the ALPHAENGINE_LIB_TYPE build option changes then invalidate the found library cache
if (NOT ALPHAENGINE_LIB_TYPE STREQUAL ALPHAENGINE_FOUND_LIB_TYPE)
    unset (ALPHAENGINE_LIBRARIES CACHE)
    set (ALPHAENGINE_FOUND_LIB_TYPE ${ALPHAENGINE_LIB_TYPE} CACHE INTERNAL "Lib type when ALPHAENGINE library was last found")

    # ALPHAENGINE prefers static library type by default while CMake prefers shared one, so we need to change CMake preference to agree with ALPHAENGINE
    if (NOT ALPHAENGINE_LIB_TYPE STREQUAL SHARED)
        list (REVERSE CMAKE_FIND_LIBRARY_SUFFIXES)
    endif ()
endif ()

# Cater for the shared library extension in Emscripten build has been changed to ".bc"
if (EMSCRIPTEN)
    string (REPLACE .so .bc CMAKE_FIND_LIBRARY_SUFFIXES "${CMAKE_FIND_LIBRARY_SUFFIXES}")   # Stringify for string replacement
endif ()


set (ALPHAENGINE_PATH_SUFFIX "AlphaEngine")

if (CMAKE_PROJECT_NAME STREQUAL Urho3D)
    	# Library location is already known to be in the build tree

	set (ALPHAENGINE_HOME ${CMAKE_BINARY_DIR})
	set (ALPHAENGINE_INCLUDE_DIRS  "${ALPHAENGINE_HOME}/include/${ALPHAENGINE_PATH_SUFFIX}")
   
        set (ALPHAENGINE_LIBRARIES AlphaEngine)
        set (FOUND_MESSAGE "Found ALPHAENGINE: as CMake target")
else ()
    # Library location would be searched (based on ALPHAENGINE_HOME variable if provided and in system-wide default location)
    if (NOT ALPHAENGINE_HOME AND DEFINED ENV{ALPHAENGINE_HOME})
        file (TO_CMAKE_PATH "$ENV{ALPHAENGINE_HOME}" ALPHAENGINE_HOME)
    endif ()
    # ALPHAENGINE_HOME variable should be an absolute path, so use a non-rooted search even when we are cross-compiling
    if (ALPHAENGINE_HOME)
        list (APPEND CMAKE_PREFIX_PATH ${ALPHAENGINE_HOME})
        set (SEARCH_OPT NO_CMAKE_FIND_ROOT_PATH)
    endif ()
    # For Android platform, search in path similar to ANDROID_LIBRARY_OUTPUT_PATH variable
    if (ANDROID)
        if (ALPHAENGINE_HOME)
            set (ALPHAENGINE_LIB_SEARCH_HINT HINTS ${ALPHAENGINE_HOME}/libs/${ANDROID_NDK_ABI_NAME})
        else ()
            set (ALPHAENGINE_LIB_SEARCH_HINT HINTS /usr/local/libs/${ANDROID_NDK_ABI_NAME})
        endif ()
    endif ()
    if (NOT ALPHAENGINE_64BIT)
        # For 32-bit, force to search in 'lib' path even when the host system defaulted to use 'lib64'
        set_property (GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS FALSE)
    elseif (WIN32)
        # For 64-bit, force to search in 'lib64' path even when the Windows platform is not defaulted to use it
        set_property (GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS TRUE)
    endif ()
    find_path (ALPHAENGINE_INCLUDE_DIRS EngineStd.h PATH_SUFFIXES ${PATH_SUFFIX} ${SEARCH_OPT})
    if (ALPHAENGINE_INCLUDE_DIRS)
        set (BASE_INCLUDE_DIR ${ALPHAENGINE_INCLUDE_DIRS})   # Preserve the base include dir because the original variable will be turned into a list below
        get_filename_component (PATH ${ALPHAENGINE_INCLUDE_DIRS} PATH)
        set (ALPHAENGINE_INCLUDE_DIRS ${PATH} )       
        if (NOT ALPHAENGINE_HOME)
            # ALPHAENGINE_HOME is not set when using SDK installed on system-wide default location, so set it now
            get_filename_component (PATH ${PATH} PATH)
            set (ALPHAENGINE_HOME ${PATH})
        endif ()
    endif ()	
    find_library (ALPHAENGINE_LIBRARIES NAMES EngineStd ${ALPHAENGINE_LIB_SEARCH_HINT} PATH_SUFFIXES ${PATH_SUFFIX} ${SEARCH_OPT})
    if (WIN32)
        # For Windows platform, give a second chance to search for a debug version of the library
        find_library (ALPHAENGINE_LIBRARIES_DBG NAMES EngineStd_d ${ALPHAENGINE_LIB_SEARCH_HINT} PATH_SUFFIXES ${PATH_SUFFIX} ${SEARCH_OPT})
        # If both the non-debug and debug version of the libraries are found then use them both
        set (ALPHAENGINE_LIBRARIES_REL ${ALPHAENGINE_LIBRARIES})
        # Otherwise, ALPHAENGINE_LIBRARIES variable should have the path to either one of the version
        if (ALPHAENGINE_LIBRARIES)
            if (ALPHAENGINE_LIBRARIES_DBG)
                list (APPEND ALPHAENGINE_LIBRARIES ${ALPHAENGINE_LIBRARIES_DBG})
            endif ()
        else ()
            set (ALPHAENGINE_LIBRARIES ${ALPHAENGINE_LIBRARIES_DBG})
        endif ()
        # For shared library type, also initialize the ALPHAENGINE_DLL variable for later use
        if (ALPHAENGINE_LIB_TYPE STREQUAL SHARED AND ALPHAENGINE_HOME)
            find_file (ALPHAENGINE_DLL_REL EngineStd.dll HINTS ${URHO3D_HOME}/bin NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
            if (ALPHAENGINE_DLL_REL)
                list (APPEND ALPHAENGINE_DLL ${ALPHAENGINE_DLL_REL})
            endif ()
            find_file (ALPHAENGINE_DLL_DBG EngineStd_d.dll HINTS ${URHO3D_HOME}/bin NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
            if (ALPHAENGINE_DLL_DBG)
                list (APPEND ALPHAENGINE_DLL ${ALPHAENGINE_DLL_DBG})
            endif ()
        endif ()
    endif ()
endif ()

if (ALPHAENGINE_INCLUDE_DIRS AND ALPHAENGINE_LIBRARIES)
    set (ALPHAENGINE_FOUND 1)
    if (NOT FOUND_MESSAGE)
        set (FOUND_MESSAGE "Found ALPHAENGINE: ${ALPHAENGINE_LIBRARIES}")
    endif ()
    include (FindPackageMessage)
    FIND_PACKAGE_MESSAGE (ALPHAENGINE ${FOUND_MESSAGE} "[${ALPHAENGINE_LIBRARIES}][${ALPHAENGINE_INCLUDE_DIRS}]")
    set (ALPHAENGINE_HOME ${ALPHAENGINE_HOME} CACHE PATH "Path to ALPHAENGINE build tree or SDK installation location (external project only)" FORCE)
elseif (ALPHAENGINE_FIND_REQUIRED)
    if (ANDROID)
        set (NOT_FOUND_MESSAGE "For Android platform, double check if you have specified to use the same ANDROID_ABI as the ALPHAENGINE library in the build tree or SDK.")
    endif ()
    message (FATAL_ERROR
        "Could not find ALPHAENGINE library in ALPHAENGINE build tree or SDK installation. "
        "Use ALPHAENGINE_HOME environment variable or build option to specify the location of the build tree or SDK installation. ${NOT_FOUND_MESSAGE}")
endif ()

mark_as_advanced (ALPHAENGINE_INCLUDE_DIRS ALPHAENGINE_LIBRARIES ALPHAENGINE_LIBRARIES_REL ALPHAENGINE_LIBRARIES_DBG ALPHAENGINE_DLL ALPHAENGINE_DLL_REL ALPHAENGINE_DLL_DBG ALPHAENGINE_HOME)



