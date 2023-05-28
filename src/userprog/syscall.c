// Mostafa Ibrahim 22010677
#include "userprog/syscall.h"


static void syscall_handler (struct intr_frame *);

bool isValid (void * vaddr);

void
syscall_init (void) 
{
  // initialize lock for any function that accesses/uses any file
  lock_init(&mainLock);
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f) 
{
  // Extrating Stack pointer
  int *stack_ptr = f->esp;
  
  /* If stack pointer is not valid, kill */
  if(!isValid (stack_ptr))
    exit(-1);
  
 
  int syscall_number = *stack_ptr;
  

  switch (syscall_number)
  {
    case SYS_HALT:
      halt();
      break;
		
		case SYS_EXIT:
      if(!isValid(stack_ptr+1))
        exit(-1);
			exit(*(stack_ptr+1));
      break;

    case SYS_CREATE:
      if(!isValid(stack_ptr+4) || !isValid(stack_ptr+5))
        exit(-1);
      lock_acquire(&mainLock);
      f->eax = create ((void*)(*((int*)f->esp + 4)), *((unsigned*)((int*)f->esp + 5)));
      lock_release(&mainLock);
      break;
    
    case SYS_REMOVE:
      if(!isValid(stack_ptr+1))
        exit(-1);
      lock_acquire(&mainLock);
      f->eax = remove((const char *)*(stack_ptr+1));
      lock_release(&mainLock);
      break;
    
    case SYS_READ:
      if (!isValid(stack_ptr+5) || !isValid (stack_ptr+6) || !isValid (stack_ptr+7))
        exit(-1);
      lock_acquire(&mainLock);
      f->eax=read(*(stack_ptr+5),(void *)*(stack_ptr+6),*(stack_ptr+7));
      lock_release(&mainLock);
      break;
		
		case SYS_WRITE:
      if (!isValid(stack_ptr+5) || !isValid(stack_ptr+6) || 
            !isValid (stack_ptr+7))
        exit(-1);
      lock_acquire(&mainLock);
      f->eax = write(*(stack_ptr+5),(void*)*(stack_ptr+6),*(stack_ptr+7));
      lock_release(&mainLock);
      break;

		default:
			exit(-1);      
			break;
  }

  // printf("syscall \n");
}

// shutdown
void halt (void)
{
  shutdown_power_off();
}

bool create (const char * file, unsigned initial_size)
{
  if(file == NULL) exit(-1);
  return filesys_create(file, initial_size);
}

bool remove (const char * file)
{
  if(file == NULL) exit(-1);
  return filesys_remove(file);
}

int read (int fd, void * buffer, unsigned size)
{
  unsigned len =0;

  //read input from terminal 
  if (fd == STDIN_FILENO)
  { 
    while (len < size)
    {
      *((char *)buffer+len) = input_getc();
      len++;
    }
    return len;
  }

  return len;
}

// Writes size bytes from buffer to the open file fd
int write (int fd, const void *buffer, unsigned size)
{
  if (fd == STDOUT_FILENO)
  {
    putbuf(buffer,size);
    return size;
  }

  return -1;
}


void exit (int status)
{
  thread_current()->exit_code = status; 
  thread_exit();
}

/* Checks for validity of a user addresses */
bool isValid(void * vaddr)
{
  return (is_user_vaddr(vaddr) && pagedir_get_page(thread_current()->pagedir,vaddr)!=NULL);
}
