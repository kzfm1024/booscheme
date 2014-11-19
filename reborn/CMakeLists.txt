cmake_minimum_required(VERSION 2.8)
project(booscheme)
add_executable(test test.cpp pair.cpp number.cpp symbol.cpp environment.cpp primitive.cpp primitive_cc.cpp primitive_call_cc.cpp closure.cpp input_port.cpp input_port.cpp output_port.cpp boo.cpp base_library.cpp eval_cc.cpp continuation.cpp)
target_link_libraries(test gc)
