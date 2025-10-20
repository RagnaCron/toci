cmake_minimum_required(VERSION 4.0)
project(sdl3_template C)

# This is so the runtime and assets work with each other. =)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/assets
        DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(SDLTTF_VENDORED ON)
#set(GLM_BUILD_TEST OFF)

#add_subdirectory(vendors)
add_subdirectory(src)
