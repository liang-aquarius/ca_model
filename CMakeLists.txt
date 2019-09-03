cmake_minimum_required(VERSION 3.14)
project(ca_fifo)

set(CMAKE_CXX_STANDARD 14)

include_directories(utils)
include_directories(src)

add_executable(ca_fifo tests/main.cpp utils/ca_module.h src/ca_fifo_ctrl.h src/ca_fifo_mem.h utils/common.cpp utils/common.h src/ca_fifo_top.h tests/driver_monitor.h)
