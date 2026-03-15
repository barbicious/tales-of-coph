cmake_minimum_required(VERSION 4.1)
project(talesofcoph C)

set(CMAKE_C_STANDARD 23)

add_executable(talesofcoph src/main.c
        src/state.c
        src/state.h
        src/utils.h
        src/renderer.c
        src/renderer.h
        src/window.c
        src/window.h
        src/sprite_sheet.c
        src/sprite_sheet.h
        src/font.c
        src/font.h
        src/arcade.c
        src/arcade.h
)

target_link_libraries(talesofcoph PRIVATE SDL2 m)

target_include_directories(talesofcoph PRIVATE ${CMAKE_SOURCE_DIR}/vendor)