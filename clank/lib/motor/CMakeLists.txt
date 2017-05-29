###
# Motor Library CMake file
#
# Configures the Motor library to be built.
#
# Requires a variable indicating the root directory to copy public header
# files of the library to.
#   CONTROL_INCLUDE_PATH
##

set ( LIBRARY_NAME "motor" )

set ( LIBRARY_HEADERS
  "IMotor.hpp"
  "VHN2Motor/VHN2Motor.hpp"
  "LiteMotor/LiteMotor.hpp"
  "ClawMotor/ClawMotor.hpp"
  "ClawMotor/ClawMotorController.hpp"
  )

set ( LIBRARY_SOURCES
  "VHN2Motor/VHN2Motor.cpp"
  "LiteMotor/LiteMotor.cpp"
  "ClawMotor/ClawMotor.cpp"
  "ClawMotor/ClawMotorController.cpp"
  )

add_library("${LIBRARY_NAME}" ${LIBRARY_SOURCES} )
add_dependencies("${LIBRARY_NAME}" "configuration" "hardware" )
target_link_libraries( "${LIBRARY_NAME}" "configuration" "hardware")
