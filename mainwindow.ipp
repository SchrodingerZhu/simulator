#ifndef MAINWINDOW_IPP
#define MAINWINDOW_IPP
#include "./ui_mainwindow.h"
#include "mainwindow.h"

template<class T>
void MainWindow::editStack(size_t _n, T value)
{
    auto n = 0xffffffffu - _n;
    if (sizeof(T) - 1 > n || n >= stack.size()) {
        throw std::runtime_error {"invalid stack access: write"};
    }
    ::new (reinterpret_cast<T *>(stack.begin() + n + 1 - sizeof(T))) T(value);
    for(auto i = n + 1 - sizeof(T); i <= n; ++i) {
        ui->stack->item(stack.size() - i - 1)->setText(QString::number(stack[i], 2).rightJustified(8, '0'));
    }
}

template<class T>
T MainWindow::fetchStack(size_t _n)
{
    auto n = 0xfffffffu - _n;
    if (sizeof(T) - 1 > n || n >= stack.size()) {
        throw std::runtime_error {"invalid stack access: read"};
    }
    return *reinterpret_cast<T *>(stack.begin() + n + 1 - sizeof(T));
}

template<class T>
void MainWindow::editHeap(size_t n, T value)
{
    if (n + sizeof(T) > (size_t)heap.size()) {
        throw std::runtime_error {"invalid heap access: write"};
    }
    ::new (reinterpret_cast<T *>(heap.begin() + n)) T(value);
    for(auto i = n; i < n + sizeof(T); ++i) {
        this->ui->heap->item(i)->setText(QString::number(heap[i], 2).rightJustified(8, '0'));
    }
}

template<class T>
T MainWindow::fetchHeap(size_t n)
{
    if (n + sizeof(T) > heap.size()) {
        throw std::runtime_error {"invalid heap access: read"};
    }
    return *reinterpret_cast<T *>(heap.begin() + n);
}

#endif // MAINWINDOW_IPP