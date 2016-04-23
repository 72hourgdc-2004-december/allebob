
#include "explosion.h"

BITMAP *bigexp[10];

int make_explosion(explosion *e, int x, int y, int numf, BITMAP **f)
{
	if (!f || !numf)
		return 0;

	e->cur_frame = 0;
	e->num_frames = numf;
	e->x = x;
	e->y = y;
	e->w = f[numf - 1]->w;
	e->h = f[numf - 1]->h;
	e->frames = f;

	return 1;
}

int explosion_tick(explosion *e)
{
	if (e->timer > 2) {
		if (++e->cur_frame >= e->num_frames)
			return 0;
		e->timer = 0;
	} else
		e->timer++;
	return 1;
}

void draw_explosion(explosion *e, int sx, int sy)
{
	draw_sprite(buffer, e->frames[e->cur_frame], (SCREEN_W / 2 + e->x - sx) - 50, (SCREEN_H / 2 - e->y + sy) - 50);
}
