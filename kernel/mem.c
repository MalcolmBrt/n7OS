#include <n7OS/mem.h>

uint32_t free_page_bitmap_table[PAGES_TABLE_SIZE];

/**
 * @brief Marque la page allouée
 * 
 * Lorsque la page a été choisie, cette fonction permet de la marquer allouée
 * 
 * @param addr Adresse de la page à allouer
 */
void setPage(uint32_t addr) {
    uint32_t page_index = addr / PAGE_SIZE;
    uint32_t row = page_index / 32;
    uint32_t col = page_index % 32;
    free_page_bitmap_table[row] = free_page_bitmap_table[row] | (1 << col);
}

/**
 * @brief Désalloue la page
 * 
 * Libère la page allouée.
 * 
 * @param addr Adresse de la page à libérer
 */
void clearPage(uint32_t addr) {
    uint32_t page_index = addr / PAGE_SIZE;
    uint32_t row = page_index / 32;
    uint32_t col = page_index % 32;
    // l'opération ~ permet d'inverser tous les bits
    free_page_bitmap_table[row] = free_page_bitmap_table[row] & ~(1 << col);
}

/**
 * @brief Fourni la première page libre de la mémoire physique tout en l'allouant
 * 
 * @return uint32_t Adresse de la page sélectionnée
 */
uint32_t findfreePage() {
    for (int row = 0; row < PAGES_TABLE_SIZE; row++) {
        for (int col = 0; col < 32; col++) {
            // ET logique pour vérifier si le jème bit est a 0
            if ((free_page_bitmap_table[row] & (1 << col)) == 0) {
                // alloue la page
                free_page_bitmap_table[row] = free_page_bitmap_table[row] | (1 << col);
                // calcul l'adresse
                uint32_t page_index = row * 32 + col;
                return page_index * PAGE_SIZE;
            }
        }
    }
    return 0xFFFFFFFF;
}

/**
 * @brief Initialise le gestionnaire de mémoire physique
 * 
 */
void init_mem() {
    // initialise toutes les pages comme libre
    for (int i = 0; i < PAGES_TABLE_SIZE; i++) {
        free_page_bitmap_table[i] = 0;
    }
}

/**
 * @brief Affiche l'état de la mémoire physique
 * 
 */
void print_mem() {
    
}