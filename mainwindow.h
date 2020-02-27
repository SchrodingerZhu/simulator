#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QListWidget>
#include <memory>
#include <QThread>
#include <QTimer>
#include "instrcution.h"
#include "heap.h"
#include "stack.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow;
class Executor;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override ;
private slots:
    void on_aboutButton_clicked();

    void on_openButton_clicked();

    void on_translateButton_clicked();

    void on_executeButton_clicked();

    void on_stepButton_clicked();

    void on_resetButton_clicked();

    void on_stopButton_clicked();

public:
    union {
        uint64_t all;
        struct {
            uint32_t low;
            uint32_t high;
        } part;
    } ACC;
    Ui::MainWindow *ui;
    Executor* executor = nullptr;
    bool advanceCounter = true;
    QTimer timer;
    QVector<Instruction> instructions {};
    Stack stack;
    Heap heap;
    uint32_t REGS[32] = {};
    size_t PC = 0;
    void showWarning(QString str);
    void updateRegValue(int no, uint32_t value, const QBrush& brush = QBrush("red"), bool init = false);
    void updateProgramCounter(size_t value);    

    //heap
    uint32_t allocHeap(size_t size);
    void deallocHeap(size_t addr);
    template<class T>
    T fetchHeap(uint32_t addr);
    template<class T>
    void editHeap(uint32_t addr, T value);

    //stack
    void increaseStack(size_t n);
    void decreaseStack(size_t n);
    template<class T>
    T& fetchStack(uint32_t addr);
    template<class T>
    void editStack(uint32_t addr, T value);

    bool inStack(uint32_t addr);
    void updateStack(uint32_t addr, size_t size);
    void updateLow(uint32_t value);
    void updateHigh(uint32_t value);
    void updateAcc(uint64_t value);
    void translateAll();
    void resetAll();
    template<class T>
    T* getRealAddr(uint32_t addr);
    void handleSyscall();
};



#endif // MAINWINDOW_H
