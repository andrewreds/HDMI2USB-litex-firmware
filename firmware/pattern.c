#include <stdlib.h>


#include <generated/csr.h>
#include <generated/mem.h>
#include <hw/flags.h>
#include <string.h>
#include <system.h>
#include <time.h>

#include "pattern.h"
#include "processor.h"
#include "stdio_wrap.h"
#include "uptime.h"
#include "version_data.h"

// #define DEBUG_PATTERN_TIME 1

static int pattern_param = 0;

unsigned int pattern_framebuffer_base(void) {
	return FRAMEBUFFER_BASE_PATTERN;
}

#define NUM_BARS 8

static const unsigned int color_bar[NUM_BARS] = {
	YCBCR422_WHITE,
	YCBCR422_YELLOW,
	YCBCR422_CYAN,
	YCBCR422_GREEN,
	YCBCR422_PURPLE,
	YCBCR422_RED,
	YCBCR422_BLUE,
	YCBCR422_BLACK
};

static const unsigned char font5x7[] = {
	0x00, 0x00, 0x00, 0x00, 0x00,// (space)
	0x00, 0x00, 0x5F, 0x00, 0x00,// !
	0x00, 0x07, 0x00, 0x07, 0x00,// "
	0x14, 0x7F, 0x14, 0x7F, 0x14,// #
	0x24, 0x2A, 0x7F, 0x2A, 0x12,// $
	0x23, 0x13, 0x08, 0x64, 0x62,// %
	0x36, 0x49, 0x55, 0x22, 0x50,// &
	0x00, 0x05, 0x03, 0x00, 0x00,// '
	0x00, 0x1C, 0x22, 0x41, 0x00,// (
	0x00, 0x41, 0x22, 0x1C, 0x00,// )
	0x08, 0x2A, 0x1C, 0x2A, 0x08,// *
	0x08, 0x08, 0x3E, 0x08, 0x08,// +
	0x00, 0x50, 0x30, 0x00, 0x00,// ,
	0x08, 0x08, 0x08, 0x08, 0x08,// -
	0x00, 0x60, 0x60, 0x00, 0x00,// .
	0x20, 0x10, 0x08, 0x04, 0x02,// /
	0x3E, 0x51, 0x49, 0x45, 0x3E,// 0
	0x00, 0x42, 0x7F, 0x40, 0x00,// 1
	0x42, 0x61, 0x51, 0x49, 0x46,// 2
	0x21, 0x41, 0x45, 0x4B, 0x31,// 3
	0x18, 0x14, 0x12, 0x7F, 0x10,// 4
	0x27, 0x45, 0x45, 0x45, 0x39,// 5
	0x3C, 0x4A, 0x49, 0x49, 0x30,// 6
	0x01, 0x71, 0x09, 0x05, 0x03,// 7
	0x36, 0x49, 0x49, 0x49, 0x36,// 8
	0x06, 0x49, 0x49, 0x29, 0x1E,// 9
	0x00, 0x36, 0x36, 0x00, 0x00,// :
	0x00, 0x56, 0x36, 0x00, 0x00,// ;
	0x00, 0x08, 0x14, 0x22, 0x41,// <
	0x14, 0x14, 0x14, 0x14, 0x14,// =
	0x41, 0x22, 0x14, 0x08, 0x00,// >
	0x02, 0x01, 0x51, 0x09, 0x06,// ?
	0x32, 0x49, 0x79, 0x41, 0x3E,// @
	0x7E, 0x11, 0x11, 0x11, 0x7E,// A
	0x7F, 0x49, 0x49, 0x49, 0x36,// B
	0x3E, 0x41, 0x41, 0x41, 0x22,// C
	0x7F, 0x41, 0x41, 0x22, 0x1C,// D
	0x7F, 0x49, 0x49, 0x49, 0x41,// E
	0x7F, 0x09, 0x09, 0x01, 0x01,// F
	0x3E, 0x41, 0x41, 0x51, 0x32,// G
	0x7F, 0x08, 0x08, 0x08, 0x7F,// H
	0x00, 0x41, 0x7F, 0x41, 0x00,// I
	0x20, 0x40, 0x41, 0x3F, 0x01,// J
	0x7F, 0x08, 0x14, 0x22, 0x41,// K
	0x7F, 0x40, 0x40, 0x40, 0x40,// L
	0x7F, 0x02, 0x04, 0x02, 0x7F,// M
	0x7F, 0x04, 0x08, 0x10, 0x7F,// N
	0x3E, 0x41, 0x41, 0x41, 0x3E,// O
	0x7F, 0x09, 0x09, 0x09, 0x06,// P
	0x3E, 0x41, 0x51, 0x21, 0x5E,// Q
	0x7F, 0x09, 0x19, 0x29, 0x46,// R
	0x46, 0x49, 0x49, 0x49, 0x31,// S
	0x01, 0x01, 0x7F, 0x01, 0x01,// T
	0x3F, 0x40, 0x40, 0x40, 0x3F,// U
	0x1F, 0x20, 0x40, 0x20, 0x1F,// V
	0x7F, 0x20, 0x18, 0x20, 0x7F,// W
	0x63, 0x14, 0x08, 0x14, 0x63,// X
	0x03, 0x04, 0x78, 0x04, 0x03,// Y
	0x61, 0x51, 0x49, 0x45, 0x43,// Z
	0x00, 0x00, 0x7F, 0x41, 0x41,// [
	0x02, 0x04, 0x08, 0x10, 0x20,// "\"
	0x41, 0x41, 0x7F, 0x00, 0x00,// ]
	0x04, 0x02, 0x01, 0x02, 0x04,// ^
	0x40, 0x40, 0x40, 0x40, 0x40,// _
	0x00, 0x01, 0x02, 0x04, 0x00,// `
	0x20, 0x54, 0x54, 0x54, 0x78,// a
	0x7F, 0x48, 0x44, 0x44, 0x38,// b
	0x38, 0x44, 0x44, 0x44, 0x20,// c
	0x38, 0x44, 0x44, 0x48, 0x7F,// d
	0x38, 0x54, 0x54, 0x54, 0x18,// e
	0x08, 0x7E, 0x09, 0x01, 0x02,// f
	0x08, 0x14, 0x54, 0x54, 0x3C,// g
	0x7F, 0x08, 0x04, 0x04, 0x78,// h
	0x00, 0x44, 0x7D, 0x40, 0x00,// i
	0x20, 0x40, 0x44, 0x3D, 0x00,// j
	0x00, 0x7F, 0x10, 0x28, 0x44,// k
	0x00, 0x41, 0x7F, 0x40, 0x00,// l
	0x7C, 0x04, 0x18, 0x04, 0x78,// m
	0x7C, 0x08, 0x04, 0x04, 0x78,// n
	0x38, 0x44, 0x44, 0x44, 0x38,// o
	0x7C, 0x14, 0x14, 0x14, 0x08,// p
	0x08, 0x14, 0x14, 0x18, 0x7C,// q
	0x7C, 0x08, 0x04, 0x04, 0x08,// r
	0x48, 0x54, 0x54, 0x54, 0x20,// s
	0x04, 0x3F, 0x44, 0x40, 0x20,// t
	0x3C, 0x40, 0x40, 0x20, 0x7C,// u
	0x1C, 0x20, 0x40, 0x20, 0x1C,// v
	0x3C, 0x40, 0x30, 0x40, 0x3C,// w
	0x44, 0x28, 0x10, 0x28, 0x44,// x
	0x0C, 0x50, 0x50, 0x50, 0x3C,// y
	0x44, 0x64, 0x54, 0x4C, 0x44,// z
	0x00, 0x08, 0x36, 0x41, 0x00,// {
	0x00, 0x00, 0x7F, 0x00, 0x00,// |
	0x00, 0x41, 0x36, 0x08, 0x00,// }
	0x08, 0x08, 0x2A, 0x1C, 0x08,// ->
	0x08, 0x1C, 0x2A, 0x08, 0x08 // <-
};


