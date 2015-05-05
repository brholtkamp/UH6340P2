cmake_minimum_required(VERSION 3.1)
project(Project_2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Tree/tree.hpp Tree/Node/node.hpp)
add_executable(Project_2 ${SOURCE_FILES} Tree/tree.hpp Tree/Node/node.hpp Tree/Node/internalNode.hpp Tree/Node/leafNode.hpp Tree/tree.hpp Tree/Node/node.hpp Tree/Node/internalNode.hpp Tree/Node/leafNode.hpp)