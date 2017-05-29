set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CONTROL_BUILD_PATH}/bin" )
file( COPY "navigation/MapPlotter.py" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/python/visualization/navigation/" )
