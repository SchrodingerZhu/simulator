#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "global.h"
#include <QMessageBox>
#include <QFileDialog>
#include <iostream>
#include "instruction_impl.h"
#include "mainwindow.ipp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Simulator");
    ui->registers->setRowCount(32);
    ui->registers->setColumnCount(2);
    ui->registers->horizontalHeader()->setVisible(false);
    for(int i = 0; i < 32; ++i) {
        ui->registers->setVerticalHeaderItem(i, new QTableWidgetItem {QString::number(i)});
        ui->registers->setItem(i, 0, new QTableWidgetItem { REG_NAME[i] });
        ui->registers->setItem(i, 1, new QTableWidgetItem { "0" });
        ui->registers->item(i, 1)->setForeground(QBrush("green"));
    }
    ui->registers->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setFixedSize(this->maximumSize());
    ui->programCounter->setReadOnly(true);
    ui->textOutput->setReadOnly(true);
    ui->heapSize->setReadOnly(true);
    InstructionImpl::mainW = this;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_aboutButton_clicked()
{
    QMessageBox box(this);
    box.setWindowTitle("About");
    box.setIcon(QMessageBox::Information);
    box.setText("A MIPS Simulator for CSC3050 Assignment-2. \nAuthor: Yifan ZHU <i@zhuyi.fan>");
    box.exec();
}

void MainWindow::on_openButton_clicked()
{
    auto fileName = QFileDialog::getOpenFileName(this, tr("Open File"));
    if (fileName.isEmpty()) return;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        showWarning("failed to load file");
        return;
    }
    instructions.clear();
    while (!file.atEnd()) {
        auto res = file.readLine().trimmed();
        if (res.isEmpty()) continue;
        int i = 0;
        bool ok = false;
        uint32_t result;
        if (res.length() != 32) goto ERR_OPEN;
        for (; i < 32; ++i) {
            result = res.toUInt(&ok, 2);
            if (!ok) {
ERR_OPEN:       showWarning("Invalid File Format");
                return;
            }
        }
        instructions.push_back(Instruction { .__content = result });
    }
    ui->instructions->setRowCount(instructions.size());
    ui->instructions->setColumnCount(1);
    ui->instructions->horizontalHeader()->setVisible(false);
    auto m = BASE_ADDR;
    for(int i = 0; i < instructions.size(); ++i, m += 4) {
        ui->instructions->setVerticalHeaderItem(i, new QTableWidgetItem {QString::number(m, 16)});
        ui->instructions->setItem(i, 0, new QTableWidgetItem { QString::number(instructions[i].__content, 2).rightJustified(32, '0')});
    }
    ui->instructions->setColumnWidth(0, 10000);
    ui->instructions->setEditTriggers(QAbstractItemView::NoEditTriggers);
    updateRegValue(29, 0xFFFFFFFFu, QBrush("red"), true);
    updateProgramCounter(BASE_ADDR);
    heap.clear();
    stack.clear();
    ui->heap->clear();
    ui->stack->clear();
    ui->heapSize->setText("0");
}

void MainWindow::showWarning(QString str)
{
    QMessageBox box(this);
    box.setWindowTitle("Warning");
    box.setIcon(QMessageBox::Warning);
    box.setText(str);
    box.exec();
}

void MainWindow::updateRegValue(int no, uint32_t value, const QBrush& brush, bool init)
{
    static int lastUpdate = 0;
    auto temp = REGS[no];
    REGS[no] = value;
    ui->registers->item(lastUpdate, 1)->setForeground(QBrush("green"));
    lastUpdate = no;
    auto item = ui->registers->item(no, 1);
    item->setForeground(brush);
    item->setText(QString::number(value, 16));
    if (!init && no == 29) {
        if (temp < value) {
            decreaseStack(value - temp);
        } else {
            increaseStack(temp - value);
        }
    }
}

void MainWindow::updateProgramCounter(size_t value)
{
    ui->programCounter->setText(QString("0x%1").arg(QString::number(value, 16)));
    PC = value;
}

void MainWindow::increaseHeap(size_t n)
{
    heap.resize(heap.size() + n);
    ui->heapSize->setText(QString::number(heap.size()));
    for(auto i = 0; i < (int)n; ++i) {
        this->ui->heap->addItem(QString::number(0).rightJustified(8, '0'));
    }
}

void MainWindow::decreaseHeap(size_t n)
{
    if (heap.size() < (int)n) {
        throw std::runtime_error {"heap size cannot be decreased by that much"};
    }

    ui->heapSize->setText(QString::number(heap.size() - n));
    for(auto i = heap.size() - 1; i >= heap.size() - (int)n; --i) {
        ::delete this->ui->heap->takeItem(i);
    }
    heap.resize(heap.size() - n);
}

void MainWindow::increaseStack(size_t n)
{
    stack.resize(stack.size() + n);
    for(auto i = 0; i < (int)n; ++i) {
        this->ui->stack->insertItem(0, QString::number(0).rightJustified(8, '0'));
    }
}

void MainWindow::decreaseStack(size_t n)
{
    if (stack.size() < (int)n) {
        throw std::runtime_error {"stack size cannot be decreased by that much"};
    }
    for(size_t i = 0; i < n; ++i) {
        ::delete this->ui->stack->takeItem(0);
    }
    stack.resize(stack.size() - n);
}



bool MainWindow::inHeap(size_t addr) {
    return addr < heap.size();
}
