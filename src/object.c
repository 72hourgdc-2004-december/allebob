#include "object.h"
#include "game.h"

SPRITE *object_sprite(BITMAP *bmp, int x, int y)
{
	SPRITE *s = calloc(1, sizeof *s);
	s->x = x;
	s->y = y;
	s->bitmap = bmp;

	sprite_prerotate(s, 1);
	return s;
}

OBJECT *object_new(void)
{
	OBJECT *self = calloc(1, sizeof *self);
	return self;
}

void object_del(OBJECT *self)
{
	free(self);
}

void object_place(SPRITE *s, float x, float y)
{
	OBJECT *o = object_new();
	o->x = x;
	o->y = y;
	o->sprite = s;
	game_add_object(&game, o);
}

void building_place(SPRITE *s, float x, float y)
{
	OBJECT *o = object_new();
	o->x = x;
	o->y = y;
	o->sprite = s;
	game_add_building(&game, o);
}

void object_draw(OBJECT *self)
{
	int x = SCREEN_W / 2 + self->x - game.scroll_x;
	int y = SCREEN_H / 2 - self->y + game.scroll_y;

	if (x > WRAP_AROUND / 2)
		x -= WRAP_AROUND;
	if (x <= -WRAP_AROUND / 2)
		x += WRAP_AROUND;

	sprite_draw(self->sprite, x, y, 0);
}
