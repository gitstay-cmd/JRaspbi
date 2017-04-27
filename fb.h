#ifndef fb_h_INCLUDED
#define fb_h_INCLUDED
#include <stdint.h>
#include "rpi-mailbox-interface.h"

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480
#define SCREEN_DEPTH	16

#define COLOUR_DELTA	0.05

typedef struct{
    uint16_t r;
    uint16_t g;
    uint16_t b;
    uint16_t a;
}color_t;

extern unsigned char FONT[];

rpi_mailbox_property_t* mp;

#define RGB(r, g, b) ((((r) >> 3)<<11) | (((g)>>2)<<5) | ((b)>>3))

#define COLOR_BLACK RGB(0, 0, 0)

#define COLOR_GREEN RGB(0, 255, 0)

#define COLOR_CYAN RGB(0, 255, 255)

#define COLOR_RED RGB(255, 0, 0)

#define COLOR_YELLOW RGB(255, 255, 0)

#define COLOR_WHITE RGB(255, 255, 255)

#define CONSOLE_WHITE COLOR_WHITE
#define CONSOLE_BLACK COLOR_BLACK
#define CONSOLE_GREEN COLOR_GREEN
#define CONSOLE_CYAN COLOR_CYAN
#define CONSOLE_RED COLOR_RED
#define CONSOLE_YELLOW COLOR_YELLOW

typedef struct{	
    uint32_t width;
    uint32_t height;
    uint32_t vwidth;
    uint32_t vheight;
    uint32_t pitch;
    uint32_t depth;
    uint16_t fore;
    uint16_t back;
    uint32_t x;
    uint32_t y;
    uint32_t fb;
    uint32_t size;
}fb_t;

fb_t fb;

void fb_init();

void console_drawc(int c);

void console_puts(const char *s);

void clear();

void newline();

void draw_char(const int c, const int x, int y, const uint16_t fore, uint16_t back);

void draw_pixel(const int x, const int y, const uint16_t color);
#endif // fb_h_INCLUDED

