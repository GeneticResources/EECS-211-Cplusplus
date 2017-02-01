cmake_minimum_required(VERSION 3.3)
project(hw04 CXX)
include(.eecs211/CMakeLists.txt)

add_test_program(linked_test
                 linked_test.cpp
                 linked_lib.cpp)

