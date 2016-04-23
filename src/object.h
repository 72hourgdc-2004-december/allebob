#ifndef _OBJECT_H
#define _OBJECT_H

#include "globals.h"
#include "sprite.h"

typedef struct OBJECT OBJECT;

struct OBJECT
{
	float x, y;
	SPRITE *sprite;
	int damage;
	int destroyed;
};

SPRITE *object_sprite(BITMAP *bmp, int x, int y);
void object_place(SPRITE *s, float x, float y);
void object_draw(OBJECT *self);
void building_place(SPRITE *s, float x, float y);
void object_del(OBJECT *self);

#endif
