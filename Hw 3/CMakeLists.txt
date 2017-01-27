cmake_minimum_required(VERSION 3.3)
project(hw03 CXX)
include(.eecs211/CMakeLists.txt)

add_program(circle
            circle.cpp
            circle_lib.cpp)

add_program(prime
            prime.cpp
            prime_lib.cpp)

add_test_program(circle_test
                 circle_test.cpp
                 circle_lib.cpp)

add_test_program(prime_test
                 prime_test.cpp
                 prime_lib.cpp)

