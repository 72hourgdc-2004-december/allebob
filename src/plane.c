// vim: ts=8 noet sw=8 sts=8
#include "plane.h"
#include "game.h"

/* How fast can the plane rotate up/down. */
#define ROTATE_SPEED 0.3
/* How much 'spin' can it get. */
#define ROTATIONAL_FRICTION 0.80
/* How much will the plane auto-rotate due to air friction. */
#define AIR_ROTATE 0.03

/* How much air friction (1.0 = none). */
#define FRICTION 0.960
/* How strong is the acceleration downwards? */
#define GRAVITY 0.33
/* How much engine acceleration? */
#define THRUST 0.38
/* How good does it 'glide'. */
#define GLIDE 0.85

/* Height below which engines have full power. */
#define AIR_HIGH 3000 
/* Height where engines have no more power. */
#define AIR_MAX 5000

/* How much velocity remains when colliding with the ground. */
#define GROUND_BOUNCE 0.99
/* Force created by collision with other plane. */
#define COLLISION_FORCE 1.5

PLANE_TYPE plane_types[PLANE_TYPES_MAX];

void plane_control(PLANE *self, int rotate)
{
	PLANE_TYPE *t = &plane_types[self->type];
	MATRIX_f m;
	float zero = 0;

	if (self->hull < 0)
	{
		self->thrust = 0;
		if (self->rotating == 0)
			rotate = (rand() % 2) * 4 - 2;
		else if (self->rotating > 0)
			rotate = 2;
		else
			rotate = -2;
		self->y -= 2;
	}

	if (rotate)
	{
		self->rotating += ROTATE_SPEED * rotate;
	}

	float angle = atan2(self->hy, self->hx); /* -pi..pi */

	if (angle > -AL_PI / 2 + 0.2 && angle < AL_PI / 2 - 0.2)
		self->flipped = 0;

	if (angle < -AL_PI / 2 - 0.2 || angle > AL_PI / 2 + 0.2)
		self->flipped = 1;

	/* Really cheap "physics" */

	get_vector_rotation_matrix_f(&m, 0, 0, 1, self->rotating);
	apply_matrix_f(&m, self->hx, self->hy, zero,
		&self->hx, &self->hy, &zero);

	self->rotating *= ROTATIONAL_FRICTION;

	float thrust = self->thrust * THRUST;
	if (self->y > AIR_HIGH)
	{
		float f = 1 - (self->y - AIR_HIGH) / (AIR_MAX - AIR_HIGH);
		thrust *= f;
	}

	self->vx += self->hx * thrust;
	self->vy += self->hy * thrust;

	/* gravity */
	self->vy -= GRAVITY;

	/* air resistance causing rotation */
	float air = self->vx * self->hy - self->vy * self->hx;
	get_vector_rotation_matrix_f(&m, 0, 0, 1, AIR_ROTATE * air);
	apply_matrix_f(&m, self->hx, self->hy, zero,
		&self->hx, &self->hy, &zero);

	self->vx *= FRICTION;
	self->vy *= FRICTION;

	float x_diff = self->x;
	float y_diff = self->y;

	self->x += self->vx;
	self->y += self->vy;

	/* Collision with ground. */
	if (self->y < t->radius)
	{
		if (self->vy * self->vy + self->vx * self->vx > 3)
		{
			self->y = t->radius - (self->y - t->radius);
			self->vy = -self->vy * GROUND_BOUNCE;
			self->hy = -self->hy;
			self->hull -= 1;
			if (self->hull < 0) {
				explode(self->x, self->y);
				self->destroyed = 1;
			}
			else
				sound(HIT_WAV, self->x, self->y, 255, 1000);
		}
		else
		{
			self->y = t->radius;
			self->vy = -self->vy * GROUND_BOUNCE;
		}
	}

	/* Collision with other plane. */
	PLANE *c = game_collision(&game, self);
	if (c)
	{
		float dx = self->x - c->x;
		float dy = self->y - c->y;
		float d = sqrt(dx * dx + dy * dy);
		self->vx += COLLISION_FORCE * dx / d;
		self->vy += COLLISION_FORCE * dy / d;
		c->vx -= COLLISION_FORCE * dx / d;
		c->vy -= COLLISION_FORCE * dy / d;
		self->hull -= 2;
		c->hull--;
		sound(HIT_WAV, self->x, self->y, 255, 1000);
	}

	x_diff = self->x - x_diff;
	y_diff = self->y - y_diff;

	/* air resistance/ gliding */
	float glide = self->hx * y_diff - self->hy * x_diff;
	self->vx += self->hy * glide * GLIDE;
	self->vy -= self->hx * glide * GLIDE;

	if (self->x >= WRAP_AROUND)
		self->x -= WRAP_AROUND;
	if (self->x < 0)
		self->x += WRAP_AROUND;
}

