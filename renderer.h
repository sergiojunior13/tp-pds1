#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

typedef struct Hp Hp;
typedef enum Imgs_Ids Imgs_Ids;
typedef struct Game Game;

typedef struct Renderer {
  ALLEGRO_DISPLAY* display;
  ALLEGRO_BITMAP* display_buffer;

  ALLEGRO_FONT* font;

} Renderer;

void FillRenderer(Renderer* renderer);

void Render(Renderer* renderer, Game* game);

void ClearRenderer(Renderer* renderer);

void DrawScaledText(ALLEGRO_FONT* font, ALLEGRO_COLOR color, float x, float y,
  float x_scale, float y_scale, int alignment,
  const char* text);

void RenderHealthBar(const Hp* hp, float x_begin, float x_end, float y_begin,
  ALLEGRO_FONT* font);

void RenderImage(Imgs_Ids img_id, float x, float y, float scale);

#endif