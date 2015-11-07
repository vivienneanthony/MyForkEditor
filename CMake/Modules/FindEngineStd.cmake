#
# Copyright (c) 2008-2015 the ENGINESTD project.
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

# Find ENGINESTD include directories and library in source & build tree or installed ENGINESTD SDK.
#
#  ENGINESTD_FOUND
#  ENGINESTD_INCLUDE_DIRS
#  ENGINESTD_LIBRARIES
#  ENGINESTD_LIBRARIES_REL (WIN32 only)
#  ENGINESTD_LIBRARIES_DBG (WIN32 only)
#  ENGINESTD_DLL (WIN32 only)
#  ENGINESTD_DLL_REL (WIN32 only)
#  ENGINESTD_DLL_DBG (WIN32 only)
#
#
# For internal ENGINESTD project, the ENGINESTD "build tree" path is already known.
#
# For external project that attempts to use the ENGINESTD build tree or installed ENGINESTD SDK,
# use ENGINESTD_HOME environment variable or build option to specify the path (not needed when the path is a system-wide default location).
# When setting ENGINESTD_HOME variable, it should be set to a parent directory containing both the "include" or "lib" subdirectories.
# For example: set ENGINESTD_HOME=/home/john/usr/local, if the SDK is installed using DESTDIR=/home/john and CMAKE_INSTALL_PREFIX=/usr/local
#

if (ENGINESTD_FOUND)
    # All the subprojects should use the same ENGINESTD library, so only need to search on the first (sub)project that requires ENGINESTD library
    return ()
endif ()

# If the ENGINESTD_LIB_TYPE build option changes then invalidate the found library cache
if (NOT ENGINESTD_LIB_TYPE STREQUAL ENGINESTD_FOUND_LIB_TYPE)
    unset (ENGINESTD_LIBRARIES CACHE)
    set (ENGINESTD_FOUND_LIB_TYPE ${ENGINESTD_LIB_TYPE} CACHE INTERNAL "Lib type when ENGINESTD library was last found")

    # ENGINESTD prefers static library type by default while CMake prefers shared one, so we need to change CMake preference to agree with ENGINESTD
    if (NOT ENGINESTD_LIB_TYPE STREQUAL SHARED)
        list (REVERSE CMAKE_FIND_LIBRARY_SUFFIXES)
    endif ()
endif ()

# Cater for the shared library extension in Emscripten build has been changed to ".bc"
if (EMSCRIPTEN)
    string (REPLACE .so .bc CMAKE_FIND_LIBRARY_SUFFIXES "${CMAKE_FIND_LIBRARY_SUFFIXES}")   # Stringify for string replacement
endif ()


set (ENGINESTD_PATH_SUFFIX "EngineStd")

if (CMAKE_PROJECT_NAME STREQUAL Urho3D)
    	# Library location is already known to be in the build tree

	set (ENGINESTD_HOME ${CMAKE_BINARY_DIR})
	set (ENGINESTD_INCLUDE_DIRS  "${ENGINESTD_HOME}/include/${ENGINESTD_PATH_SUFFIX}")
   
        set (ENGINESTD_LIBRARIES EngineStd)
        set (FOUND_MESSAGE "Found ENGINESTD: as CMake target")
