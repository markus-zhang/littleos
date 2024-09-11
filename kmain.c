#include "io.h"
#include "timer.h"
#include "serial.h"

int 
sum_of_three(int arg1, int arg2, int arg3);

void
test_serial();

int
kmain()
{
    // 0xB8000 -> Starting address of 80*25 framebuffer
    // Bit:     | 15 14 13 12 11 10 9 8 | 7 6 5 4 | 3 2 1 0 |
    // Content: | ASCII                 | FG      | BG      |
    char* fb = (char *) VGA_ADDRESS;

    // Clear screen
    for (int i = 0; i < 80 * 25 * 2; i += 2)
    {
        fb_write_cell(fb, i, ' ', COLOR_BLACK, COLOR_WHITE);
    }

    // Write on first row
    fb_write_cell(fb, 0, 'H', COLOR_BLACK, COLOR_WHITE);
    fb_write_cell(fb, 2, 'e', COLOR_BLACK, COLOR_WHITE);
    fb_write_cell(fb, 4, 'l', COLOR_BLACK, COLOR_WHITE);
    fb_write_cell(fb, 6, 'l', COLOR_BLACK, COLOR_WHITE);
    fb_write_cell(fb, 8, 'o', COLOR_BLACK, COLOR_WHITE);
    fb_write_cell(fb, 10, ' ', COLOR_BLACK, COLOR_WHITE);
    fb_write_cell(fb, 12, 'w', COLOR_BLACK, COLOR_WHITE);
    fb_write_cell(fb, 14, 'o', COLOR_BLACK, COLOR_WHITE);
    fb_write_cell(fb, 16, 'r', COLOR_BLACK, COLOR_WHITE);
    fb_write_cell(fb, 18, 'l', COLOR_BLACK, COLOR_WHITE);
    fb_write_cell(fb, 20, 'd', COLOR_BLACK, COLOR_WHITE);

    // Setup cursor location
    set_cursor_offset(1, 0);

    // cursor_offset = get_cursor_offset();

    // cursor_offset += 0;

    // printk('M');

    // char buf[12] = "I love you!";
    // buf[11] = 0;

    // BEGIN TEST prints() ---------------------------
    // int length = 2561;

    // char buf_long[length];
    // for (int i = 0; i < length; i++)
    // {
    //     *(buf_long + i) = 0;
    // }

    // char c = 'A';
    // for (int i = 0; i < length; i++)
    // {
    //     *(buf_long + i) = c + i / 160;
    // }
    // buf_long[length - 1] = 0;

    // prints(buf_long, length - 1);
    // --------------------------- END TEST prints()

    
    
    // BEGIN TEST serial port ---------------------------
    // for (int i = 0; i < 20; i++)
    // {
    //     test_serial();
    // }
    // --------------------------- END TEST serial port




    // BEGIN TEST print integer functions -------------------------
    unsigned long v = 123456;
    printul(v);
    // --------------------------- END TEST print integer functions

    return 0;
}

int 
sum_of_three(int arg1, int arg2, int arg3)
{
    return arg1 + arg2 + arg3;
}

void
test_serial()
{
    serial_configure_baud_rate(SERIAL_COM1_BASE, 2);

    serial_configure_line(SERIAL_COM1_BASE);

    serial_configure_fifo(SERIAL_COM1_BASE);

    serial_configure_modem(SERIAL_COM1_BASE);

    write_serial('H');
    write_serial('e');
    write_serial('l');
    write_serial('l');
    write_serial('o');
    write_serial(',');
    write_serial(' ');
    write_serial('w');
    write_serial('o');
    write_serial('r');
    write_serial('l');
    write_serial('d');
}