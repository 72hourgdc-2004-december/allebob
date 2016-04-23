#include "game.h"

void player_tick(PLANE *self)
{
	PLANE_TYPE *t = &plane_types[self->type];
	int rotate = 0;

	if (self->destroyed)
	{
		self->vx = 0;
		self->vy = 0;
		/* TODO: create smoke */
		return;
	}

	if (key[KEY_LEFT])
		rotate = -1;
	if (key[KEY_RIGHT])
		rotate = 1;

	if (key[KEY_DOWN])
	{
		self->thrust -= 0.1;
		if (self->thrust < 0)
			self->thrust = 0;
	}
	else if (key[KEY_UP])
	{
		self->thrust += 0.1;
		if (self->thrust > 1)
			self->thrust = 1;
	}

	if (self->shoot_time)
		self->shoot_time--;

	if (key[KEY_RCONTROL] || key[KEY_LCONTROL])
	{
		if (self->shoot_time == 0)
		{
			bullet_create(self->x + self->hx * (t->radius + 20),
				self->y + self->hy * (t->radius + 20),
				self->hx, self->hy, self->vx, self->vy, 0, weapons[0]);
			self->shoot_time = damage[weapons[0] - 2] * 10;
		}
	}
	if (key[KEY_ENTER] || key[KEY_LSHIFT] || key[KEY_RSHIFT] || key[KEY_CAPSLOCK])
	{
		if (self->shoot_time == 0)
		{
			bullet_create(self->x + self->hx * (t->radius + 20),
				self->y + self->hy * (t->radius + 20),
				self->hx, self->hy, self->vx, self->vy, 0, weapons[1]);
			self->shoot_time = damage[weapons[1] - 2] * 10;
		}
	}

	plane_control(self, rotate);

	OBJECT *o = game_building_collision(&game, self);
	if (o)
	{
		float dx = self->x - o->x;
		float dy = self->y - o->y;
		float d = sqrt(dx * dx + dy * dy);
		self->vx = dx / d - self->vx;
		self->vy = dy / d - self->vy;
		self->hull -= 2;
		sound(HIT_WAV, self->x, self->y, 255, 700);
	}
}
