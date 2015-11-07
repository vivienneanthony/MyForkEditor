# Set project name
project (HangarsClient)

# Define target name
set (TARGET_NAME HangarsClient)

# Set minimum version
cmake_minimum_required (VERSION 2.8.6)
if (COMMAND cmake_policy)
    cmake_policy (SET CMP0003 NEW)
    if (CMAKE_VERSION VERSION_GREATER 2.8.12 OR CMAKE_VERSION VERSION_EQUAL 2.8.12)
        # INTERFACE_LINK_LIBRARIES defines the link interface
        cmake_policy (SET CMP0022 NEW)
    endif ()
    if (CMAKE_VERSION VERSION_GREATER 3.0.0 OR CMAKE_VERSION VERSION_EQUAL 3.0.0)
        # Disallow use of the LOCATION target property - therefore we set to OLD as we still need it
        cmake_policy (SET CMP0026 OLD)
        # MACOSX_RPATH is enabled by default
        cmake_policy (SET CMP0042 NEW)
    endif ()
endif ()

# Set CMake modules search path
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

# Include Urho3D Cmake common module
include (Urho3D-CMake-common)

# Find Urho3D library
find_package (Urho3D REQUIRED)
find_package (EngineStd REQUIRED)

# Add include directories 
include_directories (${URHO3D_INCLUDE_DIRS})
include_directories (${ENGINESTD_INCLUDE_DIRS})

# Add directory as include path
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}")

# Define files
file(GLOB_RECURSE HANGARSCLIENT_INCLUDE_FILES *.h)
file(GLOB_RECURSE HANGERSCLIENT_SOURCE_FILES *.cpp)

# Define source files
define_source_files (GLOB_CPP_PATTERNS *.cpp ${GLOB_OBJC_PATTERN} EXTRA_H_FILES RECURSE)

# Setup target with resource copying
setup_main_executable ()

#attempt to link
target_link_libraries(HangarsClient Urho3D EngineStd)

