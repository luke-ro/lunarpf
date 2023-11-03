include_directories(cfg/ details/ fmt/ sinks/)

add_library(spdlog spdlog.h)
set_target_properties(spdlog  PROPERTIES LINKER_LANGUAGE CXX)
