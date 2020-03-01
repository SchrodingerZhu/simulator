#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QListWidget>
#include <memory>
#include <QThread>
#include <QTimer>
#include "instruction.h"
#include "heap.h"
#include "stack.h"
/*!
 * The Qt specified namespace for UI components
 */
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow;

class Executor;

/*!
 * MainWindow of the GUI program.
 * It also contains the following component:
 * - a heap to simulate heap allocations
 * - a stack to simulate stack operations
 * - an executor to store and run simulation steps
 * @attention we keep all fields in public to reduce the complexity of development
 */
class MainWindow : public QMainWindow {
Q_OBJECT
public:
    /*!
     * construct the MainWindow
     * @param parent the parent widget, set as nullptr by default.
     *
     * On initialization, it will:
     * - adjust the scale factor of the tables and lists.
     * - disable execution related buttons
     * - set initial values of the registers and other simulation storage
     * - capture all `SIGSEGV` signals and throw errors on caught
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /*!
     * deconstruct the MainWindows, this will:
     * - delete all ui components in the tree
     * - delete the executor
     * - the stack and heap space are freed automatically (since they are normal data members)
     */
    ~MainWindow() override;

private slots:

    /*!
     * The slot for the `aboutButton`, on click, it will pop up a about window with icon and basic information.
     */
    void on_aboutButton_clicked();

    /*!
     * The slot for the `openButton`, on click, it will open a file chooser for the user to choose the compiled asm file.
     * @attention Notice that if the file format is not correct, an error will be raised will a warning dialog.
     */
    void on_openButton_clicked();

    /*!
     * The slot for the `translateButton`, on click, it will start the parallel translation.
     * After translation, it will enable those execution related buttons.
     * It will free the current executor and try to create a new one.
     * @attention Notice that if a binary string is not recognized, the translation procedure will not stop immediately.
     * It will store the error and give a whole summary after all lines are checked and translated.
     *
     */
    void on_translateButton_clicked();

    /*!
     * The slot for the `executionButton`, on click, it will start the execution.
     * It will also:
     * - set the time interval set in the `delay`
     * - disable buttons and inputs that will change the execution behavior
     * - enable the `stopButton`
     */
    void on_executeButton_clicked();

    /*!
     * The slot for the `stepButton`, on click, it will do a single line execution.
     */
    void on_stepButton_clicked();

    /*!
     * The slot for the `stepButton`, on click, it will reset all state.
     * @attention Noitce that `reset` will also clear all instructions, you will need to open your file again
     */
    void on_resetButton_clicked();

    /*!
     * The slot for the `stepButton`, on click, it will stop the execution.
     * The `PC` will remain unchanged and the execution can be resumed.
     */
    void on_stopButton_clicked();

public:
    /*!
     * The accumulator for multiplication related operations
     */
    union {
        /// stands for the whole accumulator
        uint64_t all;
        struct LOW_HIGH {
            /// stands for the low part of the accumulator
            uint32_t low;
            /// stands for the high part of the accumulator
            uint32_t high;
        } part;
    } ACC;
    /// The pointer to the Qt UI components
    Ui::MainWindow *ui;
    /// The pointer to the executor
    Executor *executor = nullptr;
    /// An indicator to check whether we should advance the counter or not.
    /// Used for jumping related instructions.
    bool advanceCounter = true;
    /// A timer to ignite periodical signals on execution
    QTimer timer;
    /// The vector for instruction storage
    QVector<Instruction> instructions{};
    /// The stack operation simulator.
    Stack stack;
    /// The heap operation simulator.
    Heap heap;
    /// The array for all registers.
    uint32_t REGS[32] = {};
    /// The program counter.
    size_t PC = 0;

    /*!
     * Pop up a warning dialog.
     * @param str the warning message
     */
    void showWarning(QString str);

    /*!
     * Update the target register value.
     * @param no register number
     * @param value new value
     * @param brush the texture brush (to indicate there is a modification on this register)
     * @param init whether this is a initialization (may lead to different behavior when updating the stack pointer)
     */
    void updateRegValue(int no, uint32_t value, const QBrush &brush = QBrush("red"), bool init = false);

    /*!
     * Update the program counter.
     * @param value new value
     */
    void updateProgramCounter(size_t value);

    /*!
     * Allocate some new memory on heap. This operation will update the UI components in the same time.
     * @attention this will truly invoke the `mmap` syscall in your bare metal machine.
     * For x64_64 linux target, we are using `MAP_32BIT` as the flag to force the system give an
     * available address in the first 2GiB area. In the simulation, we are actually storing the size in an associate set.
     * The allocation and deallocation operations are much slower than the real world, but as it is just a simulator with heavy
     * UI animations, the cost is bearable.
     * @param size the required size of the allocation
     * @return the pointer to the beginning of the newly allocated memory.
     */
    uint32_t allocHeap(size_t size);

    /*!
     * Deallocate some memory on heap. This operation will update the UI components in the same time.
     * @param addr the address to dealloc
     * @attention In our simulator, the following things are required:
     * - the pointer should point to the start position of the memory block
     * - the memory is managed by the heap
     */
    void deallocHeap(size_t addr);

    /*!
     * Read a target word from heap.
     * @tparam T primitive word type
     * @param addr the target address
     * @return the required word value
     */
    template<class T>
    T fetchHeap(uint32_t addr);

    /*!
     * Modify the target word cell on heap.
     * @tparam T T primitive word type
     * @param addr the target address
     * @param value the new value
     */
    template<class T>
    void editHeap(uint32_t addr, T value);

    /*!
     * Increase the stack size by the given amount. This will update the UI at the same time.
     * @param n the amount to increase
     */
    void increaseStack(size_t n);

    /*!
     * Decrease the stack size by the given amount. This will update the UI at the same time.
     * @param n the amount to decrease
     */
    void decreaseStack(size_t n);

    /*!
     * get the reference of the T value at an address on stack
     * @tparam T primitive type
     * @param addr virtual address on stack
     * @return the reference to the target data
     */
    template<class T>
    T &fetchStack(uint32_t addr);

    /*!
     * Edit the stack value and update the ui display
     * @tparam T primitive type
     * @param addr virtual address on stack
     * @param value the value to be set
     */
    template<class T>
    void editStack(uint32_t addr, T value);

    /*!
     * Check whether a address is currently within the stack range.
     * @param addr the address to check
     *
     */
    bool inStack(uint32_t addr);

    void updateStack(uint32_t addr, size_t size);

    void updateLow(uint32_t value);

    void updateHigh(uint32_t value);

    void updateAcc(uint64_t value);

    void translateAll();

    void resetAll();

    template<class T>
    T *getRealAddr(uint32_t addr);

    void handleSyscall();
};


#endif // MAINWINDOW_H
