cmake_minimum_required(VERSION 3.18)
project(projetaudisen)

set(CMAKE_CXX_STANDARD 17)

add_executable(projetaudisen main.cpp
        ams.c
        amp.c
        amp.h
        frame.c
        usb.c
        usb.h)
