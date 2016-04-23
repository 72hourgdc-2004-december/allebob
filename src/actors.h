#ifndef _ACTORS_H
#define _ACTORS_H

#include "globals.h"
#include "plane.h"
#include "bullet.h"

extern void player_tick(PLANE *self);
extern void AI_tick(PLANE *self);
extern void cloud_tick(PLANE *self);
extern void tree_tick(PLANE *self);
extern void mountain_tick(PLANE *self);

#endif