static void pattern_draw_text_color(int x, int y, char *ptr, long background_color, long text_color) {
#ifdef MAIN_RAM_BASE
	int i, j, k;
	int adr;
	volatile unsigned int *framebuffer = (unsigned int *)(MAIN_RAM_BASE + pattern_framebuffer_base());
	for(i=0; ptr[i] != '\0'; i++) {
		for(j=-1; j<8; j++) {
			for(k=-1; k<6; k++) {
				adr = 6*(x + i) + k + (2*9*y + 2*j)*processor_h_active/2;
				if (k >= 0 && k < 5 && j >= 0 && j < 7 && ((font5x7[5*(ptr[i] - ' ') + k] >> j) & 0x1)) {
					framebuffer[adr + 0*processor_h_active/2] = text_color;
					framebuffer[adr + 1*processor_h_active/2] = text_color;
				} else {
					framebuffer[adr + 0*processor_h_active/2] = background_color;
					framebuffer[adr + 1*processor_h_active/2] = background_color;
				}
			}
		}
	}
/* FIXME: Framebuffer Should not even be compiled if no MAIN RAM */
#endif
}

static void pattern_draw_text(int x, int y, char *ptr) {
	pattern_draw_text_color(x, y, ptr, YCBCR422_WHITE, YCBCR422_BLACK);
}

