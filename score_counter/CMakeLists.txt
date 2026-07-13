cmake_minimum_required(VERSION 3.14)

project(score_counter)

include(FetchContent)

FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG f091b2c
)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG 55f9368
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(cpr)
FetchContent_MakeAvailable(json)

#Find pigpio on your system (now in /usr/local)
find_path(PIGPIO_INCLUDE_DIR pigpio.h)
find_library(PIGPIO_LINK_LIB pigpio)

if(NOT PIGPIO_INCLUDE_DIR OR NOT PIGPIO_LINK_LIB)
    message(FATAL_ERROR "pigpio not found! Double check your installation.")
endif()

# Tell CMake to look into your subdirectories
add_subdirectory(src)
add_subdirectory(apps)