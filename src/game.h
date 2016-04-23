#ifndef _GAME_H
#define _GAME_H

typedef struct GAME GAME;

#include "plane.h"
#include "menu.h"
#include "actors.h"
#include "radar.h"
#include "object.h"
#include "explosion.h"

#define MAX_AI_COUNT 100
#define MAX_BULLETS_COUNT 100
#define MAX_OBJECTS_COUNT 500
#define WRAP_AROUND (SCREEN_W * 12)
#define MAX_EXPLOSIONS_COUNT 10
#define MAX_BUILDINGS_COUNT 10


/* The coordinate system looks like this:

height
  300 |
  200 |
  100 |
    0 |_______________________________________
      0 100 200 300 400 ... wrap around to 0 ...

  scroll_x/scroll_y is the position in the world of the middle of the
  screen.

*/

struct GAME
{
	PLANE *player1;
	float scroll_x, scroll_y;

	PLANE *AI[MAX_AI_COUNT]; /* TODO make proper linked list */
	int num_ai;

	PLANE bullets[MAX_BULLETS_COUNT]; /* same, all in same list */
	int bullets_count;

	OBJECT *objects[MAX_OBJECTS_COUNT]; /* see above */
	OBJECT *buildings[MAX_BUILDINGS_COUNT];
	int buildings_count;

	int buildings_down;

	int ai_down;
	int mission;
	int mission_timer;

	explosion exps[MAX_EXPLOSIONS_COUNT];
	int num_exps;
};

extern GAME game;
extern SPRITE *building[2];
extern SPRITE *tree[2];

void game_tick(void);
void game_draw(void);
void game_init(void);
void game_exit(void);

void game_restart(GAME *self, int mission);

PLANE *game_collision(GAME *self, PLANE *p);
void game_add_object(GAME *self, OBJECT *o);
void game_add_building(GAME *self, OBJECT *o);
void screenshot(char *filename);
void explode(int x, int y);
void sound(int id, float x, float y, int vol, int pitch);
OBJECT *game_building_collision(GAME *self, PLANE *p);

#endif
