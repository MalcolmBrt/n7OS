#include <inttypes.h>
#include <n7OS/irq.h>


void init_irq_entry(int irq_num, uint32_t addr) {
    idt_entry_t *idt_table = (idt_entry_t *) idt;

    // Le masque & 0xFFFF permet de ne garder que la moitié droite de l'adresse
    idt_table[irq_num].offset_inf = addr & 0xFFFF;
    // On décale de 16 bits vers la droite pour amener la moitié gauche à droite
    idt_table[irq_num].offset_sup = (addr >> 16) & 0xFFFF;
    idt_table[irq_num].sel_segment = KERNEL_CS;
    idt_table[irq_num].zero = 0;
    idt_table[irq_num].type_attr = 0x8E;
}
