#ifndef MAINWINDOW_IPP
#define MAINWINDOW_IPP
#include "./ui_mainwindow.h"
#include "mainwindow.h"

template<class T>
void MainWindow::editStack(size_t _n, T value)
{
    auto n = 0xffffffffu - _n;
    if (sizeof(T) - 1 > n || n > stack.size()) {
        throw std::runtime_error {"invalid stack access: write"};
    }
    ::new (reinterpret_cast<T *>(stack.begin() + n - sizeof(T))) T(value);
    for(auto i = n - sizeof(T); i < n; ++i) {
        ui->stack->item(stack.size() - i - 1)->setText(QString::number(stack[i], 2).rightJustified(8, '0'));
    }
}

template<class T>
T MainWindow::fetchStack(size_t _n)
{
    auto n = 0xffffffffu - _n;
    if (sizeof(T) - 1 > n || n > stack.size()) {
        throw std::runtime_error {"invalid stack access: read"};
    }
    return *reinterpret_cast<T *>(stack.begin() + n - sizeof(T));
}

template<class T>
void MainWindow::editHeap(uint32_t addr, T value)
{
    ::new (heap.get<T>(addr)) T(value);
}

template<class T>
T MainWindow::fetchHeap(uint32_t addr)
{
    return *heap.get<T>(addr);
}

#endif // MAINWINDOW_IPP




