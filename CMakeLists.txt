cmake_minimum_required(VERSION 3.27.4)
project(lunarpf)

include_directories(lib/)
# include_directories(src/)

set(CMAKE_BUILD_TYPE Debug)

find_package(TinyTIFF REQUIRED)
find_package(Eigen3 3.0 REQUIRED NO_MODULE)

add_executable(${PROJECT_NAME} ws/main.cpp)

add_subdirectory(ws)
# add_subdirectory(ws)
add_subdirectory(src)
add_subdirectory(lib)

target_link_libraries(${PROJECT_NAME} TinyTIFF)
target_link_libraries(${PROJECT_NAME} Eigen3::Eigen)
target_link_libraries(${PROJECT_NAME} TerrainHandler)
target_link_libraries(${PROJECT_NAME} spdlog) #located in ./lib/





    