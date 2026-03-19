#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/paging.h>

void kernel_start(void)
{
    setup_base(0 /* la memoire virtuelle n'est pas encore definie */);
    init_console();
    initialise_paging();

    // lancement des interruptions
    sti();

    printf("bonjour");
    
    //alloc_page_entry(0xA000000, 1, 0);
    uint32_t *ptr = (uint32_t *) 0xA000000;
    (*ptr)++; // pour simuler l'erreur, retirer alloc_page_entry




    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
