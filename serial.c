#include "io.h"
#include "serial.h"

/**
* The divisor must be a 16-bit number,
* but we can only send 8-bit at a time.
* We first send the DLAB enable signal to the line control register,
* then send the higher 8-bit, then the lower 8-bit.
* For example, if the divisor is 0x010A, the higher 0x01 is first sent,
* and then the lower 0x0A is sent.
*/
void 
serial_configure_baud_rate(unsigned short com, unsigned short divisor)
{
    outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
    outb(SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00FF);
    outb(SERIAL_DATA_PORT(com), divisor & 0x00FF);
}

/* Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
* Content:  | d | b | prty  | s | dl |
* Value:    | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
*/
void 
serial_configure_line(unsigned short com)
{
    outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_CONFIG);
}

/* Bit:     | 7 6 | 5   | 4 | 3     | 2     | 1     | 0 |
*  Content: | lvl | bs  | r | dma   | clt   | clr   | e |
*/
void 
serial_configure_fifo(unsigned short com)
{
    outb(SERIAL_FIFO_COMMAND_PORT(com), SERIAL_LINE_FIFO_CONFIG);
}

/* Bit:     | 7 | 6 | 5     | 4     | 3     | 2     | 1     | 0     |
*  Content: | r | r | af    | lb    | ao2   | ao1   | rts   | dtr   |
*/
void 
serial_configure_modem(unsigned short com)
{
    outb(SERIAL_MODEM_COMMAND_PORT(com), SERIAL_LINE_MODEM_CONFIG);
}

/**
Line Status Register
The line status register is useful to check for errors and enable polling.

Bit	Name	        Meaning
0	Data ready (DR)	                            Set if there is data that can be read
1	Overrun error (OE)	                        Set if there has been data lost
2	Parity error (PE)	                        Set if there was an error in the transmission as detected by parity
3	Framing error (FE)	                        Set if a stop bit was missing
4	Break indicator (BI)	                    Set if there is a break in data input
5	Transmitter holding register empty (THRE)	Set if the transmission buffer is empty (i.e. data can be sent)
6	Transmitter empty (TEMT)	                Set if the transmitter is not doing anything
7	Impending Error	                            Set if there is an error with a word in the input buffer 

If bit-5 is 1, then buffer is empty -> ready to send
*/
int 
is_serial_buffer_empty(unsigned short com)
{
    return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

void 
write_serial(char c) 
{
   while (is_serial_buffer_empty(SERIAL_COM1_BASE) == 0);

   outb(SERIAL_COM1_BASE, c);
}