/* The I/O ports */
/* All the I/O ports are calculated relative to the data port. This is because
* all serial ports (COM1, COM2, COM3, COM4) have their ports in the same
* order, but they start at different values.
*/
#define SERIAL_COM1_BASE 0x3F8 /* COM1 base port */
#define SERIAL_DATA_PORT(base) (base)
#define SERIAL_FIFO_COMMAND_PORT(base) (base + 2)
/*
The Line Control register sets the general connection parameters.
Bit 7                    Bit 6            Bits 5-3    Bit 2     Bits 1-0
Divisor Latch Access Bit Break Enable Bit Parity Bits Stop Bits Data Bits
*/
#define SERIAL_LINE_COMMAND_PORT(base) (base + 3)   // Line Control Register
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base) (base + 5)

/* The I/O port commands */
/* SERIAL_LINE_ENABLE_DLAB:
* Tells the serial port to expect first the highest 8 bits on the data port,
* then the lowest 8 bits will follow
* If we send 0x80, which is 10000000 in bit to the Line Control Register, it
* enables DLAB
*/
#define SERIAL_LINE_ENABLE_DLAB 0x80

// Then we send 00000011 to the same port, this sets the Data Bits to 11
// which means 8-bit length, and 0 for the Stop bit (which means 1 stop bit), 
// and 0 for the Parity bits, which means NONE parity bits
#define SERIAL_LINE_CONFIG 0x03

/*
Bits 7-6	            Bits 5-4	Bit 3	        Bit 2	            Bit 1	            Bit 0
Interrupt Trigger Level	Reserved	DMA Mode Select	Clear Transmit FIFO	Clear Receive FIFO	Enable FIFO's
We use 0xC7 which is 11000111
This clear both FIFOs, enables FIFOs, and bit 5 = 0 means using 16-byte buffer
However OSDev says nothing about bit 5
*/
#define SERIAL_LINE_FIFO_CONFIG 0xC7

/*
Bit	Name	                    Meaning
0	Data Terminal Ready (DTR)	Controls the Data Terminal Ready Pin
1	Request to Send (RTS)	    Controls the Request to Send Pin
2	Out 1	                    Controls a hardware pin (OUT1) which is unused in PC implementations
3	Out 2	                    Controls a hardware pin (OUT2) which is used to enable the IRQ in PC implementations
4	Loop	                    Provides a local loopback feature for diagnostic testing of the UART
5	0	Unused
6	0	Unused
7	0	Unused
We use 0x03, which is 00000011. We only need to ready the Data Terminal,
enable the request to send. We don't need to receive anything so we don't
care about the other bits.
*/
#define SERIAL_LINE_MODEM_CONFIG 0x03

// All configuration functions, in proper order

void 
serial_configure_baud_rate(unsigned short com, unsigned short divisor);

void 
serial_configure_line(unsigned short com);

void 
serial_configure_fifo(unsigned short com);

void 
serial_configure_modem(unsigned short com);

int 
is_serial_buffer_empty(unsigned short com);

void 
write_serial(char c);