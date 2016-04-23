
#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "globals.h"
#include "sprite.h"

#define MAX_FRAMES	10

typedef struct {
	int x, y, w, h, num_frames, cur_frame, timer;
	BITMAP **frames;
} explosion;

int make_explosion(explosion *e, int x, int y, int numf, BITMAP **f);
int explosion_tick(explosion *e);
void draw_explosion(explosion *e, int sx, int sy);
void remove_explosion(int i);

extern BITMAP* bigexp[10];

#endif
