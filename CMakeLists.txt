cmake_minimum_required(VERSION 3.5)

project(simulator LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

add_subdirectory(mimalloc)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenMP REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
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
        heap.h
        heap.cpp
        instruction_impl.cpp instruction_impl.h mainwindow.ipp syscall.h)

target_link_libraries(simulator PRIVATE Threads::Threads Qt5::Widgets mimalloc-static)