void plane_distance(PLANE *self, float x, float y, float *dx, float *dy)
{
	*dx = x - self->x;
	*dy = y - self->y;
	if (*dx > WRAP_AROUND / 2)
		*dx -= WRAP_AROUND;
	if (*dx < -WRAP_AROUND / 2)
		*dx += WRAP_AROUND;
}

float plane_get_angle(PLANE *self)
{
	return atan2(self->hy, self->hx);
}

SPRITE *plane_get_sprite(PLANE *self)
{
	PLANE_TYPE *t = &plane_types[self->type];
	if (self->flipped)
		return &t->left;
	else
		return &t->right;
}

void plane_draw(PLANE *self)
{
	int x = SCREEN_W / 2 + self->x - game.scroll_x;
	int y = SCREEN_H / 2 - self->y + game.scroll_y;

	if (x > WRAP_AROUND / 2)
		x -= WRAP_AROUND;
	if (x <= -WRAP_AROUND / 2)
		x += WRAP_AROUND;

	sprite_draw(plane_get_sprite(self), x, y, plane_get_angle(self));

#ifdef SHOW_GRID
	PLANE_TYPE *t = &plane_types[self->type];

	int white = makecol(255, 255, 255);
	int blue = makecol(0, 0, 255);
        int red = makecol(255, 0, 0);
	circle(buffer, x, y, t->radius, t->color);
	line(buffer, x, y, x + self->hx * t->radius,
		 y - self->hy * t->radius, white);
        line(buffer, x, y, x + self->vx * t->radius,
		y - self->vy * t->radius, red);

	/* up vector */
	if (self->flipped)
		line(buffer, x, y, x + self->hy * t->radius,
			y + self->hx * t->radius, blue);
	else
		line(buffer, x, y, x - self->hy * t->radius,
			y - self->hx * t->radius, blue);
#endif
}

int plane_collides(PLANE *self, PLANE *other)
{
	return sprite_collides(
		plane_get_sprite(self), self->x, -self->y, plane_get_angle(self),
		plane_get_sprite(other), other->x, -other->y, plane_get_angle(other));
}

PLANE *plane_new(void)
{
	PLANE *self = calloc(1, sizeof *self);
	return self;
}

void plane_del(PLANE *self)
{
	free (self);
}

void plane_type_colorize(PLANE_TYPE *self, int dat, int dat_mask, int color)
{
	self->color = color;

	if (dat_mask)
	{
		sprite_colorize(&self->right,
			(BITMAP*)data[dat].dat,
			(BITMAP*)data[dat_mask].dat,
			color);
	}
	else
		self->right.bitmap = (BITMAP*)data[dat].dat;

	self->left.bitmap = create_bitmap(self->right.bitmap->w,
		self->right.bitmap->h);

	clear_to_color(self->left.bitmap, bitmap_mask_color(self->left.bitmap));
	draw_sprite_v_flip(self->left.bitmap, self->right.bitmap, 0, 0);
}

void plane_type_prerotate(PLANE_TYPE *self, int n)
{
	int mid_x = self->right.bitmap->w / 2;
	int mid_y = self->right.bitmap->h / 2;
	self->left.x = mid_x;
	self->left.y = self->left.bitmap->h - 1 - mid_y;
	self->right.x = mid_x;
	self->right.y = mid_y;
	sprite_prerotate(&self->left, n);
	sprite_prerotate(&self->right, n);
}
