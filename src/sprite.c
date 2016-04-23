#include "sprite.h"
#include "pixelmask.h"

/* Makes a colored sprite from a bitmap, a color mask, and a color. */
void sprite_colorize(SPRITE *self, BITMAP *orig, BITMAP *colormask, int color)
{
	self->bitmap = create_bitmap(orig->w, orig->h);
	int pink = bitmap_mask_color(self->bitmap);
	clear_to_color(self->bitmap, pink);
	draw_sprite(self->bitmap, orig, 0, 0);

	int r, g, b, i, j, col;
	float ch, cs, h, s, v;
	rgb_to_hsv(getr(color), getg(color), getb(color), &ch, &cs, &v);

	for (i = 0; i < self->bitmap->w; i++) {
		for (j = 0; j < self->bitmap->h; j++) {
			col = getpixel(colormask, i, j);
			if (col != pink) {
				rgb_to_hsv(getr(col), getg(col), getb(col), &h, &s, &v);
				hsv_to_rgb(ch, cs, v, &r, &g, &b);
				putpixel(self->bitmap, i, j, makecol(r, g, b));
			}
		}
	}
}

/* Optimizes a bitmap to take only as little space as necesary, while
 * maintaining the correct offset in x/y.
 */
static BITMAP *optimize_bitmap(BITMAP *bmp, int *x, int *y)
{
	int l = bmp->w;
	int r = -1;
	int t = bmp->h;
	int b = -1;
	int i, j;
	for (j = 0; j < bmp->h; j++)
	{
		for (i = 0; i < bmp->w; i++)
		{
			if (getpixel(bmp, i, j) !=
				bitmap_mask_color(bmp))
			{
				if (i < l)
					l = i;
				if (j < t)
					t = j;
				if (i > r)
					r = i;
				if (j > b)
					b = j;
			}
		}
	}
	#ifdef PAGEFLIP
	BITMAP *optimized = create_video_bitmap(1 + r - l, 1 + b - t);
	#else
	BITMAP *optimized = create_bitmap(1 + r - l, 1 + b - t);
	#endif
	blit(bmp, optimized, l, t, 0, 0, optimized->w, optimized->h);
	*x -= l;
	*y -= t;
	return optimized;
}

/* Creates n pre-rotates frames and bit-masks for a sprite. */
void sprite_prerotate(SPRITE *self, int n)
{
	int i;
	int w = self->bitmap->w;
	int h = self->bitmap->h;
	self->n = n;
	self->frames = calloc(n, sizeof *self->frames);
	BITMAP *temp = create_bitmap(2 * (w + h), 2 * (w + h));
	for (i = 0; i < n; i++)
	{
		int a = itofix(256) * i / n;
		clear_to_color(temp, bitmap_mask_color(temp));
		pivot_sprite(temp, self->bitmap, w + h, w + h,
			self->x, self->y, -a);
		self->frames[i].x = w + h;
		self->frames[i].y = w + h;
		self->frames[i].bitmap = optimize_bitmap(temp,
			&self->frames[i].x, &self->frames[i].y);
		self->frames[i].mask = pixelmask_create(self->frames[i].bitmap);
	}
	destroy_bitmap(temp);
}

static int get_frame(SPRITE *self, float radians)
{
	int f = (int)(((float)self->n + 0.5) * radians / (2 * AL_PI)) % self->n;
	if (f < 0)
		f += self->n;
	return f;
}

/* Checks two sprites with pre-rotated masks for collision. */
int sprite_collides(SPRITE *self, int x, int y, float angle,
	SPRITE *other, int x_, int y_, float angle_)
{
	SPRITE_FRAME *f = &self->frames[get_frame(self, angle)];
	SPRITE_FRAME *f_ = &other->frames[get_frame(other, angle_)];
	return pixelmask_collision(
		f->mask, x - f->x, y - f->y, f->bitmap->w, f->bitmap->h,
		f_->mask, x_ - f_->x, y_ - f_->y, f_->bitmap->w, f_->bitmap->h);
}

void sprite_draw(SPRITE *self, int x, int y, float angle)
{
	SPRITE_FRAME *f = &self->frames[get_frame(self, angle)];
	draw_sprite(buffer, f->bitmap, x - f->x, y - f->y);
}
