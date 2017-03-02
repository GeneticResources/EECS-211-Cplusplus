cmake_minimum_required(VERSION 3.3)
project(networksim CXX)
include(.eecs211/CMakeLists.txt)

add_program(networksim
            main.cpp
            interface.cpp
            system.cpp
            machines.cpp
            datagram.cpp
            parsing.cpp
            errors.cpp
            linked_lib.cpp)

add_test_program(networksim_test
                 networksim_test.cpp
                 system.cpp
                 machines.cpp
                 datagram.cpp
                 parsing.cpp
                 errors.cpp
                 linked_lib.cpp)
