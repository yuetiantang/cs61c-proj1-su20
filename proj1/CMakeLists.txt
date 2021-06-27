cmake_minimum_required(VERSION 3.16)
project(su20_proj1_yuetiantang C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(su20_proj1_yuetiantang
        hashtable.c
        hashtable.h
        philspel.c
        philspel.h)
