#include "game.h"

int weapons[2] = { 2, 3 };
int damage[4] = { 1, 3, 2, 4 };

void bullet_tick(PLANE *self)
{
	if (!self->bullet_time)
	{
		self->destroyed = 1;
		return;
	}
	self->bullet_time--;

	/* Smoke */
	if (self->hull)
	{
		self->type = 6;
		self->y--;
		return;
	}

	self->x += self->vx;
	self->y += self->vy;

	if (self->x >= WRAP_AROUND)
		self->x -= WRAP_AROUND;
	if (self->x < 0)
		self->x += WRAP_AROUND;

	/* AI bullets can't hit AI..
	 * TODO: maybe will allow friendly fire later.
	 */
	PLANE *c = NULL;
	if (self->bullet_ai)
	{
		if (plane_collides(self, game.player1))
			c = game.player1;
	}
	else
	{
		c = game_collision(&game, self);
	}
	if (c)
	{
		c->hull -= damage[self->type - 2];
		self->hull = 1; /* smoke */
		self->bullet_time = 30;
		sound(HIT_WAV, self->x, self->y, 255, 1000);
	}

	/* Player bullets can hit buildings. */
	if (!self->bullet_ai)
	{
		OBJECT *o = game_building_collision(&game, self);
		if (o && o->destroyed == 0)
		{
			sound(HIT_WAV, self->x, self->y, 255, 1000);
			o->damage += damage[self->type - 2];
			self->hull = 1; /* smoke */
			self->bullet_time = 30;
			if (o->damage > 20)
			{
				o->destroyed = 1;
				game.buildings_down++;
				o->sprite = building[1];
				explode(self->x, self->y);
			}
		}
	}

	if (self->y < 0)
		self->bullet_time = 0;
}

void bullet_create(float x, float y, float dx, float dy, float vx, float vy,
	int ai, int type)
{
	if (game.bullets_count >= MAX_BULLETS_COUNT)
		bullet_remove(rand() % MAX_BULLETS_COUNT);

	int i = game.bullets_count;
	float d = sqrt(dx * dx + dy * dy);
	memset(&game.bullets[i], 0, sizeof game.bullets[i]);
	game.bullets[i].hx = dx / d;
	game.bullets[i].hy = dy / d;
	game.bullets[i].vx = vx + dx * 8 / d;
	game.bullets[i].vy = vy + dy * 8 / d;
	game.bullets[i].x = x + vx;
	game.bullets[i].y = y + vy;
	game.bullets[i].type = type;
	game.bullets[i].bullet_time = 60;
	game.bullets[i].bullet_ai = ai;

	game.bullets_count++;
	if (type == 2)
		sound(SHOT_WAV, x, y, 180, 1000);
	else
		sound(RKT_WAV, x, y, 200, 1000 + (rand() & 255));
}

void bullet_remove(int i)
{
	int last = game.bullets_count - 1;
	game.bullets[i] = game.bullets[last];
	game.bullets_count--;
}
