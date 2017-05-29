set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CONTROL_BUILD_PATH}/bin/unitTest/timers" )

set( TEST_NAME "WatchdogTest" )
project( "${TEST_NAME}" )
include_directories(${gtest_SOURCE_DIR}/include)
add_executable( "${TEST_NAME}" "WatchdogUnitTest.cpp" )
add_dependencies( "${TEST_NAME}" "gtest_main" "configuration" "timers" "pthread")
target_link_libraries( "${TEST_NAME}" "gtest_main" "configuration" "timers" "pthread")