else ()
    # Library location would be searched (based on ENGINESTD_HOME variable if provided and in system-wide default location)
    if (NOT ENGINESTD_HOME AND DEFINED ENV{ENGINESTD_HOME})
        file (TO_CMAKE_PATH "$ENV{ENGINESTD_HOME}" ENGINESTD_HOME)
    endif ()
    # ENGINESTD_HOME variable should be an absolute path, so use a non-rooted search even when we are cross-compiling
    if (ENGINESTD_HOME)
        list (APPEND CMAKE_PREFIX_PATH ${ENGINESTD_HOME})
        set (SEARCH_OPT NO_CMAKE_FIND_ROOT_PATH)
    endif ()
    # For Android platform, search in path similar to ANDROID_LIBRARY_OUTPUT_PATH variable
    if (ANDROID)
        if (ENGINESTD_HOME)
            set (ENGINESTD_LIB_SEARCH_HINT HINTS ${ENGINESTD_HOME}/libs/${ANDROID_NDK_ABI_NAME})
        else ()
            set (ENGINESTD_LIB_SEARCH_HINT HINTS /usr/local/libs/${ANDROID_NDK_ABI_NAME})
        endif ()
    endif ()
    if (NOT ENGINESTD_64BIT)
        # For 32-bit, force to search in 'lib' path even when the host system defaulted to use 'lib64'
        set_property (GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS FALSE)
    elseif (WIN32)
        # For 64-bit, force to search in 'lib64' path even when the Windows platform is not defaulted to use it
        set_property (GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS TRUE)
    endif ()
    find_path (ENGINESTD_INCLUDE_DIRS EngineStd.h PATH_SUFFIXES ${PATH_SUFFIX} ${SEARCH_OPT})
    if (ENGINESTD_INCLUDE_DIRS)
        set (BASE_INCLUDE_DIR ${ENGINESTD_INCLUDE_DIRS})   # Preserve the base include dir because the original variable will be turned into a list below
        get_filename_component (PATH ${ENGINESTD_INCLUDE_DIRS} PATH)
        set (ENGINESTD_INCLUDE_DIRS ${PATH} )       
        if (NOT ENGINESTD_HOME)
            # ENGINESTD_HOME is not set when using SDK installed on system-wide default location, so set it now
            get_filename_component (PATH ${PATH} PATH)
            set (ENGINESTD_HOME ${PATH})
        endif ()
    endif ()	
    find_library (ENGINESTD_LIBRARIES NAMES EngineStd ${ENGINESTD_LIB_SEARCH_HINT} PATH_SUFFIXES ${PATH_SUFFIX} ${SEARCH_OPT})
    if (WIN32)
        # For Windows platform, give a second chance to search for a debug version of the library
        find_library (ENGINESTD_LIBRARIES_DBG NAMES EngineStd_d ${ENGINESTD_LIB_SEARCH_HINT} PATH_SUFFIXES ${PATH_SUFFIX} ${SEARCH_OPT})
        # If both the non-debug and debug version of the libraries are found then use them both
        set (ENGINESTD_LIBRARIES_REL ${ENGINESTD_LIBRARIES})
        # Otherwise, ENGINESTD_LIBRARIES variable should have the path to either one of the version
        if (ENGINESTD_LIBRARIES)
            if (ENGINESTD_LIBRARIES_DBG)
                list (APPEND ENGINESTD_LIBRARIES ${ENGINESTD_LIBRARIES_DBG})
            endif ()
        else ()
            set (ENGINESTD_LIBRARIES ${ENGINESTD_LIBRARIES_DBG})
        endif ()
        # For shared library type, also initialize the ENGINESTD_DLL variable for later use
        if (ENGINESTD_LIB_TYPE STREQUAL SHARED AND ENGINESTD_HOME)
            find_file (ENGINESTD_DLL_REL EngineStd.dll HINTS ${URHO3D_HOME}/bin NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
            if (ENGINESTD_DLL_REL)
                list (APPEND ENGINESTD_DLL ${ENGINESTD_DLL_REL})
            endif ()
            find_file (ENGINESTD_DLL_DBG EngineStd_d.dll HINTS ${URHO3D_HOME}/bin NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
            if (ENGINESTD_DLL_DBG)
                list (APPEND ENGINESTD_DLL ${ENGINESTD_DLL_DBG})
            endif ()
        endif ()
    endif ()
endif ()

if (ENGINESTD_INCLUDE_DIRS AND ENGINESTD_LIBRARIES)
    set (ENGINESTD_FOUND 1)
    if (NOT FOUND_MESSAGE)
        set (FOUND_MESSAGE "Found ENGINESTD: ${ENGINESTD_LIBRARIES}")
    endif ()
    include (FindPackageMessage)
    FIND_PACKAGE_MESSAGE (ENGINESTD ${FOUND_MESSAGE} "[${ENGINESTD_LIBRARIES}][${ENGINESTD_INCLUDE_DIRS}]")
    set (ENGINESTD_HOME ${ENGINESTD_HOME} CACHE PATH "Path to ENGINESTD build tree or SDK installation location (external project only)" FORCE)
elseif (ENGINESTD_FIND_REQUIRED)
    if (ANDROID)
        set (NOT_FOUND_MESSAGE "For Android platform, double check if you have specified to use the same ANDROID_ABI as the ENGINESTD library in the build tree or SDK.")
    endif ()
    message (FATAL_ERROR
        "Could not find ENGINESTD library in ENGINESTD build tree or SDK installation. "
        "Use ENGINESTD_HOME environment variable or build option to specify the location of the build tree or SDK installation. ${NOT_FOUND_MESSAGE}")
endif ()

mark_as_advanced (ENGINESTD_INCLUDE_DIRS ENGINESTD_LIBRARIES ENGINESTD_LIBRARIES_REL ENGINESTD_LIBRARIES_DBG ENGINESTD_DLL ENGINESTD_DLL_REL ENGINESTD_DLL_DBG ENGINESTD_HOME)
