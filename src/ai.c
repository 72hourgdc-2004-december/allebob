#include "game.h"

static inline float sign(float x)
{
	if (x < 0)
		return -1;
	if (x > 0)
		return 1;
	return 0;
}

void AI_tick(PLANE *self)
{
	PLANE_TYPE *t = &plane_types[self->type];
	PLANE *enemy = game.player1;
	int rotate = 0;

	if (self->destroyed)
	{
		if (self->hull < 0)
			game.ai_down++;
		self->hull = 1;
		return;
	}

	/* Direction we are facing currently. */
	float angle = atan2(self->hy, self->hx); /* -pi..pi */

	float dx, dy;
	if (self->ai_dumb_time == 0)
	{
		self->ai_dumb_x = enemy->x;
		self->ai_dumb_y = enemy->y;
		if (self->ai_dumb && self->ai_dumb_y < 100)
			self->ai_dumb_y = 100;
		self->ai_dumb_time = self->ai_dumb * 30;
	}
	else
		self->ai_dumb_time--;

	if (enemy->destroyed)
	{
		self->ai_dumb_y = 500;
	}

	plane_distance (self, self->ai_dumb_x, self->ai_dumb_y, &dx, &dy);
	float d = sqrt(dx * dx + dy * dy);

	float enemy_angle;

	/* Don't rotate if too close. */
	if (d < 50)
		enemy_angle = angle;
	else
	{
		/* Direction towards enemy. */
		enemy_angle = atan2(dy, dx); /* -pi..pi */
	}

	/* Rotate towards enemy. */
	float rel = enemy_angle - angle; /* -2*pi..2*pi */
	if (rel < -AL_PI)
		rel += 2 * AL_PI;
	else if (rel > AL_PI)
		rel -= 2 * AL_PI;

	if (!self->ai_wait)
	{
		if (rel < 0)
			rotate = 1;
		else
			rotate = -1;
		/* When far away, don't rotate so much. */
		self->ai_wait = 10 - d / 100;
		if (self->ai_wait < 0)
			self->ai_wait = 0;
		self->ai_rot = rotate;
	}
	else
	{
		rotate = self->ai_rot;
		self->ai_wait--;
	}

	if (self->shoot_time == 0)
	{
		/* Inside the range -0.2..0.2, should, but randomly not at
		 * once (else never aims directly). With a too small range,
		 * always aims directly, but is easy to avoid, and too
		 * boring.
		 */
		if ((rel > -0.2 && rel < 0.2) & !(rand() % 10))
		{
			bullet_create(self->x + self->hx * (t->radius + 20),
				self->y + self->hy * (t->radius + 20),
				self->hx, self->hy, self->vx, self->vy, 1, 2);
			self->shoot_time = 10;
		}
	}
	else
		self->shoot_time--;

	/* Slow down when near enemy. */
	if (d > 100)
		self->thrust = 1;
	else
		self->thrust = (d / 200);

	/* Let go of gas pedal before hitting ground */
	if (self->y + self->vy * 30 < 0)
	{
		self->thrust = 0;
		if (self->hx > 0)
			rotate = -1;
		if (self->hx < 0)
			rotate = 1;
	}
	
	/* Fly tight forward loops */
	if (self->hy < -0.7)
		self->thrust = 0;
	if (self->hy < 0 && sign(enemy->hx) != sign(self->hx))
		self->thrust = 0;

	/* Don't run into others. */
	int i;
	for (i = 0; game.AI[i]; i++)
	{
		PLANE *p = game.AI[i];
		if (self != p)
		{
			float px = p->x - self->x - self->vx * 20;
			float py = p->y - self->y - self->vy * 20;
			float p = sqrt (px * px + py * py);
			if (p < 100)
			{
				self->thrust = p / 200.0;
			}
		}
	}

	plane_control(self, rotate);
}
