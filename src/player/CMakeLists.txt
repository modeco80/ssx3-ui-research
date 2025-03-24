# test player thing maybe

add_executable(uiplayer
    main.cpp
)

target_link_libraries(uiplayer PRIVATE
    core
    core_sdl
    SDL2_ttf
)

set_target_properties(uiplayer
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)