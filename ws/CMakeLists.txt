# add_subdirectory(tests/)

add_executable(altTest tests/altTest.cpp)
target_link_libraries(altTest TinyTIFF)
target_link_libraries(altTest Eigen3::Eigen)
target_link_libraries(altTest TerrainHandler)
target_link_libraries(altTest spdlog)

add_executable(coords_test tests/coordinates_test.cpp)
target_link_libraries(coords_test TinyTIFF)
target_link_libraries(coords_test Eigen3::Eigen)
target_link_libraries(coords_test TerrainHandler)
target_link_libraries(coords_test spdlog)
