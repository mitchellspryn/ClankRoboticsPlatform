###
# Robot Library CMake file
#
# Configures the Robot library to be built.
#
# Requires a variable indicating the root directory to copy public header
# files of the library to.
#   CONTROL_INCLUDE_PATH
##

set ( LIBRARY_NAME "robot" )

set ( LIBRARY_HEADERS
  "TeleoperatedRobot.hpp"
  )

set ( LIBRARY_SOURCES
  "TeleoperatedRobot.cpp"
  )

add_library("${LIBRARY_NAME}" ${LIBRARY_SOURCES} )
add_dependencies("${LIBRARY_NAME}" "configuration" "module" "communication" "timers" )
target_link_libraries( "${LIBRARY_NAME}" "configuration" "module" "communication" "timers")
