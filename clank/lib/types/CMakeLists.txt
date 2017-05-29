###
# Types Library CMake file
#
# Configures the types library to be built.
#
# Requires a variable indicating the root directory to copy public header
# files of the library to.
#   CONTROL_INCLUDE_PATH
##

set ( LIBRARY_NAME "types" )

set ( LIBRARY_HEADERS
  "navigation/MapPrecision.hpp"
  "navigation/CSpacePoint.hpp"
  )

set ( LIBRARY_SOURCES
  "navigation/CSpacePoint.cpp"
  )

add_library("${LIBRARY_NAME}" ${LIBRARY_SOURCES} )
