//Mostafa Ibrahim 22010677
#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

// #include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "process.h"
#include "pagedir.h"
#include "threads/vaddr.h"
#include "devices/shutdown.h"
#include "filesys/filesys.h"

// import system calls declaration
#include "user/syscall.h"
// for the input function
#include "devices/input.h"
#include "threads/synch.h"

// lock for thread synchronization
struct lock mainLock;

void syscall_init (void);

#endif /* userprog/syscall.h */
