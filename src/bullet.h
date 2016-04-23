#ifndef _BULLET_H_
#define _BULLET_H_

void bullet_tick(PLANE *self);
void bullet_create(float x, float y, float dx, float dy, float vx, float vy, int ai, int type);
void bullet_remove(int i);

extern int weapons[2], damage[4];

#endif
