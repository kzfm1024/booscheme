cmake_minimum_required(VERSION 2.8)
project(booscheme)
add_executable(test test.cpp pair.cpp number.cpp symbol.cpp environment.cpp primitive.cpp closure.cpp output_port.cpp boo.cpp)
target_link_libraries(test gc)
