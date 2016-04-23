#include "game.h"
#include "missions.h"

#define GROUND_WIDTH 20

GAME game;
SPRITE *building[2];
SPRITE *tree[2];

PALETTE blackpal;
PALETTE redpal;

void game_restart(GAME *self, int mission)
{
	if (self->player1)
		plane_del(self->player1);
	int i;
	for (i = 0; self->AI[i]; i++)
	{
		plane_del(game.AI[i]);
	}
	for (i = 0; self->objects[i]; i++)
		object_del(self->objects[i]);
	memset (self, 0, sizeof *self);

	self->mission = mission;

	for (i = 0; i < 25; i++)
	{
		int x = rand() % WRAP_AROUND;
		int t = rand() % 2;
		object_place(tree[t], x, 15);
	}
	for (i = 0; i < 25; i++)
	{
		int x = rand() % WRAP_AROUND;
		int t = rand() % 2;
		object_place(tree[t], x, 10);
	}
	for (i = 0; i < 25; i++)
	{
		int x = rand() % WRAP_AROUND;
		int t = rand() % 2;
		object_place(tree[t], x, 5);
	}

	missions[mission]();

	game.scroll_x = game.player1->x + game.player1->vx * 60 / 4;
	game.scroll_y = game.player1->y + game.player1->vy * 60 / 4;
}

void game_init(void)
{
	/* Set font colors. */
	int i;
	for (i = 0; i < 256; i++)
	{
		blackpal[i].r = i * 20 / 255;
		blackpal[i].g = i * 20 / 255;
		blackpal[i].b = i * 63 / 255;
	}
	for (i = 0; i < 256; i++)
	{
		redpal[i].r = 63;
		redpal[i].g = i * 20 / 255;
		redpal[i].b = i * 63 / 255;
	}
	

	memset (&game, 0, sizeof game);
	/* TODO: do all the initialization properly */
	plane_types[0].radius = 25;
	plane_type_colorize(&plane_types[0], JET, JET_COLORMASK, makecol(0, 255, 0));
	plane_type_prerotate(&plane_types[0], 64);

	plane_types[1].radius = 25;
	plane_type_colorize(&plane_types[1], JET, JET_COLORMASK, makecol(255, 50, 50));
	plane_type_prerotate(&plane_types[1], 64);

	plane_types[2].radius = 3;
	plane_type_colorize(&plane_types[2], BULLET, 0, makecol(0, 0, 0));
	plane_type_prerotate(&plane_types[2], 32);

	plane_types[3].radius = 20;
	plane_type_colorize(&plane_types[3], MISSILE1, 0, makecol(0, 0, 0));
	plane_type_prerotate(&plane_types[3], 32);

	plane_types[4].radius = 22;
	plane_type_colorize(&plane_types[4], MISSILE2, 0, makecol(0, 0, 0));
	plane_type_prerotate(&plane_types[4], 32);

	plane_types[5].radius = 18;
	plane_type_colorize(&plane_types[5], MISSILE3, 0, makecol(0, 0, 0));
	plane_type_prerotate(&plane_types[5], 32);

	plane_types[6].radius = 15;
	plane_type_colorize(&plane_types[6], SMOKE, 0, makecol(0, 0, 0));
	plane_type_prerotate(&plane_types[6], 8);

	for (i = BIGEXP1; i <= BIGEXP10; i++) {
		bigexp[i - BIGEXP1] = (BITMAP*)data[i].dat;
	}

	BITMAP *b = data[BUILDING_BMP].dat;
	building[0] = object_sprite(b, b->w / 2, b->h - 1);
	b = data[BUILDING_DESTROYED_BMP].dat;
	building[1] = object_sprite(b, b->w / 2, b->h - 1);

	tree[0] = object_sprite((BITMAP*)data[TREE1].dat, 20, 70);
	tree[1] = object_sprite((BITMAP*)data[TREE2].dat, 20, 70);

	menu_init();
}

void game_tick(void)
{
	PLANE *p = game.player1;
	if (game.mission)
		player_tick(p);

	int i;
	for (i = 0; game.AI[i]; i++)
	{
		AI_tick(game.AI[i]);
	}

	for (i = 0; i < game.bullets_count; i++)
	{
		bullet_tick(&game.bullets[i]);
	}
	for (i = 0; i < game.bullets_count; i++)
	{
		if (game.bullets[i].destroyed)
		{
			bullet_remove(i);
			if (i > 0)
				i--;
		}
	}

	/* Center around where player is expected in 1/4 second. */
	game.scroll_x = p->x + p->vx * 60 / 4;
	game.scroll_y = p->y + p->vy * 60 / 4;

	if (game.scroll_y < SCREEN_H / 2 - 1)
		game.scroll_y = SCREEN_H / 2 - 1;

	if (game.mission_timer)
	{
		game.mission_timer--;
		if (game.mission_timer == 0)
		{
			if (!game.player1->destroyed)
				game.mission += 1;
			game_restart(&game, game.mission);
			return;
		}
	}
	else
	{
		if (game.player1->destroyed)
		{
			game.mission_timer = 60 * 2;
		}
		else if (game.ai_down == game.num_ai &&
			game.buildings_down == game.buildings_count)
		{
			game.mission_timer = 60 * 2;
		}
	}

	for (i = 0; i < game.num_exps; i++)
		if (!explosion_tick(&game.exps[i]))
			remove_explosion(i);
}

