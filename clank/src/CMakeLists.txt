set( EXECUTABLE_NAME "StartTeleoperatedRobot" )
project( "${EXECUTABLE_NAME}" )
add_executable( "${EXECUTABLE_NAME}" "StartTeleoperatedRobot.cpp" )
#include_directories( "${OPENCV_INCLUDE_DIRS}")
add_dependencies( "${EXECUTABLE_NAME}" "configuration" "robot" )
target_link_libraries( "${EXECUTABLE_NAME}" "configuration" "robot" )
