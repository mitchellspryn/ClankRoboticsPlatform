set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CONTROL_BUILD_PATH}/bin/util/motor" )

set( TEST_NAME "LiteMotorTest" )
project( "${TEST_NAME}" )
include_directories(${gtest_SOURCE_DIR}/include)
add_executable( "${TEST_NAME}" "LiteMotorTest.cpp" )
add_dependencies( "${TEST_NAME}"  "motor" "pthread")
target_link_libraries( "${TEST_NAME}" "motor" "pthread")

set( TEST_NAME "ClawMotorTest" )
project( "${TEST_NAME}" )
include_directories(${gtest_SOURCE_DIR}/include)
add_executable( "${TEST_NAME}" "ClawMotorTest.cpp" )
add_dependencies( "${TEST_NAME}"  "motor" "pthread")
target_link_libraries( "${TEST_NAME}" "motor" "pthread")

