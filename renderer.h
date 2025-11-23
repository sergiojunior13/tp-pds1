#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

typedef struct Hp Hp;
typedef enum Imgs_Ids Imgs_Ids;
typedef struct Game Game;

typedef struct Font {
  ALLEGRO_FONT* ptr;
  int size;
} Font;

typedef struct Renderer {
  ALLEGRO_DISPLAY* display;
  float display_width;
  float display_height;
  ALLEGRO_BITMAP* display_buffer;
} Renderer;

void FillRenderer(Renderer* renderer);

void Render(Renderer* renderer, Game* game);

void ClearRenderer(Renderer* renderer);

void DrawMultilineText(ALLEGRO_COLOR color, int font_size, float x, float y, float max_width, int alignment, const char* text);

void DrawText(ALLEGRO_COLOR color, int font_size, float x, float y, int alignment, const char* text);

void RenderHealthBar(const Hp* hp, float x_begin, float x_end, float y_begin,
  ALLEGRO_COLOR color);

float RenderImage(Imgs_Ids img_id, float x, float y, float width);

#endif