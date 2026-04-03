#include <n7OS/time.h>
#include <inttypes.h>
#include <n7OS/cpu.h>
#include <n7OS/irq.h>
#include <n7OS/console.h>
#include <stdio.h>

extern void handler_IT_TIME();

uint32_t system_ticks = 0;

void init_time() {
    // Channel 0, accès poids faible/poids fort, générateur d'impulsion, fréquence définie en binaire
    outb(0x34, 0x43);
    // Affectation de la fréquence, poids faible, au Channel 0
    outb(FREQUENCE & 0xFF, 0x40);
    // puis les 8 bits de poids fort
    outb(FREQUENCE >> 8, 0x40);
    init_irq_entry(32, (uint32_t) handler_IT_TIME);
    outb(inb(0x21) & 0xFE, 0x21);
}

void show_time() {
  uint16_t oldpos = get_cursor();

  put_cursor(72);

  uint32_t total_secondes = system_ticks / 1000;
  uint32_t heures   = total_secondes / 3600;
  uint32_t minutes  = (total_secondes % 3600) / 60;
  uint32_t secondes = total_secondes % 60;

  printf("%02d:%02d:%02d", heures, minutes, secondes);

  put_cursor(oldpos);
}

void handler_time() {
    system_ticks++;

    // Tous les 1000 tics (donc 1 seconde), on affiche l'uptime
    if (system_ticks % 1000 == 0) {
        show_time();
    }

    // Acquittement de l'interruption au PIC
    outb(0x20, 0x20);
}