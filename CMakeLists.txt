cmake_minimum_required(VERSION 3.28) 
project(MuzhikDanmaku LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

file(GLOB_RECURSE SOURCES
"src/*.cpp"
)

add_executable(MuzhikDanmaku ${SOURCES})
target_include_directories(MuzhikDanmaku PRIVATE include)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-audio sfml-system)