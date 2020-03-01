cmake_minimum_required(VERSION 3.5)

project(simulator LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(mimalloc)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories(include)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenMP REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
add_executable(simulator
        src/main.cpp
        src/mainwindow.cpp
        include/mainwindow.h
        include/mainwindow.ui
        src/global.cpp
        include/global.h
        src/instruction.cpp
        include/instruction.h
        include/executor.h
        src/executor.cpp
        include/heap.h
        src/heap.cpp
        misc/resources.qrc
        src/instruction_impl.cpp include/instruction_impl.h include/mainwindow.ipp include/syscall.h include/fs.h src/stack.cpp include/stack.h)

target_link_libraries(simulator PRIVATE Threads::Threads Qt5::Widgets mimalloc-static)
