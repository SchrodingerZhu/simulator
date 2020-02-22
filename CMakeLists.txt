cmake_minimum_required(VERSION 3.5)

project(simulator LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

add_subdirectory(mimalloc)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)


add_executable(simulator
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    global.cpp
    global.h
    instrcution.cpp
    instrcution.h
        instruction_impl.cpp instruction_impl.h mainwindow.ipp)

target_link_libraries(simulator PRIVATE Qt5::Widgets mimalloc-static)
