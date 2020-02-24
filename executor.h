#ifndef EXECUTOR_H
#define EXECUTOR_H
#include "instruction_impl.h"

class Executor : public QObject {
    Q_OBJECT
public:
    static MainWindow* mainW;
    std::vector<std::unique_ptr<InstructionImpl>> impls;
public slots:
    void next();
    void exit(int code = 0);
signals:
    void finished();
};

#endif // EXECUTOR_H
