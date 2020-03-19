# MIPS Simulator
![](misc/mips.png) 
@image latex mips.png

A toy GUI-aided simulator for MIPS 2000 assembly language.

## Introduction
This is a toy simulator of MIPS 2000 assembly environment for CSC3050 assignment 2. An assembler is integrated into the simulator.
The simulator will first invoke the assembler to transform the assembly language into binary code. After that, all binary code will
be decoded and transform into predefined structs in `c++`. 

When simulation starts, the simulator will maintain the status in the registers and memory (stack/static/heap). All IO operations will be handled
by GUI events.

## Language Support
MIPS 2000 language without floating point instructions, coprocessor instructions and pesudo instructions.
The data part supports:
- word (array)
- halfword (array)
- byte (array)
- space (array)
- ascii
- asciiz

## Syscall List
|Name|Code|
|----|----|
|PRINT_INT| 1 |
|PRINT_STRING| 4 |
|READ_INT| 5 |
|READ_STRING| 8 |
|MMAP| 9 |
|EXIT| 10 |
|OPEN| 13 |
|READ| 14 |
|WRITE| 15 |
|CLOSE| 16 |
|EXIT2| 17 |
|FAST_COPY| 10000 |
|FILE_OPEN_DIALOG| 10001 |
|MUNMAP| 10002 |

## Build
### Build Environment
- Linux (64bit, GNU environment, Kernel > 3.0)
- GNU Toolchain (GCC and binary tools, Clang >= 6/GCC >= 7, GCC 9 is recommended)
- CMake (3.5 and above)
- Qt5

### Preparation
On Ubuntu 16.04, you can install the toolchains with the following code:
```bash
sudo apt update
sudo apt install clang-6.0 libomp-dev libomp5 
sudo apt-get update
sudo apt-get -y install clang-6.0 clang-6.0 libomp-dev libomp5 qt5-default qtbase5-dev
```

### Compilation
On Ubuntu 16.04,
```
mkdir build && cd build
env CC=clang-6.0 CXX=clang-6.0 cmake .. -DCMAKE_BUILD_TYPE=Release
make -j $(nproc)
```

### Special Notice
Older GCC on Ubuntu has a bug on `thread_local` linkage, hence it is recommended to use `llvm` toolchain on old distribution.


