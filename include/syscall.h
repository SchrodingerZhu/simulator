//
// Created by schrodinger on 2/23/20.
//

#ifndef SIMULATOR_SYSCALL_H
#define SIMULATOR_SYSCALL_H

#define SYSCALL_PRINT_CHAR 11
#define SYSCALL_PRINT_INT 1
#define SYSCALL_PRINT_STRING 4

#define SYSCALL_READ_CHAR 12
#define SYSCALL_READ_INT 5
#define SYSCALL_READ_STRING 8

#define SYSCALL_MMAP 9
#define SYSCALL_EXIT 10

#define SYSCALL_OPEN 13
#define SYSCALL_READ 14
#define SYSCALL_WRITE 15
#define SYSCALL_CLOSE 16
#define SYSCALL_EXIT2 17

#define SYSCALL_FAST_COPY 10000
#define SYSCALL_UI_OPEN_FILE 10001
#define SYSCALL_MUNMAP 10002

#endif //SIMULATOR_SYSCALL_H
