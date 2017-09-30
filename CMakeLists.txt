cmake_minimum_required(VERSION 3.8)
project(CompressionOfGraphicInformationByNN)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c image_parser.h image_parser.c)
add_executable(CompressionOfGraphicInformationByNN ${SOURCE_FILES})