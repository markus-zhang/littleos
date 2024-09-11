#ifndef IO_H
#define IO_H

#define VGA_ADDRESS     0xB8000
#define COLOR_BLACK     0
#define COLOR_WHITE     15
#define FRAMEBUFFER_LEN 2000

extern unsigned short cursor_offset;

unsigned short
get_cursor_offset();

void 
outb(short port, char value);

unsigned char 
inb(unsigned short port);

void
set_cursor_offset(int row, int column);

void
set_cursor();

// Scroll the text screen up for one row
void
scroll_text_up();

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
fb_write_cell(char* fb, int i, char c, unsigned char fg, unsigned char bg);

// print a char at cursor
int 
printk(char c);

// print a string at cursor
int 
prints(char *buf, unsigned int len);

long
get_string_len(char *buf);

void
printul(unsigned long v);

void
printuc(unsigned char v);

#endif