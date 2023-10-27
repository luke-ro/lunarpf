cmake_minimum_required(VERSION 3.27.4)
project(lunarpf)

find_package(TinyTIFF REQUIRED)
find_package(Eigen3 3.0 REQUIRED NO_MODULE)

add_executable(${PROJECT_NAME} ws/main.cpp)

add_subdirectory(ws)
add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} TinyTIFF)
target_link_libraries(${PROJECT_NAME} Eigen3::Eigen)
target_link_libraries(${PROJECT_NAME} TerrainHandler)
    