static void pattern_render_bars(int h_active, int v_active, int param) {
	(void) param;

	int i, x, y;
	int color, color_index;

	// how many ints does each bar need (rounding up)
	int bar_width = (h_active + NUM_BARS - 1) / NUM_BARS / 2;
	int next_bar;

	volatile unsigned int *framebuffer = (unsigned int *)(MAIN_RAM_BASE + pattern_framebuffer_base());

	for (y=0,i=0; y < v_active; y++) {
		next_bar = bar_width;
		color_index = 0;
		color = color_bar[color_index];

		for (x=0; x < h_active/2; x++,i++) {
			if (x >= next_bar) {
				next_bar = next_bar + bar_width;
				color_index ++;
				color = color_bar[color_index];
			}

			framebuffer[i] = color;
		}
	}
}

static void pattern_render_v_stripes(int h_active, int v_active, int param) {
	(void) param;

	int i;

	volatile unsigned int *framebuffer = (unsigned int *)(MAIN_RAM_BASE + pattern_framebuffer_base());

	for(i=0; i<h_active*v_active/2; i++) {
		framebuffer[i] = 0x801080ff;
	}
}

struct pattern_metadata pattern_all_metadata[] = {
	{"bars", "Vertical color bars", pattern_render_bars, NULL},
	{"v_stripes", "Vertical color stripes of <param> width", pattern_render_v_stripes, NULL},

	/* All null entry to signify end of array */
	{NULL, NULL, NULL, NULL}
};

