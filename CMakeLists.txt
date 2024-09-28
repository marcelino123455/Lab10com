cmake_minimum_required(VERSION 3.28)
project(lab10)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab10 main.cpp exp.cpp parser.cpp scanner.cpp token.cpp visitor.cpp)
