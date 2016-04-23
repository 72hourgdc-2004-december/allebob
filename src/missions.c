#include "missions.h"

static void mission0(void)
{
	PLANE *p = plane_new();
	p->x = 0;
	p->y = 1100;
	p->hx = 1;
	p->hull = 10;
	game.player1 = p;
}

static void mission1(void)
{
	PLANE *p = plane_new();
	p->x = 0;
	p->y = 1100;
	p->hx = 1;
	p->hull = 10;
	game.player1 = p;

	building_place(building[0], 1000, 5);
	building_place(building[0], 3000, 5);
	building_place(building[0], 6000, 5);
	game.buildings_count = 3;
}

static void mission2(void)
{
	PLANE *p = plane_new();
	p->x = 0;
	p->y = 1100;
	p->hx = 1;
	p->hull = 10;
	game.player1 = p;

	int i;
	for (i = 0; i < 1; i++)
	{
		p = plane_new();
		p->x = i * 200;
		p->y = 500;
		p->hx = 1;
		p->type = 1;
		p->ai_dumb = 2;
		p->hull = 10;
		game.AI[i] = p;
		game.num_ai++;
	}
}

static void mission3(void)
{
	PLANE *p = plane_new();
	p->x = 0;
	p->y = 1100;
	p->hx = 1;
	p->hull = 10;
	game.player1 = p;

	int i;
	for (i = 0; i < 2; i++)
	{
		p = plane_new();
		p->x = i * 200;
		p->y = 500;
		p->hx = 1;
		p->type = 1;
		p->ai_dumb = 1;
		p->hull = 10;
		game.AI[i] = p;
		game.num_ai++;
	}
}

static void mission4(void)
{
	PLANE *p = plane_new();
	p->x = 0;
	p->y = 1100;
	p->hx = 1;
	p->hull = 10;
	game.player1 = p;

	int i;
	for (i = 0; i < 1; i++)
	{
		p = plane_new();
		p->x = i * 200;
		p->y = 500;
		p->hx = 1;
		p->type = 1;
		p->ai_dumb = 0;
		p->hull = 10;
		game.AI[i] = p;
		game.num_ai++;
	}
}

static void mission5(void)
{
	PLANE *p = plane_new();
	p->x = 0;
	p->y = 100;
	p->hx = 1;
	p->hull = 10;
	game.player1 = p;

	building_place(building[0], 2000, 5);
	building_place(building[0], 4000, 5);
	building_place(building[0], 8000, 5);
	game.buildings_count = 3;

	int i;
	for (i = 0; i < 10; i++)
	{
		p = plane_new();
		p->x = i * 900;
		p->y = 500;
		p->hx = 1;
		p->type = 1;
		p->ai_dumb = i == 5 ? 0 : 3;
		p->hull = 10;
		game.AI[i] = p;
		game.num_ai++;
	}
}


static void mission6(void)
{
	PLANE *p = plane_new();
	p->x = 0;
	p->y = 100;
	p->hx = 1;
	p->hull = 10;
	game.player1 = p;
	int i;

	for (i = 0; i < 50; i++)
	{
		building_place(building[0], 500 + i * 150, 5);
		game.buildings_count++;
	}

	
	for (i = 0; i < 1; i++)
	{
		p = plane_new();
		p->x = 4800;
		p->y = 500;
		p->hx = 1;
		p->type = 1;
		p->ai_dumb = 1;
		p->hull = 10;
		game.AI[i] = p;
		game.num_ai++;
	}
}

char const *mission_names[] =
{
	NULL,
	"Sudden Strike",
	"First Flight",
	"One vs Two",
	"Top Gun",
	"Outnumbered",
	"Raider"
};

char const *mission_objectives[] =
{
	NULL,
	"Destroy all enemy buildings!",
	"Destroy your opponent!",
	"Destroy all enemy forces!",
	"Win the dogfight!",
	"Destroy all resistance!",
	"Missile raid!"
};

void (*missions[])(void) =
{
	mission0,
	mission1,
	mission2,
	mission3,
	mission4,
	mission5,
	mission6,
	NULL
};
