#ifndef MAINWINDOW_IPP
#define MAINWINDOW_IPP

#include "./ui_mainwindow.h"
#include "mainwindow.h"

template<class T>
void MainWindow::editStack(uint32_t addr, T value) {
    ::new(stack.get<T>(addr)) T(value);
    updateStack(addr, sizeof(T));
}

template<class T>
T &MainWindow::fetchStack(uint32_t addr) {
    return *stack.get<T>(addr);
}

template<class T>
void MainWindow::editHeap(uint32_t addr, T value) {
    ::new(reinterpret_cast<T *>(addr)) T(value);
}

template<class T>
T MainWindow::fetchHeap(uint32_t addr) {
    return *reinterpret_cast<T *>(addr);
}

template<class T>
T *MainWindow::getRealAddr(uint32_t addr) {
    switch (memoryType(addr)) {
        case STACK:
            return stack.get<T>(addr);
        case STATIC:
            return reinterpret_cast<T *>(frame.begin() + addr);
        default:
            return reinterpret_cast<T *>(addr);
    }
}

template<class T>
void MainWindow::edit(uint32_t addr, T value) {
    switch (memoryType(addr)) {
        case STACK:
            editStack(addr, value);
            break;
        default:
            ::new(getRealAddr<T>(addr)) T(value);
    }
}

#endif // MAINWINDOW_IPP




