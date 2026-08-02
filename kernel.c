// kernel.c
// https://github.com/retrojan/rtjn-kernel

// color attribute for text 
#define COLOR 0x0C
/*
0x0 Black
0x1 Blue
0x2 Green
0x3 Cyan
0x4 Red
0x5 Magenta
0x6 Brown
0x7 Light Gray
0x8 Dark Gray
0x9 Light Blue
0xA Light Green
0xB Light Cyan
0xC Light Red
0xD Light Magenta
0xE Yellow
0xF White
*/

// function prototypes
void outb(unsigned short port, unsigned char data);
unsigned char inb(unsigned short port);
void clear(void);
void print_str(char *word);
void set_cursor(int row, int col);
void upd_cursor(unsigned int off);

// video memory pointer (text mode, color)
volatile char *vmem = (volatile char *) 0xb8000;
unsigned int offset = 0;          // current position in video memory (bytes)
int row = 5;                      // cursor row (0-24)
int col = 0;                      // cursor column (0-79)

// main kernel entry point - called from boot.asm
void rkernel(){
    clear();               // clear screen with default color

    // ASCII text
    char *line1 = "          ,------.       ,--------.               ,--.";
    char *line2 = "          |  .--. ' ,---.'--.  .--',--.--. ,---.  `--' ,--,--.,--,--,";
    char *line3 = "          |  '--'.'| .-. :  |  |   |  .--'| .-. | ,--.' ,-.  ||      |";
    char *line4 = "          |  |\\  \\ \\   --.  |  |   |  |   ' '-' ' |  |\\ '-'  ||  ||  |";
    char *line5 = "          `--' '--' `----'  `--'   `--'    `---'.-'  / `--`--'`--''--'";
    char *line6 = "                                                '---'";
    
    // print lines at fixed positions
    set_cursor(5, 0);
    print_str(line1);
    set_cursor(6, 0);
    print_str(line2);
    set_cursor(7, 0);
    print_str(line3);
    set_cursor(8, 0);
    print_str(line4);
    set_cursor(9, 0);
    print_str(line5);
    set_cursor(10, 0);
    print_str(line6);
    
    // move cursor to top-left
    row = 0; col = 0;
    set_cursor(row, col);
    unsigned char last_key = 0;   // debounce: prevent repeated keypress
    
    // main loop - handle keyboard input
    while(1) {
        unsigned char scancode = inb(0x60);  // read keyboard scancode
        
        // if key is pressed (bit 7 = 0) and not repeated
        if ((scancode & 0x80) == 0 && scancode != last_key) {
            last_key = scancode;
            
            // arrow key handling
            if (scancode == 0x48 && row > 0) {      // up
                row--;
                set_cursor(row, col);
            }
            else if (scancode == 0x50 && row < 24) { // down
                row++;
                set_cursor(row, col);
            }
            else if (scancode == 0x4B && col > 0) {  // left
                col--;
                set_cursor(row, col);
            }
            else if (scancode == 0x4D && col < 79) { // right
                col++;
                set_cursor(row, col);
            }
        }
        
        // key released - reset debounce
        if (scancode & 0x80) { last_key = 0; }
        
        // small delay - prevent CPU overload
        for (int i = 0; i < 100; i++);
    }
}

// clear - fill all cells with spaces and set color
void clear(){
    unsigned int i = 0;
    while (i < (80 * 25 * 2)) {
        vmem[i] = ' ';            // character
        i++;
        vmem[i] = COLOR;          // color attribute
        i++;
    }
    offset = 0;
    upd_cursor(0);
}

// print string to screen at current cursor position
void print_str(char *word){
    unsigned int word_index = 0;
    unsigned int pos = offset;
    
    while (word[word_index] != 0 && pos < (80 * 25 * 2)) {
        vmem[pos] = word[word_index];   // character
        pos++;
        vmem[pos] = COLOR;              // color attribute
        pos++;
        word_index++;
    }
    
    offset = pos;
    upd_cursor(offset);
}

// set cursor position by row and column
void set_cursor(int r, int c){
    row = r;
    col = c;
    
    // clamp values to valid range
    if (row < 0) row = 0;
    if (row > 24) row = 24;
    if (col < 0) col = 0;
    if (col > 79) col = 79;
    
    offset = (row * 80 + col) * 2;    // byte offset (2 bytes per cell)
    upd_cursor(offset);
}

// update hardware cursor via VGA ports
void upd_cursor(unsigned int off) {
    unsigned int pos = off / 2;        // cursor position (character index)
    
    // send low byte of position
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(pos & 0xFF));
    
    // send high byte of position
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((pos >> 8) & 0xFF));
}

// write byte to I/O port
void outb(unsigned short port, unsigned char data) {
    __asm__ volatile ("outb %0, %1" : : "a"(data), "Nd"(port));
}

// read byte from I/O port
unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
