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
        src/tile.c
        src/tile.h
        src/keyboard.c
        src/keyboard.h
        src/camera.h
        src/pawn.c
        src/pawn.h
        src/hud.c
        src/hud.h
        src/item.c
        src/item.h
        src/item_inst.c
        src/item_inst.h
        src/particles.c
        src/particles.h
)

target_link_libraries(talesofcoph PRIVATE SDL2 m)

target_include_directories(talesofcoph PRIVATE ${CMAKE_SOURCE_DIR}/vendor)