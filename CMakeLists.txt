cmake_minimum_required(VERSION 3.5)

project(tecapsdesafio8 LANGUAGES C)

file(GLOB_RECURSE SOURCES
    src/*.c
)

add_executable(desafio8 ${SOURCES})

enable_testing()

add_subdirectory(test)
