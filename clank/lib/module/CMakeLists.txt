###
# Module Library CMake file
#
# Configures the module library to be built.
#
# Requires a variable indicating the root directory to copy public header
# files of the library to.
#   CONTROL_INCLUDE_PATH
##

set ( LIBRARY_NAME "module" )

set ( LIBRARY_HEADERS
  "DifferentialDriveModule/DifferentialDriveModule.hpp"
  "VideoStreamer/VideoStreamer.hpp"
  )

set ( LIBRARY_SOURCES
  "DifferentialDriveModule/DifferentialDriveModule.cpp"
  "VideoStreamer/VideoStreamer.cpp"
  )

add_library("${LIBRARY_NAME}" ${LIBRARY_SOURCES} )
add_dependencies("${LIBRARY_NAME}" "configuration" "motor" )
target_link_libraries( "${LIBRARY_NAME}" "configuration" "motor")
