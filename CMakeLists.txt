cmake_minimum_required(VERSION 3.28)
project(memesql)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES src/*.cpp src/Parser/*.cpp)

add_library(memesql STATIC ${SOURCES})
add_executable(main main.cpp)
add_subdirectory(test)

target_include_directories(memesql PRIVATE include)

target_include_directories(main PRIVATE include)

target_link_libraries(main memesql)


# add_executable(main main.cpp)
# add_library(Lexer src/Parser/Lexer.cpp src/Cell.cpp src/Definitions.cpp)

# target_include_directories(Lexer PRIVATE include)

# target_include_directories(main PRIVATE include)

# target_link_libraries(main Lexer)