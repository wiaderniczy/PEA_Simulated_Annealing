cmake_minimum_required(VERSION 3.22)
project(PEA_4)

set(CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")

add_executable(PEA_4 main.cpp Menu.cpp Menu.h includes.h SimulatedAnnealing.cpp SimulatedAnnealing.h)
