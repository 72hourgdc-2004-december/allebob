
#include "menu.h"

menu mainm;
int pr, pg, pb, er, eg, eb;
BITMAP *pjet, *ejet;

void menu_init()
{
	pr = pg = pb = er = eg = eb = 128;

	pjet = create_bitmap(200, 53);
	draw_sprite(pjet, (BITMAP*)data[MENUJET].dat, 0, 0);
	ejet = create_bitmap(200, 53);
	draw_sprite(ejet, (BITMAP*)data[MENUJET].dat, 0, 0);

	make_menu(&mainm, 11, (BITMAP*)data[MENUBG].dat);

	add_menu_item(&mainm, 0, 500, 450, 250, 100, (BITMAP*)data[PLAY].dat, (BITMAP*)data[PLAY_HOVER].dat, (BITMAP*)data[PLAY_HOVER].dat);

	add_menu_item(&mainm, 1, 60, 180, 40, 40, (BITMAP*)data[RED].dat, (BITMAP*)data[RED_HOVER].dat, (BITMAP*)data[RED_HOVER].dat);
	add_menu_item(&mainm, 2, 130, 180, 40, 40, (BITMAP*)data[GREEN].dat, (BITMAP*)data[GREEN_HOVER].dat, (BITMAP*)data[GREEN_HOVER].dat);
	add_menu_item(&mainm, 3, 200, 180, 40, 40, (BITMAP*)data[BLUE].dat, (BITMAP*)data[BLUE_HOVER].dat, (BITMAP*)data[BLUE_HOVER].dat);

	add_menu_item(&mainm, 4, 60, 420, 40, 40, (BITMAP*)data[RED].dat, (BITMAP*)data[RED_HOVER].dat, (BITMAP*)data[RED_HOVER].dat);
	add_menu_item(&mainm, 5, 130, 420, 40, 40, (BITMAP*)data[GREEN].dat, (BITMAP*)data[GREEN_HOVER].dat, (BITMAP*)data[GREEN_HOVER].dat);
	add_menu_item(&mainm, 6, 200, 420, 40, 40, (BITMAP*)data[BLUE].dat, (BITMAP*)data[BLUE_HOVER].dat, (BITMAP*)data[BLUE_HOVER].dat);

	add_menu_item(&mainm, 7, 350, 480, 40, 40, (BITMAP*)data[MENURADAR].dat, (BITMAP*)data[MENURADAR].dat, (BITMAP*)data[MENURADAR].dat);
	add_menu_item(&mainm, 8, 525, 300, 200, 100, (BITMAP*)data[BACKTOGAME].dat, (BITMAP*)data[BACKTOGAME_HOVER].dat, (BITMAP*)data[BACKTOGAME].dat);

	add_menu_item(&mainm, 9, 300, 120, 150, 50, (BITMAP*)data[MENU_WEAPON1].dat, (BITMAP*)data[MENU_WEAPON1_HOVER].dat, (BITMAP*)data[MENU_WEAPON1_HOVER].dat);
	add_menu_item(&mainm, 10, 300, 220, 150, 50, (BITMAP*)data[MENU_WEAPON2].dat, (BITMAP*)data[MENU_WEAPON2_HOVER].dat, (BITMAP*)data[MENU_WEAPON2_HOVER].dat);
}

int make_menu(menu *m, int i, BITMAP *bg)
{
	m->items_added = 0;
	m->num_items = i;
	if (!(m->items = (menu_item*)malloc(sizeof(menu_item) * i))) {
		return 0;
	}
	m->background = bg;
	m->selection = -1;
	return 1;
}

int add_menu_item(menu *m, int i, int x, int y, int w, int h, BITMAP *bit, BITMAP *hover, BITMAP *selected)
{
	if (i < m->num_items) {
		m->items[i].bit = bit;
		m->items[i].hover = hover;
		m->items[i].selected = selected;
		m->items[i].x = x;
		m->items[i].y = y;
		m->items[i].w = w;
		m->items[i].h = h;
		m->items_added++;
		return 1;
	} else {
		return 0;
	}
}

int check_mouse_in_box(int left, int top, int right, int bottom)
{
	if (mouse_x >= left && mouse_x <= right && mouse_y <= bottom && mouse_y >= top) {
		return 1;
	}
	return 0;
}

static void recolorize_menu_jets(int pcolor, int ecolor)
{
	BITMAP *maskbit = (BITMAP*)data[MENUJET_COLORMASK].dat;
	BITMAP *basebit = (BITMAP*)data[MENUJET].dat;
	int mask = bitmap_mask_color(basebit);
	clear_to_color(pjet, mask);
	clear_to_color(ejet, mask);

	draw_sprite(pjet, basebit, 0, 0);
	draw_sprite(ejet, basebit, 0, 0);

	int r, g, b, i, j, col;
	float ch, cs, h, s, v;

	rgb_to_hsv(getr(pcolor), getg(pcolor), getb(pcolor), &ch, &cs, &v);
	for (i = 0; i < pjet->w; i++) {
		for (j = 0; j < pjet->h; j++) {
			col = getpixel(maskbit, i, j);
			if (col != mask) {
				rgb_to_hsv(getr(col), getg(col), getb(col), &h, &s, &v);
				hsv_to_rgb(ch, cs, v, &r, &g, &b);
				putpixel(pjet, i, j, makecol(r, g, b));
			}
		}
	}

	rgb_to_hsv(getr(ecolor), getg(ecolor), getb(ecolor), &ch, &cs, &v);
	for (i = 0; i < ejet->w; i++) {
		for (j = 0; j < ejet->h; j++) {
			col = getpixel(maskbit, i, j);
			if (col != mask) {
				rgb_to_hsv(getr(col), getg(col), getb(col), &h, &s, &v);
				hsv_to_rgb(ch, cs, v, &r, &g, &b);
				putpixel(ejet, i, j, makecol(r, g, b));
			}
		}
	}
}

