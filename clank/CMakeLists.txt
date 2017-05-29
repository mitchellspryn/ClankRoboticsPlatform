cmake_minimum_required ( VERSION 2.8 )
find_package( Threads REQUIRED )
find_package( Boost REQUIRED )

#CMake Version 3.2 lies about unnecessary dependencies! 
#Disable the terrible warning
cmake_policy(SET CMP0046 OLD) 


#When debug mode is enabled, the code will run slower, but it will print more informative error messages
add_definitions( -DDEBUG_MODE )

#A define for the precision of the mapping module
add_definitions( -DMAPPING_MODULE_PRECISION=double )

# Set build tree location
set ( CONTROL_BUILD_PATH "${CMAKE_CURRENT_LIST_DIR}/build" )
set ( CONTROL_ADMIN_PATH "${CMAKE_CURRENT_LIST_DIR}/admin" )

message("Current Control Build Path: ${CONTROL_BUILD_PATH}")

# Set up directories to store the built files
set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CONTROL_BUILD_PATH}/bin" )
set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CONTROL_BUILD_PATH}/lib" )

# Default compilation flags
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Wall -g -std=c++0x -lpthread" )
set ( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -std=c++0x -lpthread" )

# Check this directory for additional header files (.hpp/.h)
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/lib" )

# Build all libraries
add_subdirectory( "./lib" "${CONTROL_BUILD_PATH}/lib" )

# Build utilities
add_subdirectory( "./util" "${CONTROL_BUILD_PATH}/util" )

#Build unit tests
add_subdirectory( "./unitTests" "${CONTROL_BUILD_PATH}/unitTests" )

#Build the final project
add_subdirectory( "./src" "${CONTROL_BUILD_PATH}/src" )
