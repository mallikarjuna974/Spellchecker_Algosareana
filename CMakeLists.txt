cmake_minimum_required(VERSION 3.16)
project(SubstringAutocorrect LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Python3 COMPONENTS Interpreter REQUIRED)
execute_process(
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/generate_dictionary.py
            ${CMAKE_SOURCE_DIR}/data/dictionary.txt
    RESULT_VARIABLE DICTIONARY_GENERATION_RESULT
)
if(NOT DICTIONARY_GENERATION_RESULT EQUAL 0)
    message(FATAL_ERROR "Failed to generate data/dictionary.txt")
endif()

add_executable(autocorrect
    src/main.cpp
    src/Dictionary.cpp
    src/TrieGraph.cpp
    src/Ranker.cpp
    src/Autocorrect.cpp
)
target_include_directories(autocorrect PRIVATE include)
if (MSVC)
    target_compile_options(autocorrect PRIVATE /W4)
else()
    target_compile_options(autocorrect PRIVATE -Wall -Wextra -Wpedantic)
endif()
