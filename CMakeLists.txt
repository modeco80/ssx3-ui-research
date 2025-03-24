cmake_minimum_required(VERSION 3.25)
project(luifun)

find_package(SDL2 REQUIRED)

add_library(core
    # Core
    src/core/stringprintf.cpp
    src/core/mmap_file.cpp
    src/core/refpack.cpp
    src/core/hashname.cpp
)

target_compile_features(core PUBLIC
    cxx_std_20
)

target_include_directories(core PUBLIC ${PROJECT_SOURCE_DIR}/src)


add_library(core_sdl
    # core/SDL
    src/core/sdl/window.cpp
)

target_compile_features(core_sdl PUBLIC
    cxx_std_20
)

target_include_directories(core_sdl PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(core_sdl PUBLIC
    core
    SDL2::SDL2
)

add_executable(screendump
    src/screendump_tool.cpp
)

target_link_libraries(screendump PRIVATE
    core
)

add_executable(hashname
    src/hashname_tool.cpp
)

target_link_libraries(hashname PRIVATE
    core
)


add_subdirectory(src/player)