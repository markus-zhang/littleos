#include "io.h"
#include "timer.h"

unsigned short cursor_offset = 0;

void 
outb(short port, char value) 
{
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

unsigned char 
inb(unsigned short port) 
{
    unsigned char result;
    __asm__ volatile (
        "inb %1, %0"        // Read a byte from the I/O port
        : "=a"(result)      // Output to the AL register (8-bit register of AX)
        : "d"(port)         // Input from the DX register (I/O port address)
    );
    return result;           // Return the read byte
}

/** Get cursor offset
The cursor position in text mode is a 16-bit value, 
split across two VGA registers:

Cursor Location High Byte: Stored in VGA register 0x0E.
Cursor Location Low Byte: Stored in VGA register 0x0F.

To get the current cursor position:

Read the high byte from register 0x0E using I/O ports 0x3D4 and 0x3D5.
Read the low byte from register 0x0F using the same ports.
Use 0x0E to "trigger" high byte and 0x0F to "trigger" low byte
*/
unsigned short
get_cursor_offset()
{
    short offset = 0;
    unsigned char high = 0;
    unsigned char low = 0;

    __asm__ volatile ("outb %0, %1" : : "a"((char)0x0E), "Nd"((short)0x3D4));
    __asm__ volatile ("inb %1, %0" : "=a"(high) : "Nd"((short)0x3D5));
    // Move high byte to higher 8 bits
    offset += 256 * high;

    __asm__ volatile ("outb %0, %1" : : "a"((char)0x0F), "Nd"((short)0x3D4));
    __asm__ volatile ("inb %1, %0" : "=a"(low) : "Nd"((short)0x3D5));
    offset += low;

    return offset;
}

/** Set cursor to row-column
The cursor’s position is determined with a 16 bits integer: 
0 means row zero, column zero; 
1 means row zero, column one; 
80 means row one, column zero and so on.

For a 16-bit integer say 0x01A0, we need to send it twice:
out 0x3D4, 14   ; 14 tells the framebuffer to expect the highest 8 bits
out 0x3D5, 0x01 ; sending the highest 8 bits of 0x01A0
out 0x3D4, 15   ; 15 tells the framebuffer to expect the lowest 8 bits
out 0x3D5, 0xA0 ; sending the lowest 8 bits of 0x01A0
*/
void
set_cursor_offset(int row, int column)
{
    // row 0 column 0 is the 16-bit integer 0x0000
    // row 10 column 5 is the 16-bit integer 10*80+5 = 806
    int offset = row * 80 + column;
    char high = offset >> 8;
    char low = offset & 0x00FF;

    outb(0x3D4, 14);
    outb(0x3D5, high);
    outb(0x3D4, 15);
    outb(0x3D5, low);
    cursor_offset = offset;
}

// set_cursor() actually just show cursor at cursor_offset
// TODO: rename this function
void
set_cursor()
{
    char high = cursor_offset >> 8;
    char low = cursor_offset & 0x00FF;

    outb(0x3D4, 14);
    outb(0x3D5, high);
    outb(0x3D4, 15);
    outb(0x3D5, low);
}

/**
* Essentially just copy row 1-24 to row 0-23 and clear row 24 
*/
void
scroll_text_up()
{
    char* fb = (char *) VGA_ADDRESS;

    // 24 * 80 = 1920 characters to copy, each has 2 bytes
    for (int row = 0; row <= 24; row ++)
    {
        for (int col = 0; col < 160; col += 2)
        {
            int des_offset = row * 160 + col;
            int src_offset = des_offset + 160;
            *(fb + des_offset) = *(fb + src_offset);
            *(fb + des_offset + 1) = *(fb + src_offset + 1);
        }
    }

    // Clear row 24
    for (int col = 0; col < 160; col ++)
    {
        *(fb + 1920 * 2 + col) = 0;
    }
}

/** fb_write_cell:
* Writes a character with the given foreground and background to position i
* in the framebuffer.
*
* @param fb Framebuffer base address
* @param i The location in the framebuffer
* @param c The character
* @param fg The foreground color
* @param bg The background color
*/
void
fb_write_cell(char* fb, int i, char c, unsigned char fg, unsigned char bg)
{
    fb[i] = c;
    fb[i+1] = ((fg & 0x0F) << 4) | (bg & 0x0F);

}

// print a char at cursor
// Get the offset from high and low, and then 
// TODO: Scroll screen up if offset is "overflowed"
int 
printk(char c)
{
    // cursor_offset = get_cursor_offset();
    char* fb = (char *) VGA_ADDRESS;

    fb_write_cell(fb, cursor_offset * 2, c, COLOR_BLACK, COLOR_WHITE);
    cursor_offset ++;
    set_cursor();

    return 0;
}

// print a string at cursor
/**
We need to track the cursor position but do not need to update it until done.
If cursor_position divides 80, we don't need to do anything as we use the offset
to display characters.
However, if cursor_position >= 80 * 25, we need to make the following changes:
- The bottom 80*24 is to be copied to the top 80*24
- Next character shows up at last row, column 0
*/
int 
prints(char *buf, unsigned int len)
{
    cursor_offset = get_cursor_offset();
    long char_printed = 0;
    long count = 0;
    unsigned short free_space = FRAMEBUFFER_LEN - cursor_offset;
    len = 0;    // unused, delete
    len += 1;

    char* fb = (char *) VGA_ADDRESS;

    while (*(buf + count) != 0)
    {
        // Set offset + char_printed as *(buf + char_printed)
        char c = *(buf + count);
        fb_write_cell(fb, (cursor_offset + char_printed) * 2, c, COLOR_BLACK, COLOR_WHITE);

        char_printed ++;
        count ++;

        // If whole screen is full, move up one row
        if (char_printed >= free_space)
        {
            // Copy row 1-24 to row 0-23
            delay(400000);
            scroll_text_up();
            free_space = 80;    // The bottom row is free again
            char_printed = 0;   // Reset number of chars printed
            cursor_offset = FRAMEBUFFER_LEN - free_space;   // Start printing from row 24 col 0

        }
    }

    cursor_offset += char_printed;

    set_cursor();

    return 0;
    
}

long
get_string_len(char *buf)
{
    long len = 0;
    while (*(buf++) != 0)
    {
        len++;
    }
    return len;
}

/** Print an unsigned long at cursor
*   v is between 0 and 0xFF(255)
*/
// int
// printul(unsigned long v)
// {
//     unsigned long div = 10;
//     unsigned long d = v / div;
//     unsigned long r = v - d * div;
//     // Largest unsigned long < 10^19
//     char temp[19];
//     unsigned char count = 0;

//     while (1)
//     {
//         temp[count] = 
//     }
// }

void
printul(unsigned long v)
{
    unsigned long div = 10;
    unsigned char temp[19];
    int i = 0;
    for (; v != 0; i++)
    {
        unsigned long d = v / div;
        unsigned char r = v % div;
        temp[i] = r;
        v = d;
    }

    // i got one extra value
    i --;
    for (; i >= 0; i--)
    {
        printuc(temp[i]);
    }
}

void
printuc(unsigned char v)
{
    printk(v + '0');
}