cmake_minimum_required(VERSION 3.20)
project(nanojvm LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

add_subdirectory(nanojvm-common)
add_subdirectory(nanojvm-classfile)
add_subdirectory(nanojvm-tests)
# add_subdirectory(nanojvm-core)

