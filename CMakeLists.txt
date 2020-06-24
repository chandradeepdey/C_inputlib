cmake_minimum_required(VERSION 3.16)
project("C_inputlib"
        VERSION 8.5
        DESCRIPTION "Some functions to input things like ints and doubles and strings with error checking"
        LANGUAGES C)

set(CMAKE_C_STANDARD 11)

add_library(inputlib STATIC src/inputlib.c)
target_include_directories(inputlib PUBLIC include)
