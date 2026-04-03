#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/paging.h>

void kernel_start(void)
{
    init_console();
    initialise_paging();
    
   

    //init_syscall();

    init_irq();

    sti();

    // instruction assembleur int génère une interruption
    __asm__ __volatile__("int $50");

    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
