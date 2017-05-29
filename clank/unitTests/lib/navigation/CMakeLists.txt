set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CONTROL_BUILD_PATH}/bin/unitTest/navigation" )

set( TEST_NAME "SparseMapTest" )
project( "${TEST_NAME}" )
include_directories(${gtest_SOURCE_DIR}/include)
add_executable( "${TEST_NAME}" "SparseMapTest.cpp" )
add_dependencies( "${TEST_NAME}" "gtest_main" "types" "navigation" )
target_link_libraries( "${TEST_NAME}" "gtest_main" "types" "navigation" )

