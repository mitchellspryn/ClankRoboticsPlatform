###
# Messages Library CMake file
#
# Configures the Messages library to be built.
#
# Requires a variable indicating the root directory to copy public header
# files of the library to.
#   CONTROL_INCLUDE_PATH
##

set ( LIBRARY_NAME "messages" )

set ( LIBRARY_HEADERS
  "messages.hpp"
  )

set ( LIBRARY_SOURCES
  "dummy.cpp"
  )

add_library("${LIBRARY_NAME}" ${LIBRARY_SOURCES} )
target_link_libraries( "${LIBRARY_NAME}" )
