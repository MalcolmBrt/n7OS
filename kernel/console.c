#include <n7OS/console.h>
#include <n7OS/cpu.h>

// pour chaque case, 8 premiers bits correspondent à la couleur et 8 derniers correspondent au char
uint16_t *scr_tab;

void init_console() {
    scr_tab= (uint16_t *) SCREEN_ADDR;
    clear_screen();
}

void clear_screen() {
    for (int i=0; i < (VGA_HEIGHT*VGA_WIDTH); i++) {
        scr_tab[i] = (CHAR_COLOR << 8) | ' ';
    }
    put_cursor(0);
}

void put_cursor(uint16_t pos) { // 80*25 -> forcément sur 16 bits
    if (pos >= 0 && pos < VGA_HEIGHT*VGA_WIDTH) {
        outb(CMD_LOW, PORT_CMD);
        outb(pos&0xff, PORT_DATA); // agit comme un filtre en "masquant" les 8 derniers bits
        outb(CMD_HIGH, PORT_CMD);
        outb(pos>>8, PORT_DATA);
    }
}

uint16_t get_cursor() {
    uint16_t pos;
    outb(CMD_LOW, PORT_CMD);
    pos=inb(PORT_DATA);
    outb(CMD_HIGH, PORT_CMD);
    pos=inb(PORT_DATA)<<8|pos;
    return pos;
}

void console_putchar(const char c) {
    uint16_t pos = get_cursor();
    if (c > 31 && c < 127) {
        scr_tab[pos]= CHAR_COLOR<<8|c;
        put_cursor(pos+1);
    } else {
        switch (c)
        {
        case '\b':
            put_cursor(pos-1);
            break;
        case '\t':
            put_cursor(pos+8);
            break;
        case '\n':
            put_cursor(((pos / VGA_WIDTH) + 1) * VGA_WIDTH);
            break;
        case '\f':
            clear_screen();
            break;
        case '\r':
            put_cursor((pos / VGA_WIDTH) * VGA_WIDTH);
            break;
        default:
            break;
        }
    }
}

void console_putbytes(const char *s, int len) {
    for (int i= 0; i<len; i++) {
        console_putchar(s[i]);
    }
}