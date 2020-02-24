cmake_minimum_required(VERSION 3.5)

project(simulator LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(mimalloc)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_library(Threads REQUIRED)
find_library(OpenMP REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -fopenmp")

add_executable(simulator
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        global.cpp
        global.h
        instrcution.cpp
        instrcution.h
        executor.h
        executor.cpp
        instruction_impl.cpp instruction_impl.h mainwindow.ipp syscall.h)

target_link_libraries(simulator PRIVATE ${CMAKE_THREAD_LIBS_INIT} ${OpenMP_CXX_LIBRARIES} Qt5::Widgets mimalloc-static)
