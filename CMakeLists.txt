cmake_minimum_required(VERSION 3.26)
project(Assignment_0x5 C)

set(CMAKE_C_STANDARD 11)

include_directories(src)

add_executable(Assignment_0x5
        src/assembly.c
        src/assembly.h
        src/decode.c
        src/decode.h
        src/execute.c
        src/execute.h
        src/helper.c
        src/helper.h
        src/macros.h
        src/main.c
        src/memory.c
        src/memory.h
        src/read_exec.c
        src/read_exec.h
        src/simulate.c
        src/simulate.h)
