// Mostafa Ibrahim 22010677
/* 
   pintos -v --filesys-size=2 -p ../examples/crack -a crack  -p
 ../examples/shellcode -a shellcode -- -f -q run 'shellcode'
   -*- Woahhh, have fun -*-
   http://www.phrack.org/issues.html?issue=49&id=14#article
   http://www.phrack.org/issues.html?issue=57&id=15#article
   Somewhat simpler to achieve in Pintos actually.
 */

/* pintos -v --filesys-size=2 -p ../../examples/shellcode -a shellcode -- -f -q run
shellcode| Thats the code i'll run */ 

#include <syscall.h>
#include <stdio.h>
#include <string.h>

// #if 0
/* This it the below assembly code in binary form. It runs. To get it,
 * just compile the code below and use the debugger to dump the code
 * in the main function. */
char shellcode[] =
  // "\x90\x90\x90\x90\x90\xe9\x0b\x00"
  // "\x00\x00\x6a\x02\xcd\x30\x31\xc0"
  // "\x50\x40\x50\xcd\x30\xe8\xf0\xff"
  // "\xff\xff""crack";


/*

The exit code is designed to be executed  and will cause the process to exit with a status code of 1. The program's main function outputs the size of the shellcode array before passing the location of the shellcode array to a shell function pointer and invoking it.
 The instructions of the code are:
xor%eax,%eax: This instruction performs a bitwise exclusive or (xor) operation between the contents of eax and itself to set the value of the eax register to 0.

inc %eax: increments the value of the eax register by 1.

mov %eax,%ebx: copies the value of the eax into the ebx register

int $0x80: This instruction generates a software interrupt with a value of 0x80. On a Linux system, this interrupt is used to invoke the kernel to perform various system calls. In this case, the value of the EBX register (which is 1) is interpreted as the number of the _exit system call, which causes the process to exit with the status code specified in the EAX register (which is also 1).

int $0x80: This command generates a software interrupt with the value 0x80, which is used to call the kernel on a Linux system in order to execute some different system calls. The process exits with a status code that is stored in the eax register. The value of the ebx register, which is 1, determines which system call to execute. In this case, the value of ebx is interpreted as the number of the _exit system call, which is also 1. Therefore, the process executes the _exit system call and exits with the status code indicated in the eax register.

*/

/*
The following code based on charles stevenson 
Refrences 
Stevenson,C., Linux/x86 -exit(1)-7 bytes.
Available from: https://shell-storm.org/shellcode/files/shellcode-55.html
 
*/
"\x31\xc0"              // xor  %eax,%eax
"\x40"                  // inc  %eax
"\x89\xc3"              // mov  %eax,%ebx
"\xcd\x80"              // int  $0x80
;


int main( void )
{
#if 1
 int *ret; /* A local variable is stored on the stack. */

  ret = (int *)&ret + 2;   /* Two steps above in the stack is the
                            * address to continue at when main
                            * return... the normal function return
                            * address. */
  (*ret) = (int)shellcode; /* We overwrite it with the address to the
                            * shell code. This will check that the
                            * shell code works as expected. */
  

#else
  /* Assembler code to do the following:
   *
   *  exec("crack");
   *  exit();
   *
   * Apparently the code 0x01 can not be sent as input to pintos, so
   * it can not be part of any instruction. Reason unknown. Theory:
   * 0x01 may be grabbed as Ctrl-a by QEMU ?
   *
   * Translate push 0x01 ==> ... push %eax
   *
   * The tricky part is to figure out at which address the name of the
   * program to start is stored. The call instruction solves it
   * nicely. It saves the following address as return address.
   */
  __asm__("jmp    0x0f;"             /* jump to CALL */
/* actual address of string pushed as return address by CALL */
          "push   $0x2;"             /* push EXEC syscall number */
          "int    $0x30;"            /* make syscall */
          "xor    %eax,%eax;"        /* load 0 in eax */
          "push   %eax;"             /* push exit_status */
          "inc    %eax;"             /* inc eax to 1 */
          "push   %eax;"             /* push EXIT syscall number */
          "int    $0x30;"            /* make syscall */
/* CALL */"call   -0x0C;"            /* jumps back again */
          ".string \"crack\";"       /* program to start */
    );
#endif
}
