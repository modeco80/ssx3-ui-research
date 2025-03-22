cmake_minimum_required(VERSION 3.25)
project(luifun)

find_package(SDL2 REQUIRED)

add_executable(lui_thing
    # sources

    # Core
    src/core/stringprintf.cpp
    src/core/mmap_file.cpp
    src/core/refpack.cpp
    src/core/hashname.cpp


    # core/SDL
    src/core/sdl/window.cpp
    src/core/sdl/texture.cpp


    src/main.cpp
)

target_compile_features(lui_thing PUBLIC
    cxx_std_20
)

target_include_directories(lui_thing PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(lui_thing PRIVATE
    SDL2::SDL2
)