#include <n7OS/paging.h>
#include <stddef.h> // nécessaire pour NULL

PageTable repertoire_pages;


void initialise_paging() {
    repertoire_pages = (PageTable) findfreePage();

    for (uint32_t i = 0; i < 1024; i++) {
        PageTable pt = (PageTable) findfreePage();
        for (uint32_t j = 0; j < 1024; j++) {
            
        }
    }

    PageTable first_table_pages = (PageTable) findfreePage();

    for (int i = 0; i < 1024; i++) {
        first_table_pages[i].value = 0;
    }

    

}

PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel) {
    PageTable pgtab= NULL;

    return pgtab;
}
