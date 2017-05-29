###
# Timers Library CMake file
#
# Configures the Timers library to be built.
#
# Requires a variable indicating the root directory to copy public header
# files of the library to.
#   CONTROL_INCLUDE_PATH
##

set ( LIBRARY_NAME "timers" )

set ( LIBRARY_HEADERS
  "Watchdog.hpp"
  )

set ( LIBRARY_SOURCES
  "Watchdog.cpp"
  )

add_library("${LIBRARY_NAME}" ${LIBRARY_SOURCES} )
add_dependencies("${LIBRARY_NAME}" "configuration" "pthread")
target_link_libraries( "${LIBRARY_NAME}" "configuration" "pthread")
