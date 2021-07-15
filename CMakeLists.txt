cmake_minimum_required(VERSION 2.8.9)
project (test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -I ./")
set(FBS_SCHEMA ${CMAKE_CURRENT_SOURCE_DIR}/Widget.fbs)
set(FBS_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

file(MAKE_DIRECTORY ${FBS_BINARY_DIR})
add_custom_command(
  OUTPUT ${FBS_BINARY_DIR}/widget_generated.h
  COMMAND flatc --binary --schema -o ${FBS_BINARY_DIR} --cpp ${FBS_SCHEMA}
  DEPENDS ${FBS_SCHEMA})
include_directories(SYSTEM ${FBS_BINARY_DIR})

add_executable(test test.cpp ${FBS_BINARY_DIR}/widget_generated.h)
target_link_libraries(test debug flatbuffers optimized flatbuffers)
