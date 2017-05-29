###
# Hardware Library CMake file
#
# Configures the Hardware library to be built.
#
# Requires a variable indicating the root directory to copy public header
# files of the library to.
#   CONTROL_INCLUDE_PATH
##

set ( LIBRARY_NAME "hardware" )

set ( LIBRARY_HEADERS
  "SamPeripheralExtentions.hpp"
  "SamPeripheral.hpp"
  "SamPeripheralManager.hpp"
  "SamSerialPort.hpp"
  "GpioTypes.hpp"
  "GpioPin.hpp"
  "AdcPin.hpp"
  "PwmPin.hpp"
  )

set ( LIBRARY_SOURCES
  "SamPeripheralExtentions.cpp"
  "SamPeripheral.cpp"
  "SamPeripheralManager.cpp"
  "SamSerialPort.cpp"
  "GpioPin.cpp"
  "AdcPin.cpp"
  "PwmPin.cpp"
  )

add_library("${LIBRARY_NAME}" ${LIBRARY_SOURCES} )
add_dependencies("${LIBRARY_NAME}" "configuration" "pthread")
target_link_libraries( "${LIBRARY_NAME}" "configuration" "pthread")
