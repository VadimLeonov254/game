cmake_minimum_required(VERSION 3.10)
project(game)

set(CMAKE_CXX_STANDARD 17)

find_package(raylib REQUIRED)

include_directories(hdr)

add_executable(game
    src/main.cpp
    src/cube.cpp
    src/player.cpp
    src/hold.cpp
    src/climbing.cpp
    src/route.cpp
    src/environment.cpp
)

target_link_libraries(game
    raylib
)