void pattern_fill_framebuffer(int h_active, int v_active)
{
#ifdef MAIN_RAM_BASE
	int i, j;
	flush_l2_cache();
	volatile unsigned int *framebuffer = (unsigned int *)(MAIN_RAM_BASE + pattern_framebuffer_base());

	struct pattern_metadata* pattern_current_metadata = &pattern_all_metadata[pattern_current];

#ifdef DEBUG_PATTERN_TIME
	int t1, t2;
	static char buffer[32];

	timer0_update_value_write(1);
	t1 = timer0_reload_read() - timer0_value_read();
#endif

	if (pattern_current_metadata->render) {
		pattern_current_metadata->render(h_active, v_active, pattern_param);
	}

#ifdef DEBUG_PATTERN_TIME
	timer0_update_value_write(1);
	t2 = timer0_reload_read() - timer0_value_read();
#endif

	// draw a border around that.
	for (i=0; i<h_active*2; i++) {
		framebuffer[i] = YCBCR422_WHITE;
	}
	
	for (i=(v_active-4)*h_active*2/4; i<h_active*v_active*2/4; i++) {
		framebuffer[i] = YCBCR422_WHITE;
	}
	
	for (i=0; i<v_active*2; i++) {
		// do the left bar
		for (j=0; j<2; j++) {
			framebuffer[(i*h_active)+j] = YCBCR422_WHITE;
			framebuffer[(i*h_active)+j + (1*h_active/2)] = YCBCR422_WHITE;
		}
		
		// do the right bar
		for (j=h_active-2; j<h_active; j++) {
			framebuffer[(i*h_active)+j] = YCBCR422_YELLOW;
			framebuffer[(i*h_active)+j + (1*h_active/2)] = YCBCR422_WHITE;
		}		
	}

	// Line 1 - uptime + version information
	int line = 1;
	pattern_draw_text(19, line, (char*)git_describe);
	line++;
	// Line 2 - build time data
	pattern_draw_text(1, line, "Built: "__DATE__" "__TIME__);
	line++;
	// Line 3- running on
	pattern_draw_text(1, line, "Running on ");
	pattern_draw_text(12, line, (char*)board);
	line++;

	// Line 4 - empty
	line++;

#ifndef HIDE_ADVERT
	// Line 5 - Hi!
	pattern_draw_text(1, line, "Hi! I am HDMI2USB ");
	line++;
	// Line 6+7 - Want...
	pattern_draw_text_color(1, line, "Want to hack on FOSS video capture systems?", YCBCR422_BLUE, YCBCR422_WHITE);
	line++;
	pattern_draw_text_color(1, line, "Get in touch with us! #timvideos on Freenode IRC", YCBCR422_RED, YCBCR422_WHITE);
	line++;
	// Line 8 - URLs..
	pattern_draw_text(1, line, "code.timvideos.us / enjoy-digital.fr");
	pattern_draw_text_color(6, line, "tim", YCBCR422_WHITE, YCBCR422_RED);
	pattern_draw_text_color(9, line, "videos", YCBCR422_WHITE, YCBCR422_BLUE);
	pattern_draw_text_color(27, line, "digital", YCBCR422_WHITE, YCBCR422_CYAN);
#endif

#ifdef DEBUG_PATTERN_TIME
	line++;
	line++;
	// Line 10 - render time
	sprintf(buffer, "Render time: %d", t2-t1);
	pattern_draw_text(1, line, buffer);
#endif

	flush_l2_cache();
/* FIXME: Framebuffer Should not even be compiled if no MAIN RAM */
#endif
}

int pattern_load(const char* name, const char* param_str) {
	int i;
	int new_pattern_index = 0;

	if (name[0] == '\0') {
		// we want the next pattern

		new_pattern_index = pattern_current + 1;

		if (pattern_all_metadata[new_pattern_index].name == NULL) {
			// we have reached the end of the array. Wrap back around

			new_pattern_index = 0;
		}
	} else if (name[0] >= '0' && name[0] <= '9') {
		// we have an index into the array, rather then a name
		new_pattern_index = atoi(name);

		for (i=0; i<=new_pattern_index; i++) {
			if (pattern_all_metadata[i].name == NULL) {
				// the index is out of bounds

				return 1;
			}
		}
	} else {
		// do a text match of the name

		for (i=0;; i++) {
			if (pattern_all_metadata[i].name == NULL) {
				// the index is out of bounds

				return 1;
			}

			if (strcmp(name, pattern_all_metadata[i].name) == 0) {
				// we have a match

				new_pattern_index = i;
				break;
			}
		}
	}

	pattern_current = new_pattern_index;
	pattern_param = atoi(param_str);

	if (pattern_param < 0) {
		pattern_param = 0;
	}

	if (pattern_param > 255) {
		pattern_param = 255;
	}

	pattern_fill_framebuffer(processor_h_active, processor_v_active);

	return 0;
}

void pattern_service(void)
{
#ifdef MAIN_RAM_BASE
	static int last_event;
	static char buffer[16];

	if(elapsed(&last_event, SYSTEM_CLOCK_FREQUENCY)) {
		sprintf(buffer, "Uptime: %s", uptime_str());
		pattern_draw_text(1, 1, buffer);
	}
	flush_l2_cache();
/* FIXME: Framebuffer Should not even be compiled if no MAIN RAM */
#endif
}
