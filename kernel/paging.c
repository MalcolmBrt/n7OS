#include <n7OS/paging.h>
#include <stddef.h> // nécessaire pour NULL
#include <n7OS/kheap.h>
#include <n7OS/mem.h>
#include <string.h> // pour memset()
#include <n7OS/processor_structs.h>

PageDirectory repertoire_pages;

void initialise_paging() {
    uint32_t index = 0;
    init_kheap();
    init_mem();

    repertoire_pages = (PageDirectory)kmalloc_a(sizeof(PDE)*1024);

    // rempli les 4096 octets par des 0 par sécurité
    memset(repertoire_pages, 0, 4096);

    for (int i = 0; i < 1024; i++) {
        PageTable table_pages = (PageTable)kmalloc_a(sizeof(PTE)*1024);
        memset(table_pages, 0, 4096);

        repertoire_pages[i].page_entry.P = 1;
        repertoire_pages[i].page_entry.W = 1;
        repertoire_pages[i].page_entry.U = 1; // On ouvre le répertoire aux utilisateurs
        repertoire_pages[i].page_entry.Page = ((uint32_t)table_pages) >> 12;

        index = (uint32_t)table_pages + sizeof(PTE)*1024;
    }

    for (int i = 0; i < index; i += sizeof(PTE)*1024) {
        alloc_page_entry(i, 1, 1);
    }

    // On met l'adresse du répertoire dans CR3
    __asm__ __volatile__("mov %0, %%cr3" ::"r"(repertoire_pages));
    // On met le bit 31 de CR0 à 1. Ce bit permet de valider la pagination. (https://www.gladir.com/CODER/ASM8086/cr0.htm)
    uint32_t cr0;
    __asm__ __volatile__("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ __volatile__("mov %0, %%cr0" ::"r"(cr0));

    setup_base((uint32_t)repertoire_pages);
}

// cette fonction lie une vrai page de mémoire physique à une adresse virtuelle
PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel) {
    // on garde les 10 bits de gauche, décalage de 22 bits vers la droite
    uint32_t rep_index = address >> 22;
    // on garde les 10 bits du milieu, décalage de 12 bits 
    // et on applique le masque 0x03FF (10 bits de poids faible à 1, reste à 0)
    uint32_t table_index = (address >> 12) & 0x03FF;

    // On récupère l'adresse stockée dans le répertoire et on la re-décale vers la gauche
    PageTable pgtab = (PageTable)(repertoire_pages[rep_index].page_entry.Page << 12);

    // Obtenir une page physique libre
    uint32_t phys_page = findfreePage();
    if (phys_page == 0xFFFFFFFF) {
        return NULL; // Plus de mémoire RAM physique disponible
    }

    // Configurer l'entrée dans la table avec la nouvelle page physique
    pgtab[table_index].page_entry.P = 1;
    pgtab[table_index].page_entry.W = is_writeable;
    pgtab[table_index].page_entry.U = ~is_kernel; 
    
    // Décaler l'adresse physique de 12 bits vers la droite pour le bit-field
    pgtab[table_index].page_entry.Page = (phys_page >> 12);

    return pgtab;
}