cmake_minimum_required(VERSION 3.20.0)

set(SHIELD x_nucleo_iks01a2)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(nos)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

target_sources(app PRIVATE src/main.c)