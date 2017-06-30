cmake_minimum_required(VERSION 2.8)
project(come-come C)

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ansi -pedantic -Wall")
endif()

set(PACMAN_SRC
    src/main.c
    src/platform.c
    src/platform.h
    src/entities.c
    src/entities.h
    src/maps.c
    src/maps.h
)

add_executable(come-come ${PACMAN_SRC})

add_custom_command(TARGET come-come POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/maps $<TARGET_FILE_DIR:come-come>/maps)

add_custom_command(TARGET come-come POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/map_menu $<TARGET_FILE_DIR:come-come>/map_menu)
