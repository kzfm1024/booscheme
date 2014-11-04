cmake_minimum_required(VERSION 2.8)
project(booscheme)
add_executable(test test.cpp symbol.cpp util.cpp)
target_link_libraries(test gc)
