cmake_minimum_required(VERSION 3.3)
project(networksim CXX)
include(.eecs211/CMakeLists.txt)

set(SRC system.cpp
        machines.cpp
        datagram.cpp
        parsing.cpp
        errors.cpp)

add_program(networksim
        main.cpp
        interface.cpp
        ${SRC})

add_test_program(networksim_test
        networksim_test.cpp
        ${SRC})
