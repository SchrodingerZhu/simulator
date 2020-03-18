#include "executor.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

void Executor::next() {
    if (((mainW->PC - BASE_ADDR) >> 2u) < (size_t) mainW->instructions.size()) {
        try {
            impls[(mainW->PC - BASE_ADDR) >> 2u]->exec();
        } catch (const std::runtime_error &error) {
            QDialog dialog(mainW);
            auto layout = new QVBoxLayout(&dialog);
            dialog.setWindowTitle("Exception Trap");
            auto label = new QLabel(error.what());
            auto resume = new QPushButton("Resume");
            auto abort = new QPushButton("Abort");
            layout->addWidget(label);
            layout->addWidget(resume);
            layout->addWidget(abort);
            connect(resume, SIGNAL(clicked()), &dialog, SLOT(accept()));
            connect(abort, SIGNAL(clicked()), &dialog, SLOT(reject()));
            dialog.setLayout(layout);
            dialog.exec();
            if (dialog.result() == QDialog::Rejected) {
                emit finished();
                return exit(1);
            }
        }
        if (mainW->advanceCounter) mainW->updateProgramCounter(mainW->PC + 4);
        mainW->ui->instructions->setCurrentCell((mainW->PC - BASE_ADDR) >> 2u, 0);
        mainW->advanceCounter = true;
    } else if (((mainW->PC - BASE_ADDR) >> 2u) == mainW->instructions.size()) {
        emit finished();
    } else {
        mainW->showWarning("invalid PC");
        emit finished();
    }
}

void Executor::exit(int code) {
    mainW->showWarning(QString("Program exit [code: %1]").arg(code));
    emit finished();
}


