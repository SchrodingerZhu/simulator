# The Design Brochure
## The Procedure of Simulation
After all translation is done, the process goes like the following:
- Run the code at the current `PC`
  - Arithmetic Operations are done directly
  - Load/Store Operations are moved to corresponding memory handlers
  - Syscall Operations are moved to corresponding functions
- Update GUI if needed
- If exceptions/faults/exit occurs
- Advance PC if no special jumps happened

## The design of MMAP/MUNMAP Simulation
We use raw `mmap` syscall with `MMAP_32BIT` flags to directly handle the syscall. 
To maintain the GUI part, we use a tagged statistic red-black tree from GNU's `PBDS` library to
record the order of the memory begin positions. Hence, the size and memory blocks can be visualized
in a list widget of GUI

## SigFault Capturing
We directly use the signal capture funtions of low-level `c` library to handle the faults on memory.
However, if the target operations is too destructive, the simulator will quit directly without popping
a dialog.

## Code Deduplication
We use a lot of macros to generate the code for us. In fact, we have designed a simple DSL to help us 
declare and implement an instruction.

## Store the Instructions
To store the compiled instructions, we use the polymorphic classes. The binary code is "compiled" into unique pointer to 
the implementation classes. Hence, we do not need to do a lot of branches and dispatches in the running time.
