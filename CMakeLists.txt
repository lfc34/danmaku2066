cmake_minimum_required(VERSION 3.28) 
project(MuzhikDanmaku VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

#debug
#set(CMAKE_CXX_FLAGS "-ggdb -pedantic-errors -Wall -Wextra -Wconversion -Wsign-conversion -Werror")

#game with optimisation release
set(CMAKE_CXX_FLAGS "-O2 -DNDEBUG")

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    #the library literally got major 3.0.0 update which broke the game. So consider using old vers.
    GIT_TAG 2.6.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

file(GLOB_RECURSE SOURCES
"src/*.cpp"
"src/*.h"
)

add_executable(MuzhikDanmaku ${SOURCES})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-audio sfml-system)