void menu_tick()
{
	int i;
	for (i = 0; i < mainm.num_items; i++) {
		if (check_mouse_in_box(mainm.items[i].x, mainm.items[i].y, mainm.items[i].x + mainm.items[i].w, mainm.items[i].y + mainm.items[i].h)) {
			if (mouse_b & 1 && button_wait > 10) {
				switch (i) {
					case 0:
						if (pr != 128 || pg != 128 || pb != 128) {
							plane_type_colorize(&plane_types[0], JET, JET_COLORMASK, makecol(pr, pg, pb));
							plane_type_prerotate(&plane_types[0], 64);
						}
						if (er != 128 || eg != 128 || eb != 128) {
							plane_type_colorize(&plane_types[1], JET, JET_COLORMASK, makecol(er, eg, eb));
							plane_type_prerotate(&plane_types[1], 64);
						}
						menu_active = 0;
						game_restart(&game, 0);
						break;
					case 7:
						radar_corner++;
						if (radar_corner > 3)
							radar_corner = 0;
						break;
					case 8:
						menu_active = 0;
						break;
					case 9:
						weapons[0]++;
						if (weapons[0] > 5)
							weapons[0] = 2;
						break;
					case 10:
						weapons[1]++;
						if (weapons[1] > 5)
							weapons[1] = 2;
						break;
					default:
						mainm.selection = i;
						break;
				}
				button_wait = 0;
			}
		}
	}

	// player red
	if (mainm.selection == 1 && key[KEY_UP] && key_wait > 3 && pr < 255)
		pr++;
	else if (mainm.selection == 1 && key[KEY_DOWN] && key_wait > 3 && pr > 0)
		pr--;
	// player green
	else if (mainm.selection == 2 && key[KEY_UP] && key_wait > 3 && pg < 255)
		pg++;
	else if (mainm.selection == 2 && key[KEY_DOWN] && key_wait > 3 && pg > 0)
		pg--;
	// player blue
	else if (mainm.selection == 3 && key[KEY_UP] && key_wait > 3 && pb < 255)
		pb++;
	else if (mainm.selection == 3 && key[KEY_DOWN] && key_wait > 3 && pb > 0)
		pb--;
	// enemy red
	else if (mainm.selection == 4 && key[KEY_UP] && key_wait > 3 && er < 255)
		er++;
	else if (mainm.selection == 4 && key[KEY_DOWN] && key_wait > 3 && er > 0)
		er--;
	// enemy green
	else if (mainm.selection == 5 && key[KEY_UP] && key_wait > 3 && eg < 255)
		eg++;
	else if (mainm.selection == 5 && key[KEY_DOWN] && key_wait > 3 && eg > 0)
		eg--;
	// enemy blue
	else if (mainm.selection == 6 && key[KEY_UP] && key_wait > 3 && eb < 255)
		eb++;
	else if (mainm.selection == 6 && key[KEY_DOWN] && key_wait > 3 && eb > 0)
		eb--;
	recolorize_menu_jets(makecol(pr, pg, pb), makecol(er, eg, eb));
}

void draw_menu(menu *m)
{
	acquire_bitmap(buffer);
	if (m->background) {
		draw_sprite(buffer, m->background, 0, 0);
	}

	BITMAP *dbit;
	int i;
	for (i = 0; i < m->items_added; i++) {
		if (m->selection == i) {
			dbit = m->items[i].selected;
		} else if (check_mouse_in_box(m->items[i].x, m->items[i].y, m->items[i].x + m->items[i].w, m->items[i].y + m->items[i].h)) {
			dbit = m->items[i].hover;
		} else {
			dbit = m->items[i].bit;
		}
		if (dbit)
			draw_sprite(buffer, dbit, m->items[i].x, m->items[i].y);

	}
	draw_sprite(buffer, pjet, 50, 120);
	draw_sprite(buffer, ejet, 50, 480);

	draw_sprite(buffer, (BITMAP*)data[weapons[0] - 2 + MENU_CHAINGUN].dat, 550, 120);
	draw_sprite(buffer, (BITMAP*)data[weapons[1] - 2 + MENU_CHAINGUN].dat, 550, 220);

	int white = makecol(255, 255, 255);

	textprintf_right_ex(buffer, font, 100, 225, white, -1, "%d", pr);
	textprintf_right_ex(buffer, font, 170, 225, white, -1, "%d", pg);
	textprintf_right_ex(buffer, font, 240, 225, white, -1, "%d", pb);
	textprintf_right_ex(buffer, font, 100, 410, white, -1, "%d", er);
	textprintf_right_ex(buffer, font, 170, 410, white, -1, "%d", eg);
	textprintf_right_ex(buffer, font, 240, 410, white, -1, "%d", eb);

	textprintf_centre_ex(buffer, font, 150, 100, white, -1, "Player Color");
	textprintf_centre_ex(buffer, font, 150, 550, white, -1, "Enemy Color");

	textprintf_ex(buffer, font, 400, 500, white, -1, corners[radar_corner]);
	release_bitmap(buffer);
}
