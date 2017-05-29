include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

#First level 
add_subdirectory("gtest")
add_subdirectory("types")
add_subdirectory("configuration")
add_subdirectory("visualization")

#Second level
add_subdirectory("communication")
add_subdirectory("hardware")
add_subdirectory("motor")
add_subdirectory("timers")
add_subdirectory("navigation")

#Third level
add_subdirectory("module")

#Fourth level
add_subdirectory("robot")
