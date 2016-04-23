// vim: ts=8 noet sw=8 sts=8
#ifndef _PLANE_H
#define _PLANE_H

#include "globals.h"
#include "sprite.h"

typedef struct PLANE PLANE;
typedef struct PLANE_TYPE PLANE_TYPE;

#define PLANE_TYPES_MAX 10

extern PLANE_TYPE plane_types[PLANE_TYPES_MAX];


struct PLANE_TYPE
{
	float radius;
	int color;

	SPRITE left;
	SPRITE right;
};

struct PLANE
{
	int destroyed;

	float x, y; /* Position */
	float vx, vy; /* Velocity */
	float hx, hy; /* Heading */
	float rotating; /* Rotational velocity */

	float thrust;
	int flipped;

	int type;
	int customizations[8]; /* TODO: customizations */

	int shoot_time;
	int hull, bulletdmg;

	int ai_wait;
	int ai_rot;
	int ai_dumb;
	int ai_dumb_x;
	int ai_dumb_y;
	int ai_dumb_time;

	int bullet_ai;
	int bullet_time;
	int burner_time;
};

void plane_control(PLANE *self, int rotate);
void plane_draw(PLANE *self);
PLANE *plane_new(void);
void plane_del(PLANE *self);
void plane_type_colorize(PLANE_TYPE *self, int dat, int dat_mask, int color);
void plane_type_prerotate(PLANE_TYPE *self, int n);
int plane_collides(PLANE *self, PLANE *other);
void plane_distance(PLANE *self, float x, float y, float *dx, float *dy);
SPRITE *plane_get_sprite(PLANE *self);
float plane_get_angle(PLANE *self);

#endif
