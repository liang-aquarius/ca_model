cmake_minimum_required(VERSION 3.14)
project(ca_fifo)

set(CMAKE_CXX_STANDARD 14)

include_directories(libs)
include_directories(designs)

add_executable(ca_fifo tests/ca_fifo/main.cpp libs/ca_module.h designs/ca_fifo/ca_fifo_ctrl.h designs/ca_fifo/ca_fifo_mem.h libs/common.cpp libs/common.h designs/ca_fifo/ca_fifo_top.h tests/ca_fifo/driver_monitor.h)
