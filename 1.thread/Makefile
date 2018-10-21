
SRCS = $(wildcard *.c)
HEADS = $(wildcard *.h)

CXX = arm-agb-elf-g++
CC = arm-agb-elf-gcc



test: test.c thread.c thread.h int_warp.s int_handler.c thread_entry_thunk.s
	$(CC) test.c thread.c int_handler.c thread_entry_thunk.s int_warp.s -o test.elf -g

int_warp: int_warp.c 
	$(CC) int_warp.c -S -O

thread_entry_thunk: thread_entry_thunk.c
	$(CC) thread_entry_thunk.c -S -O
