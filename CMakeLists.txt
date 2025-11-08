cmake_minimum_required(VERSION 3.28) 
project(MuzhikDanmaku LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# LOGGER SETUP
option(ENABLE_LOGGER "Enable logger" ON)
if (ENABLE_LOGGER) 
    add_definitions(-DENABLE_LOGGER)
endif()

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

file(GLOB_RECURSE SOURCES
"src/*.cpp"
)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window Audio System)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

add_executable(MuzhikDanmaku ${SOURCES})
target_include_directories(MuzhikDanmaku PRIVATE include)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics SFML::Window SFML::Audio SFML::System)