#ifdef SHOW_GRID
static void draw_grid(void)
{
	int white = makecol(255, 255, 255);
	int i;

	int quadrant_x = game.scroll_x / 100;
	int x = SCREEN_W / 2 + quadrant_x * 100 - game.scroll_x;
	for (i = 0, x = x - 400; x < SCREEN_W; x += 100, i++)
	{
		vline(buffer, x, 0, SCREEN_H - 1, white);
		if (quadrant_x - 4 + i == 0)
		{
			vline(buffer, x - 2, 0, SCREEN_H - 1, white);
			vline(buffer, x + 2, 0, SCREEN_H - 1, white);
		}
	}

	int quadrant_y = game.scroll_y / 100;
	int y = SCREEN_H / 2 - quadrant_y * 100 + game.scroll_y;
	for (i = 0, y = y + 400; y >= 0; y -= 100, i++)
	{
		hline(buffer, 0, y, SCREEN_W - 1, white);
		if (quadrant_y - 4 + i == 0)
		{
			hline(buffer, 0, y - 2, SCREEN_W - 1, white);
			hline(buffer, 0, y + 2, SCREEN_W - 1, white);
		}
	}

	PLANE *p = game.player1;
}
#endif

#define cloud1 (BITMAP*)data[CLOUD1].dat
#define cloud2 (BITMAP*)data[CLOUD2].dat
#define cloud3 (BITMAP*)data[CLOUD3].dat

void draw_sky()
{
	static int cloudx[6] = { 200, 670, 420, 350, 100, 600 };
	static int cloudy[6] = { 320, 120, 130, 460, 100, 380 };
	BITMAP *cloud_bits[6] = { cloud1, cloud1, cloud2, cloud2, cloud3, cloud3 };
	int skyblue = makecol(0, 102, 255);
	int i, j, k;

	clear_to_color(buffer, skyblue);
#ifdef SHOW_GRID
	draw_grid();
#endif

	int quadrant_x = game.scroll_x / SCREEN_W;
	int quadrant_y = game.scroll_y / SCREEN_H;
	int y = - quadrant_y * SCREEN_H + game.scroll_y;

	for (j = 0; j < 2; j++)
	{
		int x = quadrant_x * SCREEN_W - game.scroll_x;
		for (i = 0; i < 2; i++)
		{
			if (quadrant_y - j >= 0)
				for (k = 0; k < 6; k++)
					draw_sprite(buffer, cloud_bits[k], x + cloudx[k], y - cloudy[k]);
			x += SCREEN_W;
		}
		y += SCREEN_H;
	}
}

