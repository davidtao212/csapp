cmake_minimum_required(VERSION 3.19)
project(csapp C)

set(CMAKE_C_STANDARD 11)

#include_directories(datalab-handout)

add_executable(phase_6 bomb/phase_6.c)

add_executable(phase_secret bomb/phase_secret.c)

add_executable(csim cachelab/csim.c cachelab/cachelab.c)
