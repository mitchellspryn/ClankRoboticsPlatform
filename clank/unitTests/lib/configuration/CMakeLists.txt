set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CONTROL_BUILD_PATH}/bin/unitTest/configuration" )

set( TEST_NAME "ConfigurationTest" )
project( "${TEST_NAME}" )
include_directories(${gtest_SOURCE_DIR}/include)
add_executable( "${TEST_NAME}" "ConfigurationUnitTest.cpp" )
add_dependencies( "${TEST_NAME}" "gtest_main" "configuration" )
target_link_libraries( "${TEST_NAME}" "gtest_main" "configuration" )

set( TEST_NAME "IndentedFileConfigurationReaderTest" )
project( "${TEST_NAME}" )
include_directories(${gtest_SOURCE_DIR}/include)
add_executable( "${TEST_NAME}" "IndentedFileConfigurationReaderUnitTest.cpp" )
add_dependencies( "${TEST_NAME}" "gtest_main" "configuration" )
target_link_libraries( "${TEST_NAME}" "gtest_main" "configuration" )


#copy over the test files
file(COPY "IndentedFileConfigurationReaderUnitTestInput_MultiLineGood.txt" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
file(COPY "IndentedFileConfigurationReaderUnitTestInput_HighlyIndented.txt" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
file(COPY "IndentedFileConfigurationReaderUnitTestInput_BadTagFormatting.txt" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
file(COPY "IndentedFileConfigurationReaderUnitTestInput_BadValueFormatting.txt" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
file(COPY "IndentedFileConfigurationReaderUnitTestInput_MultipleTopLevelTags.txt" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

