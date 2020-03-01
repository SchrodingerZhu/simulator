#ifndef EXECUTOR_H
#define EXECUTOR_H
#include "instruction_impl.h"
/*!
 * Executor is a helper class for us to store compiled instructions and enable slots for automatic execution.
 */
class Executor : public QObject {
Q_OBJECT
public:
    /// a pointer back to the MainWindow, provides APIs on the UI components.
    static MainWindow *mainW;
    /// a set of pointers to the translated result of the machine code.
    std::vector<_SIM::InstrPtr> impls;
public slots:

    /*!
     * When this slot is invoked, it will execute the instruction at the current PC position.
     * When PC touches the bottom line, it will emit a finish signal.
     * @attention `std::runtime_error` and `SIGSEGV` will be caught within the range.
     */
    void next();

    /*!
     * Exit the execution with the given code. (Information is shown)
     * It will also emit a finish signal on the closing of the information dialog.
     * @param code return code
     */
    void exit(int code = 0);

signals:

    /*!
     * This signal will be emitted when the execution is finished.
     */
    void finished();
};

#endif // EXECUTOR_H
