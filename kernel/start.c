#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/paging.h>

void kernel_start(void)
{
    init_console();
    initialise_paging();
    
    // 3. On configure les interruptions en leur donnant accès à notre mémoire virtuelle !
    setup_base((uint32_t) getRepPage());
    

    //init_syscall();

    //init_irq();

    sti();
    
    //alloc_page_entry(0xA000000, 1, 0);
    uint32_t *ptr = (uint32_t *) 0xA000000; // pour simuler l'erreur, retirer alloc_page_entry
    (*ptr)++;

    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
