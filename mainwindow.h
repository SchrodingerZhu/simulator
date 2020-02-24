#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QListWidget>
#include <memory>
#include <QThread>
#include <QTimer>
#include "instrcution.h"

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
    bool advanceCounter = true; /// @todo: THIS IS OF NO USE FOR NOW.
    QTimer timer;
    QVector<Instruction> instructions {};
    QVector<uint8_t> stack;
    QVector<uint8_t> heap;
    uint32_t REGS[32] = {};
    size_t PC = 0;
    size_t lastExec = BASE_ADDR;
    void showWarning(QString str);
    void updateRegValue(int no, uint32_t value, const QBrush& brush = QBrush("red"), bool init = false);
    void updateProgramCounter(size_t value);    

    //heap
    void increaseHeap(size_t n);
    void decreaseHeap(size_t n);
    template<class T>
    T fetchHeap(size_t n);
    template<class T>
    void editHeap(size_t n, T value);

    //stack
    void increaseStack(size_t n);
    void decreaseStack(size_t n);
    template<class T>
    T fetchStack(size_t n);
    template<class T>
    void editStack(size_t n, T value);

    bool inHeap(size_t addr);

    void updateLow(uint32_t value);
    void updateHigh(uint32_t value);
    void updateAcc(uint64_t value);
    void translateAll();
    void resetAll();
    void handleSyscall();
};



#endif // MAINWINDOW_H
