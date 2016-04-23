
#include "game.h"

BITMAP *buffer;
DATAFILE *data;
int quit = 0, menu_active = 1, key_wait = 0, button_wait = 0;

volatile int count = 0;
void increment()
{
	count++;
} END_OF_FUNCTION(increment);

int main()
{
#ifdef PAGEFLIP
	BITMAP *page1, *page2;
#endif
	allegro_init();
	install_keyboard();
	install_mouse();

	set_color_depth(32);
	if (set_gfx_mode(GFX_AUTODETECT, 800, 600, 0, 0) < 0) {
		set_color_depth(16);
		if (set_gfx_mode(GFX_AUTODETECT, 800, 600, 0, 0) < 0) {
			allegro_message("Couldn't set graphics mode... quitting!");
			return 1;
		}
	}
	#ifdef PAGEFLIP
	page1 = create_video_bitmap(SCREEN_W, SCREEN_H);
	page2 = create_video_bitmap(SCREEN_W, SCREEN_H);
	buffer = page1;
	#else
	buffer = create_bitmap(SCREEN_W, SCREEN_H);
	#endif
	clear_bitmap(buffer);

	install_timer();
	LOCK_VARIABLE(count);
	LOCK_FUNCTION(increment);
	install_int_ex(increment, BPS_TO_TIMER(60));

	reserve_voices(8, 0);
	install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL);

	data = load_datafile("allebob.dat");

	game_init();

	int ticks = count;
	int drawn_frames = 0;
	while (!quit) {
		while (ticks <= count) {
			if (key[KEY_ESC] && key_wait > 12) {
				if (menu_active)
					quit = 1;
				else
					menu_active = 1;
				key_wait = 0;
			}
			if (key[KEY_F1] && key_wait > 12) {
				screenshot("screen.bmp");
				key_wait = 0;
			}

                        if (menu_active)
				menu_tick();
                        else
                        {
				game_tick();
                        }
			key_wait++;
			button_wait++;
			ticks++;
		}
                if (drawn_frames < ticks)
		{
			clear(buffer);
                        if (menu_active)
			{
				draw_menu(&mainm);
				draw_sprite(buffer, mouse_sprite, mouse_x, mouse_y);
			}
			else
				game_draw();
			#ifdef PAGEFLIP
				show_video_bitmap(buffer);
				if (buffer == page1)
					buffer = page2;
				else
					buffer = page1;
			#else
			blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
			#endif
			drawn_frames = ticks;
		}
		#ifndef PAGEFLIP
		else
			rest(1);
		#endif
	}
	game_exit();

	unload_datafile(data);
	destroy_bitmap(buffer);
	return 0;
} END_OF_MAIN()
