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
)

add_executable(come-come ${PACMAN_SRC})
