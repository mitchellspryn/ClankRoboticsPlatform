###
# Communication Library CMake file
#
# Configures the Communication library to be built.
#
# Requires a variable indicating the root directory to copy public header
# files of the library to.
#   CONTROL_INCLUDE_PATH
##

set ( LIBRARY_NAME "communication" )

set ( LIBRARY_HEADERS
  "messages.hpp"
  "TcpServer.hpp"
  "Command.hpp"
  )

set ( LIBRARY_SOURCES
  "TcpServer.cpp"
  "Command.cpp"
  )

add_library("${LIBRARY_NAME}" ${LIBRARY_SOURCES} )
add_dependencies("${LIBRARY_NAME}" "configuration" "messages" "pthread")
target_link_libraries( "${LIBRARY_NAME}" )
