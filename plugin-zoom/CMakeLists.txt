cmake_minimum_required(VERSION 3.10)
project(pfa_vite)

set(CMAKE_CXX_STANDARD 11)

find_package(X11 COMPONENTS X11)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

add_executable(slicer src/slicer.cpp src/CImg.h)

target_link_libraries(slicer m pthread ${X11_LIBRARIES})

