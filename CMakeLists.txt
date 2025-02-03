cmake_minimum_required(VERSION 3.28) 
#prefer to create variables for portability
set(PROJECT_NAME MuzhikDanmaku)
project(${PROJECT_NAME} VERSION 0.1.0 LANGUAGES CXX)

#debug
set(CMAKE_CXX_FLAGS "-ggdb -pedantic-errors -Wall -Wextra -Wconversion -Wsign-conversion -Werror")

#release
#set(CMAKE_CXX_FLAGS "-O2 -DNDEBUG")

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    #the library literally got major 3.0.0 update which brake the game. So consider using old vers.
    GIT_TAG 2.6.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

find_package(SFML 2 REQUIRED COMPONENTS graphics audio window system)

set(SOURCE_DIR src) #lmao

file(GLOB_RECURSE SOURCES
"${SOURCE_DIR}/*.cpp"
"${SOURCE_DIR}/*.h"
)
#This command actually build all files into one single executable
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-audio sfml-system)