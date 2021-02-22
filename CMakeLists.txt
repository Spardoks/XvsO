cmake_minimum_required(VERSION 3.16)

project(XvsO)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

include_directories(
    ./include
)

add_executable(
  Game_XvsO
  ./src/xvso.cpp
  ./src/main.cpp
)