void game_draw(void)
{
	FONT *big = data[FONT_PCX].dat;
	int i;
	PLANE *p = game.player1;
	draw_sky();
#ifdef SHOW_GRID
	int white = makecol(255, 255, 255);
	textprintf_ex(buffer, font, 0, 0, white, -1, "pos: %d pix alt: %d pix, vel: %d pix/sec",
		(int)p->x, (int)p->y, (int)(60 * sqrt(p->vx * p->vx + p->vy * p->vy)));
	
	textprintf_ex(buffer, font, 0, 10, white, -1, "left/right: wings");
	textprintf_ex(buffer, font, 0, 20, white, -1, "up/down: engines");

	textprintf_ex(buffer, font, 0, 30, white, -1, "hull: %d",
		game.player1->hull);
	textprintf_ex(buffer, font, 0, 40, white, -1, "enemy down: %d/%d",
		game.ai_down, game.num_ai);
	textprintf_ex(buffer, font, 0, 50, white, -1, "mission: %d",
		game.mission);
#endif

	select_palette(blackpal);
	textprintf_centre_ex(buffer, big, SCREEN_W / 2, 0, -1, -1,
		"%d/%d", game.ai_down + game.buildings_down,
		game.num_ai + game.buildings_count);

	/* Hull damage. */
	int blue1 = makecol(0, 0, 255);
	int blue2 = makecol(100, 130, 255);
	circle(buffer, SCREEN_W / 2 - 70, 25, 21, blue1);
	if (game.player1->hull > 0)
		circlefill(buffer, SCREEN_W / 2 - 70, 25, game.player1->hull * 2, blue2);

	/* Draw engine indicator. */
	int red = makecol(255, 0, 0);
	int orange = makecol(255, 192, 128);
	rectfill(buffer, SCREEN_W / 2 + 50, 10,
		SCREEN_W / 2 + 50 + game.player1->thrust * 100, 17, orange);
	rect(buffer, SCREEN_W / 2 + 50, 10,
		SCREEN_W / 2 + 150, 17, red);

	if (game.scroll_y < SCREEN_H / 2 + GROUND_WIDTH)
		rectfill(buffer, 0,
			SCREEN_H - GROUND_WIDTH + (game.scroll_y - SCREEN_H / 2),
			SCREEN_W, SCREEN_H, makecol(0, 255, 0));

	for (i = 0; game.objects[i]; i++)
	{
		object_draw(game.objects[i]);
	}

	for (i = 0; game.buildings[i]; i++)
	{
		object_draw(game.buildings[i]);
	}

	int bx, by;
	fixed angle;
	for (i = 0; game.AI[i]; i++)
	{
		PLANE *a = game.AI[i];
		if (a->thrust && !a->destroyed)
		{
			bx = SCREEN_W / 2 + a->x - a->hx * 40 - game.scroll_x;
			by = SCREEN_H / 2 - a->y + a->hy * 40 + game.scroll_y;
			angle = -ftofix(plane_get_angle(a));
			pivot_sprite(buffer, (BITMAP*)data[ENGINE_FLAME + a->burner_time].dat,
				bx, by, 2, 2, angle);
			a->burner_time = !a->burner_time;
		}
                plane_draw(a);
	}

	if (p->thrust && !p->destroyed)
	{
		bx = SCREEN_W / 2 + p->x - p->hx * 40 - game.scroll_x;
		by = SCREEN_H / 2 - p->y + p->hy * 40 + game.scroll_y;
		angle = -ftofix(plane_get_angle(p));
		pivot_sprite(buffer, (BITMAP*)data[ENGINE_FLAME + p->burner_time].dat,
			bx, by, 2, 2, angle);
		p->burner_time = !p->burner_time;
	}

	plane_draw(p);

	for (i = 0; i < game.bullets_count; i++)
	{
		plane_draw(&game.bullets[i]);
	}

	for (i = 0; i < game.num_exps; i++)
		draw_explosion(&game.exps[i], game.scroll_x, game.scroll_y);

	draw_radar();

	if (game.mission_timer)
	{
		int m = game.mission + (game.player1->destroyed ? 0 : 1);
		textprintf_centre_ex(buffer, big,
			SCREEN_W / 2, SCREEN_H / 2 - 70, -1, -1,
			"Mission %d", m);
		textprintf_centre_ex(buffer, big,
			SCREEN_W / 2, SCREEN_H / 2 - 10, -1, -1,
			"%s", mission_names[m]);
		select_palette(redpal);
		textprintf_centre_ex(buffer, big,
			SCREEN_W / 2, SCREEN_H / 2 + 50, -1, -1,
			"%s", mission_objectives[m]);
	}
}

PLANE *game_collision(GAME *self, PLANE *p)
{
	if (p != game.player1)
		if (plane_collides(p, game.player1))
			return game.player1;
	int i;
	for (i = 0; self->AI[i]; i++)
	{
		if (p != game.AI[i] && !p->destroyed)
			if (plane_collides(p, game.AI[i]))
				return game.AI[i];
	}
	return NULL;
}

OBJECT *game_building_collision(GAME *self, PLANE *p)
{
	int i;
	float a = plane_get_angle(p);
	for (i = 0; i < self->buildings_count; i++)
	{
		OBJECT *o = self->buildings[i];
		if (sprite_collides(
			o->sprite, o->x, -o->y, 0,
			plane_get_sprite(p), p->x, -p->y,
			a))
			return o;
	}
	return NULL;
}

void game_exit(void)
{
	free(mainm.items);
}

void game_add_object(GAME *self, OBJECT *o)
{
	int i;
	for (i = 0; self->objects[i]; i++);
	self->objects[i] = o;
}

void game_add_building(GAME *self, OBJECT *o)
{
	int i;
	for (i = 0; self->buildings[i]; i++);
	self->buildings[i] = o;
}

void screenshot(char *filename)
{
	save_bmp(filename, buffer, NULL);
}

void sound(int id, float x, float y, int vol, int pitch)
{
	float v;
	float dx, dy;
	plane_distance (game.player1, x, y, &dx, &dy);
	float squared = dx * dx + dy * dy;
	if (squared > SCREEN_W * SCREEN_W)
		return;
	if (squared < 100 * 100)
		v = 1;
	else
		v = 100.0 / sqrt(squared);
	play_sample(data[id].dat, MIN(255, vol * v), 128 + dx / 20.0, pitch, 0);
}

void explode(int x, int y)
{
	sound(EXPL_WAV, x, y, 400, 1000);
	if (game.num_exps == MAX_EXPLOSIONS_COUNT - 1)
		remove_explosion(0);

	if (make_explosion(&game.exps[game.num_exps], x, y, 10, bigexp))
	{
		game.num_exps++;
	}
}

void remove_explosion(int i)
{
	int last = game.num_exps - 1;
	game.exps[i] = game.exps[last];
	game.num_exps--;
}
