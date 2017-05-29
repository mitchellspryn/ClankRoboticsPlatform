set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CONTROL_BUILD_PATH}/bin/util/hardware" )

set( TEST_NAME "GpioTest" )
project( "${TEST_NAME}" )
include_directories(${gtest_SOURCE_DIR}/include)
add_executable( "${TEST_NAME}" "GpioTest.cpp" )
add_dependencies( "${TEST_NAME}" "configuration" "hardware" "pthread" )
target_link_libraries( "${TEST_NAME}" "configuration" "hardware" "pthread")

set( TEST_NAME "AdcTest" )
project( "${TEST_NAME}" )
include_directories(${gtest_SOURCE_DIR}/include)
add_executable( "${TEST_NAME}" "AdcTest.cpp" )
add_dependencies( "${TEST_NAME}" "configuration" "hardware" "pthread")
target_link_libraries( "${TEST_NAME}" "configuration" "hardware" "pthread")

set( TEST_NAME "PwmTest" )
project( "${TEST_NAME}" )
include_directories(${gtest_SOURCE_DIR}/include)
add_executable( "${TEST_NAME}" "PwmTest.cpp" )
add_dependencies( "${TEST_NAME}" "configuration" "hardware" "pthread")
target_link_libraries( "${TEST_NAME}" "configuration" "hardware" "pthread")

