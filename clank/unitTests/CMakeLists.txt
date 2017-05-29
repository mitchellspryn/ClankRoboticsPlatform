include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CONTROL_BUILD_PATH}/bin" )

#Copy over the unit test shell script
file(COPY "RunAllUnitTests.sh" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

add_subdirectory("lib")


