cmake_minimum_required (VERSION 2.6)





set(
    CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${CMAKE_HOME_DIRECTORY}/lib
    )

set(
    CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${CMAKE_HOME_DIRECTORY}/bin
    )


set(
    CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_HOME_DIRECTORY}/bin
    )

add_subdirectory(src/bd_encodercs)
add_subdirectory(src/video_server/encoder)
