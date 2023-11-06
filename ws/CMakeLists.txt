# add_subdirectory(tests/)


add_executable(coords_test tests/coordinates_test.cpp)
target_link_libraries(coords_test TinyTIFF)
target_link_libraries(coords_test Eigen3::Eigen)
target_link_libraries(coords_test TerrainHandler)
target_link_libraries(coords_test spdlog)