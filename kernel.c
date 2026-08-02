#define COLOR 0x0C

void outb(unsigned short port, unsigned char data);
unsigned char inb(unsigned short port);
void clear_screen(void);
void print_string(char *word);
void set_cursor(int row, int col);
void update_cursor(unsigned int off);

volatile char *vmem = (volatile char *) 0xb8000;
unsigned int offset = 0;
int row = 5;
int col = 0;

void rkernel(){
    clear_screen();
    
    char *line1 = "          ,------.       ,--------.               ,--.";
    char *line2 = "          |  .--. ' ,---.'--.  .--',--.--. ,---.  `--' ,--,--.,--,--,";
    char *line3 = "          |  '--'.'| .-. :  |  |   |  .--'| .-. | ,--.' ,-.  ||      |";
    char *line4 = "          |  |\\  \\ \\   --.  |  |   |  |   ' '-' ' |  |\\ '-'  ||  ||  |";
    char *line5 = "          `--' '--' `----'  `--'   `--'    `---'.-'  / `--`--'`--''--'";
    char *line6 = "                                                '---'";
    
    set_cursor(5, 0);
    print_string(line1);
    
    set_cursor(6, 0);
    print_string(line2);
    
    set_cursor(7, 0);
    print_string(line3);
    
    set_cursor(8, 0);
    print_string(line4);
    
    set_cursor(9, 0);
    print_string(line5);
    
    set_cursor(10, 0);
    print_string(line6);
    
    row = 0; col = 0;
    set_cursor(row, col);
    unsigned char last_key = 0;
    
    while(1) {
        unsigned char scancode = inb(0x60);
        
        if ((scancode & 0x80) == 0 && scancode != last_key) {
            last_key = scancode;
            
            if (scancode == 0x48 && row > 0) {
                row--;
                set_cursor(row, col);
            }
            else if (scancode == 0x50 && row < 24) {
                row++;
                set_cursor(row, col);
            }
            else if (scancode == 0x4B && col > 0) {
                col--;
                set_cursor(row, col);
            }
            else if (scancode == 0x4D && col < 79) {
                col++;
                set_cursor(row, col);
            }
        }
        
        if (scancode & 0x80) { last_key = 0; } 
        for (int i = 0; i < 100; i++);
    }
}

void clear_screen(){
    unsigned int i = 0;
    while (i < (80 * 25 * 2)){
        vmem[i] = ' ';
        i++;
        vmem[i] = COLOR;
        i++;
    }
    offset = 0;
    update_cursor(0);
}

void print_string(char *word){
    unsigned int word_index = 0;
    unsigned int pos = offset;
    
    while (word[word_index] != 0 && pos < (80 * 25 * 2)) {
        vmem[pos] = word[word_index];
        pos++;
        vmem[pos] = COLOR;
        pos++;
        word_index++;
    }
    
    offset = pos;
    update_cursor(offset);
}

void set_cursor(int r, int c){
    row = r;
    col = c;
    
    if (row < 0) row = 0;
    if (row > 24) row = 24;
    if (col < 0) col = 0;
    if (col > 79) col = 79;
    
    offset = (row * 80 + col) * 2;
    update_cursor(offset);
}

void update_cursor(unsigned int off) {
    unsigned int pos = off / 2;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((pos >> 8) & 0xFF));
}

void outb(unsigned short port, unsigned char data) {
    __asm__ volatile ("outb %0, %1" : : "a"(data), "Nd"(port));
}

unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
