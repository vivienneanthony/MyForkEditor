# Set project name
project (EngineStd)

# Define target name
set (TARGET_NAME EngineStd)

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
include_directories (${URHO3D_INCLUDE_DIRS})

# Define files
file(GLOB_RECURSE ENGINE_INCLUDE_FILES *.h)
file(GLOB_RECURSE ENGINE_SOURCE_FILES *.cpp)

# Add directory as include path
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}")

# Setup library output path
if (ANDROID)
    set (OUTPUT_PATH ${ANDROID_LIBRARY_OUTPUT_PATH})
else ()
    set (OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
endif ()
set_output_directories (${OUTPUT_PATH} ARCHIVE LIBRARY)

# Define source files
define_source_files (GLOB_CPP_PATTERNS *.cpp ${GLOB_OBJC_PATTERN} EXTRA_H_FILES RECURSE)

# Setup target with resource copying
setup_library ()

# Install headers for using the Urho3D library
set (ENGINESTD_INCLUDE_DIR include/EngineStd)

# Generate platform specific pkg-config file for the benefit of Urho3D library users via SDK without CMake
get_directory_property (URHO3D_COMPILE_DEFINITIONS DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMPILE_DEFINITIONS)

# Install header files
install_header_files (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${ENGINESTD_INCLUDE_DIR} FILES_MATCHING PATTERN *.h USE_FILE_SYMLINK)  

# workaround
target_link_libraries(EngineStd Urho3D)



