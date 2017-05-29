set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CONTROL_BUILD_PATH}/bin/util/navigation" )

set( TEST_NAME "Test2DMapVisualization" )
project( "${TEST_NAME}" )
add_executable( "${TEST_NAME}" "Test2DMapVisualization.cpp" )
add_dependencies( "${TEST_NAME}" "types" "navigation" )
target_link_libraries( "${TEST_NAME}" "types" "navigation" )

