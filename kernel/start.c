#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/paging.h>
#include <n7OS/irq.h>
#include <n7OS/time.h>

void kernel_start(void)
{
    init_console();
    
    initialise_paging();
    
   

    //init_syscall();

    init_irq();

    init_time();

    sti();
    printf("test");


    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
