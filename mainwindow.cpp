#pragma clang diagnostic push
#pragma ide diagnostic ignored "openmp-use-default-none"
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "global.h"
#include <QMessageBox>
#include <QFileDialog>
#include <iostream>
#include <memory>
#include "instruction_impl.h"
#include "mainwindow.ipp"
#include "instrcution.h"
#include <atomic>
#include "executor.h"
#include <cstring>
#include "syscall.h"

MainWindow* Executor::mainW = nullptr;
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
    ui->low->setReadOnly(true);
    ui->high->setReadOnly(true);
    ui->delay->setMaximum(5000);
    ui->delay->setMinimum(0);
    InstructionImpl::mainW = this;
    Executor::mainW = this;
    ui->stopButton->setDisabled(true);
    ui->executeButton->setDisabled(true);
    ui->stepButton->setDisabled(true);
    ui->translateButton->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete executor;
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
    resetAll();
    auto fileName = QFileDialog::getOpenFileName(this, tr("Open File"));
    if (fileName.isEmpty()) return;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        showWarning("failed to load file");
        return;
    }
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
    ui->translateButton->setDisabled(false);
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
    return addr < (size_t)heap.size();
}


void MainWindow::updateLow(uint32_t value)
{
    ACC.part.low = value;
    ui->low->setText(QString("0x%1").arg(QString::number(value, 16)));
}

void MainWindow::updateHigh(uint32_t value)
{
    ACC.part.high = value;
    ui->high->setText(QString("0x%1").arg(QString::number(value, 16)));
}

void MainWindow::updateAcc(uint64_t value)
{
    ACC.all = value;
    ui->low->setText(QString("0x%1").arg(QString::number(ACC.part.low, 16)));
    ui->high->setText(QString("0x%1").arg(QString::number(ACC.part.high, 16)));
}


#define CASE(NAME, TYPE) case TYPE##_##NAME:\
    executor->impls[i] = std::make_unique<NAME##Impl> (instr);\
    break;

#define RCASE(NAME) CASE(NAME, FCR)
#define IJCASE(NAME) CASE(NAME, OPC)

void MainWindow::translateAll()
{
    executor->impls.clear();
    QStringList errors;
    std::atomic_bool success { true };
    for(int i = 0; i < instructions.size(); ++i) {
        executor->impls.push_back(nullptr);
    }

#pragma omp parallel for
    for(int i = 0; i < instructions.size(); ++i) {
        auto instr = instructions[i];
        try {
            if (instr.__content == 0) {
                executor->impls[i] = std::make_unique<NOPImpl>(instr);
            }
            else if (resolv_type(instr) == R) {
                switch (instr.INST_R.f) {
                    RCASE(ADD) RCASE(ADDU) RCASE(AND) RCASE(BREAK)
                    RCASE(DIV) RCASE(DIVU) RCASE(JALR) RCASE(JR)
                    RCASE(MFHI) RCASE(MFLO) RCASE(MTHI) RCASE(MTLO)
                    RCASE(MULT) RCASE(MULTU) RCASE(NOR) RCASE(OR)
                    RCASE(SLL) RCASE(SLLV) RCASE(SLT) RCASE(SLTU)
                    RCASE(SRA) RCASE(SRAV) RCASE(SRL) RCASE(SRLV)
                    RCASE(SUB) RCASE(SUBU) RCASE(SYSCALL) RCASE(XOR)
                default:
                    throw std::runtime_error {"no such function code"};
                }
            } else {
                switch (instr.INST_I.op) {
                    case OPC_BGEZ: // BLEZ
                        if(instr.INST_I.t) executor->impls[i] = std::make_unique<BGEZImpl>(instr);
                        else executor->impls[i] = std::make_unique<BLTZImpl>(instr);
                        break;
                        IJCASE(J) IJCASE(JAL)
                        IJCASE(ADDI) IJCASE(ADDIU) IJCASE(ANDI) IJCASE(BEQ)
                        IJCASE(BGTZ) IJCASE(BLEZ) IJCASE(BNE) IJCASE(LB)
                        IJCASE(LBU) IJCASE(LH) IJCASE(LHU) IJCASE(LUI)
                        IJCASE(ORI) IJCASE(SB) IJCASE(SLTI) IJCASE(SLTIU)
                        IJCASE(SH) IJCASE(SW) IJCASE(XORI)
                    default: __builtin_unreachable();
                }
            }
            ui->instructions->item(i, 0)->setForeground(QBrush("green"));
        } catch (const std::runtime_error& e) {
            errors.push_back(QString("%1: %2").arg(QString::number(BASE_ADDR + (i << 2), 16))
                             .arg(e.what()));
            ui->instructions->item(i, 0)->setForeground(QBrush("red"));
            success = false;
        }
    }

    if (success) {
        QMessageBox box(this);
        box.setText("Success!");
        box.setIcon(QMessageBox::Information);
        box.exec();
        ui->executeButton->setDisabled(false);
        ui->stepButton->setDisabled(false);
        ui->translateButton->setDisabled(true);
        ui->instructions->setCurrentCell(0, 0);
    } else {
        showWarning(errors.join("\n"));
        delete executor;
        executor = nullptr;
    }
}

void MainWindow::on_translateButton_clicked()
{
    delete executor;
    executor = new Executor;
    translateAll();
}

void MainWindow::on_executeButton_clicked()
{
    QObject::connect(&timer, SIGNAL(timeout()), executor, SLOT(next()));
    QObject::connect(executor, SIGNAL(finished()), this, SLOT(on_stopButton_clicked()));
    timer.setInterval(ui->delay->text().toUInt());
    timer.start();
    ui->delay->setDisabled(true);
    ui->stopButton->setDisabled(false);
    ui->stepButton->setDisabled(true);
    ui->executeButton->setDisabled(true);
}

void MainWindow::on_stepButton_clicked()
{
    executor->next();
}

void MainWindow::resetAll() {
    if(timer.isActive()) timer.stop();
    updateProgramCounter(BASE_ADDR);
    delete executor;
    executor = nullptr;
    instructions.clear();
    ui->instructions->clear();
    for(auto i = 0; i < 32; ++i) {
        updateRegValue(i, 0, QBrush("green"), true);
    }
    updateRegValue(29, 0xFFFFFFFFu, QBrush("red"), true);
    stack.clear();
    heap.clear();
    ui->stack->clear();
    ui->heap->clear();
    ui->heapSize->setText(0);
    ACC.all = 0;
    ui->high->setText("0x0");
    ui->low->setText("0x0");
    ui->textOutput->clear();
    ui->executeButton->setText("Execute");
    ui->stopButton->setDisabled(true);
    ui->executeButton->setDisabled(true);
    ui->stepButton->setDisabled(true);
    ui->translateButton->setDisabled(true);
    ui->delay->setValue(0);
    ui->instructions->setCurrentCell(0, 0);
}

void MainWindow::on_resetButton_clicked()
{
    resetAll();
}

void MainWindow::on_stopButton_clicked()
{
    timer.stop();
    ui->stopButton->setDisabled(true);
    ui->executeButton->setDisabled(false);
    ui->stepButton->setDisabled(false);
    ui->delay->setDisabled(false);
}
#define HANDLE(NAME, BLOCK)\
    case SYSCALL_##NAME:\
        BLOCK\
        break;

void MainWindow::handleSyscall() {
    switch(REGS[2]) {
        HANDLE(EXIT, { executor->exit(0); })
        HANDLE(SBRK, {
            uint32_t size = REGS[4];
            uint32_t addr = heap.size();
            increaseHeap(size);
            updateRegValue(2, addr);
        })
    }

}

#pragma clang diagnostic pop
