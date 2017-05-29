###
# Configuration Library CMake file
#
# Configures the Server library to be built.
#
# Requires a variable indicating the root directory to copy public header
# files of the library to.
#   CONTROL_INCLUDE_PATH
##

set ( LIBRARY_NAME "configuration" )

set ( LIBRARY_HEADERS
  "Configuration.hpp"
  "IndentedFileConfigurationReader.hpp"
  )

set ( LIBRARY_SOURCES
  "Configuration.cpp"
  "IndentedFileConfigurationReader.cpp"
  )

add_library("${LIBRARY_NAME}" ${LIBRARY_SOURCES} )
target_link_libraries( "${LIBRARY_NAME}" )
