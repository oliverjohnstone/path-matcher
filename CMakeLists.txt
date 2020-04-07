cmake_minimum_required(VERSION 3.15)
project(path_matcher)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_library(path_matcher src/path_matcher.cpp src/path_matcher.h)