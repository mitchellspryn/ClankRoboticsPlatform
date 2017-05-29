###
# Navigation Library CMake file
#
# Configures the Navigation library to be built.
#
# Requires a variable indicating the root directory to copy public header
# files of the library to.
#   CONTROL_INCLUDE_PATH
##

set ( LIBRARY_NAME "navigation" )

set ( LIBRARY_HEADERS
  "IMap.hpp"
  "IObstacle.hpp"
  "Obstacle.hpp"
  "SparseMap.hpp"
  )

set ( LIBRARY_SOURCES
  "Obstacle.cpp"
  "SparseMap.cpp"
  )

add_library("${LIBRARY_NAME}" ${LIBRARY_SOURCES} )
add_dependencies("${LIBRARY_NAME}" "types" )
target_link_libraries( "${LIBRARY_NAME}" "types")
