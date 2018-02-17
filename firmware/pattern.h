#ifndef __PATTERN_H
#define __PATTERN_H

#include "framebuffer.h"

/* Colors in YCBCR422 format (see pattern.py) */
#define YCBCR422_WHITE  0x80ff80ff
#define YCBCR422_YELLOW 0x00e194e1
#define YCBCR422_CYAN   0xabb200b2
#define YCBCR422_GREEN  0x2b951595
#define YCBCR422_PURPLE 0xd469e969
#define YCBCR422_RED    0x544cff4c
#define YCBCR422_BLUE   0xff1d6f1d
#define YCBCR422_BLACK  0x80108010

/* Colors in RGB format */
#define RGB_WHITE  0x00ffffff
#define RGB_YELLOW 0x0000ffff
#define RGB_CYAN   0x00ffff00
#define RGB_GREEN  0x0000ff00
#define RGB_PURPLE 0x00ff00ff
#define RGB_RED    0x000000ff
#define RGB_BLUE   0x00ff0000
#define RGB_BLACK  0x00000000

unsigned int pattern_framebuffer_base(void);

/* metadata used to describe a pattern, and how to render it */
struct pattern_metadata {
	const char *name;
	const char *desc;
	void (*render)(int h_active, int w_active, int param);
	void (*service)(int h_active, int w_active, int param);
};

extern struct pattern_metadata pattern_all_metadata[];

int pattern_current;

void pattern_fill_framebuffer(int h_active, int m_active);
void pattern_service(void);
int pattern_load(const char* name, const char* param);

#endif /* __PATTERN_H */
