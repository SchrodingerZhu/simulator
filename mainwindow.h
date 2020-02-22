#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QListWidget>
#include <memory>
#include "instrcution.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override ;

private slots:
    void on_aboutButton_clicked();

    void on_openButton_clicked();

public:
    Ui::MainWindow *ui;
    QVector<Instruction> instructions {};
    QVector<uint8_t> stack;
    QVector<uint8_t> heap;
    uint32_t REGS[32] = {};
    size_t PC = 0;
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
};



#endif // MAINWINDOW_H
