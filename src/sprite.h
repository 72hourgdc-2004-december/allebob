#ifndef _SPRITE_H
#define _SPRITE_H

#include "globals.h"

typedef struct SPRITE SPRITE;
typedef struct SPRITE_FRAME SPRITE_FRAME;

struct SPRITE_FRAME
{
	BITMAP *bitmap;
	unsigned int *mask;
	int x, y;
};

struct SPRITE
{
	int x, y; /* Offset to center. */
	BITMAP *bitmap;

	int n; /* Number of prerotations. */
	SPRITE_FRAME *frames;
};

void sprite_draw(SPRITE *self, int x, int y, float angle);
void sprite_colorize(SPRITE *self, BITMAP *orig, BITMAP *colormask, int color);
void sprite_prerotate(SPRITE *self, int n);
int sprite_collides(SPRITE *self, int x, int y, float angle, SPRITE *other, int x_, int y_, float angle_);

#endif
