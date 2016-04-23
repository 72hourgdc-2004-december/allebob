
#include "radar.h"

int radar_corner = 1;
char *corners[4] = { "Top left", "Bottom left", "Bottom right", "Top right" };

static void distance(float x, float y, float x_, float y_, float *dx, float *dy)
{
	*dx = x_ - x;
	*dy = y_ - y;
	if (*dx > WRAP_AROUND / 2)
		*dx -= WRAP_AROUND;
	if (*dx < -WRAP_AROUND / 2)
		*dx += WRAP_AROUND;
}

void draw_radar()
{
	int dx, dy, edist, i;
	float ex, ey;
	switch (radar_corner) {
		case 0:	// top left
			dx = 55;
			dy = 55;
			break;
		case 2:	// bottom right
			dx = 745;
			dy = 545;
			break;
		case 3:	// top right
			dx = 745;
			dy = 55;
			break;
		case 1: // bottom left
		default:
			dx = 55;
			dy = 545;
			break;
	}

	int playercol = makecol(0, 255, 0);
	int enemycol = makecol(255, 0, 0);

	circle(buffer, dx, dy, 50, makecol(0, 0, 0));
	circle(buffer, dx, dy, 48, makecol(0, 0, 0));

	for (i = 0; i < game.num_ai; i++) {
		if (game.AI[i]->destroyed)
			continue;
		distance(game.player1->x, game.player1->y,
			game.AI[i]->x, game.AI[i]->y, &ex, &ey);
		ex /= 30;
		ey /= 30;
		edist = sqrt(ex * ex + ey * ey);
		ex = dx + ex;
		ey = dy - ey;

		if (edist < 50)
			circlefill(buffer, ex, ey, 1, enemycol);
	}

	for (i = 0; i < game.buildings_count; i++) {
		if (game.buildings[i]->destroyed)
			continue;
		distance(game.player1->x, game.player1->y,
			game.buildings[i]->x, game.buildings[i]->y, &ex, &ey);
		ex /= 30;
		ey /= 30;
		edist = sqrt(ex * ex + ey * ey);
		ex = dx + ex;
		ey = dy - ey - 2;
		if (edist < 50)
			circlefill(buffer, ex, ey, 2, enemycol);
	}

	if (game.player1->y < 50 * 30)
	{
		float h = game.player1->y / 30;
		float hx = sqrt(50 * 50 - h * h);
		hline(buffer, dx - hx, dy + h, dx + hx,
			makecol(0, 150, 0));
	}
	circlefill(buffer, dx, dy, 1, playercol);
}
