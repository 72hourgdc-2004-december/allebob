#ifndef _PIXELMASK_H
#define _PIXELMASK_H

#include "globals.h"

unsigned int *pixelmask_create(BITMAP *bmp);
int pixelmask_collision(unsigned int *mask, int x, int y, int w, int h, unsigned int *mask_, int x_, int y_, int w_, int h_);

#endif
