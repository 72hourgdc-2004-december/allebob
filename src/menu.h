
#ifndef MENU_H
#define MENU_H

#include "globals.h"
#include "radar.h"

typedef struct {
	int x, y, w, h;
	BITMAP *bit, *hover, *selected;
} menu_item;

typedef struct {
	BITMAP *background;
	int selection, num_items, items_added;
	menu_item *items;
} menu;

extern menu mainm;
extern int pr, pg, pb, er, eg, eb;

void menu_init();
void menu_tick();
int make_menu(menu *m, int i, BITMAP *bg);
int add_menu_item(menu *m, int i, int x, int y, int w, int h, BITMAP *bit, BITMAP *hover, BITMAP *selected);
int check_mouse_in_box(int l, int t, int r, int b);
int check_mouse_selection(menu *m);
void draw_menu(menu *m);

#endif
