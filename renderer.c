#include "renderer.h"

#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "utils.h"
#include "core/core.h"
#include "images/load-images.h"

#include "entities/player/player.h"
#include "entities/enemy/enemy.h"
#include "entities/card/card.h"

void DrawMultilineScaledText(ALLEGRO_FONT* font, ALLEGRO_COLOR color, float x, float y, float max_width,
  float x_scale, float y_scale, int alignment, const char* text) {
  ALLEGRO_TRANSFORM transform;
  al_identity_transform(&transform);  // Start with an identity transform
  al_scale_transform(&transform, x_scale, y_scale);  // Apply scaling (e.g., sx=2.0, sy=2.0 for double size)
  al_use_transform(&transform);  // Use the transform for subsequent drawing

  // Adjust x, y to the scale 
  x /= x_scale;
  y /= y_scale;

  // al_draw_multiline_text(font, color, x, y, 150, DEFAULT_LINE_HEIGHT, alignment, text);
  al_draw_multiline_text(font, color, x, y, max_width, DEFAULT_LINE_HEIGHT, alignment, text);

  // Reset the transform
  al_identity_transform(&transform);
  al_use_transform(&transform);
}

void DrawScaledText(ALLEGRO_FONT* font, ALLEGRO_COLOR color, float x, float y,
  float x_scale, float y_scale, int alignment, const char* text) {
  ALLEGRO_TRANSFORM transform;
  al_identity_transform(&transform);  // Start with an identity transform
  al_scale_transform(&transform, x_scale, y_scale);  // Apply scaling (e.g., sx=2.0, sy=2.0 for double size)
  al_use_transform(&transform);  // Use the transform for subsequent drawing

  // Adjust x, y to the scale 
  x /= x_scale;
  y /= y_scale;

  // al_draw_multiline_text(font, color, x, y, 150, DEFAULT_LINE_HEIGHT, alignment, text);
  al_draw_text(font, color, x, y, alignment, text);

  // Reset the transform
  al_identity_transform(&transform);
  al_use_transform(&transform);
}

void FillRenderer(Renderer* renderer) {
  al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
  al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
  al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

  renderer->display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
  must_init(renderer->display, "display");

  renderer->display_buffer =
    al_create_bitmap(DISPLAY_BUFFER_WIDTH, DISPLAY_BUFFER_HEIGHT);
  must_init(renderer->display_buffer, "display buffer");

  renderer->font = al_create_builtin_font();
  must_init(renderer->font, "font");
}

void RenderBackground(Game* game) {
  int bgId = game->phase < 11 ? Normal_Bg_Img_Id : End_Bg_Img_Id;

  ALLEGRO_BITMAP* bg_img = imgs_bitmaps[bgId];

  al_draw_scaled_bitmap(
    bg_img,
    0, 0,
    al_get_bitmap_width(bg_img),
    al_get_bitmap_height(bg_img),
    0, 0,
    DISPLAY_WIDTH, DISPLAY_HEIGHT,
    0);
}

void RenderDeck(Renderer* renderer, int x_left, int y_top) {
  ALLEGRO_BITMAP* prev_bmp_target = al_get_target_bitmap();

  ALLEGRO_BITMAP* deck_bitmap = al_create_bitmap(DECK_WIDTH, DECK_HEIGHT);
  al_set_target_bitmap(deck_bitmap);

  al_draw_filled_rounded_rectangle(0, 0, DECK_WIDTH, DECK_HEIGHT, 10, 0,
    al_map_rgb(255, 255, 255));
  al_set_target_bitmap(prev_bmp_target);

  al_draw_scaled_bitmap(deck_bitmap, 0, 0, DECK_WIDTH, DECK_HEIGHT, x_left,
    y_top, DECK_WIDTH, DECK_HEIGHT, 0);
  al_destroy_bitmap(deck_bitmap);
}

void RenderHealthBar(const Hp* hp, float x_begin, float x_end, float y_begin,
  ALLEGRO_FONT* font) {
  al_draw_filled_rounded_rectangle(
    x_begin - HEALTH_BAR_PADDING,
    y_begin,
    x_end + HEALTH_BAR_PADDING,
    y_begin + HEALTH_BAR_HEIGHT + HEALTH_BAR_PADDING * 2,
    HEALTH_BAR_RX + HEALTH_BAR_PADDING, HEALTH_BAR_RY + HEALTH_BAR_PADDING, al_map_rgb(255, 255, 255));

  float health_bar_width_life_percentage = x_begin + (x_end - x_begin) * ((float)hp->crr) / hp->max;

  al_draw_filled_rounded_rectangle(
    x_begin,
    y_begin + HEALTH_BAR_PADDING,
    health_bar_width_life_percentage,
    y_begin + HEALTH_BAR_HEIGHT + HEALTH_BAR_PADDING,
    HEALTH_BAR_RX, HEALTH_BAR_RY, al_map_rgb(0, 255, 0));

  char text[100] = "";
  sprintf(text, "%d/%d", hp->crr, hp->max);

  float x_scale = 1.7, y_scale = 1.7;
  float center_x = (x_begin + x_end) / 2.0;
  float text_begin_y = y_begin + HEALTH_BAR_PADDING + (HEALTH_BAR_HEIGHT - DEFAULT_LINE_HEIGHT) / 2.0;

  DrawScaledText(font, al_map_rgb(0, 0, 0), center_x,
    text_begin_y, x_scale, y_scale, ALLEGRO_ALIGN_CENTER, text);
}

void RenderEnergy(Renderer* renderer) {}

float RenderImage(Imgs_Ids img_id, float x, float y, float width) {
  float img_w = al_get_bitmap_width(imgs_bitmaps[img_id]);
  float img_h = al_get_bitmap_height(imgs_bitmaps[img_id]);

  float scale = width / img_w;

  al_draw_scaled_bitmap(imgs_bitmaps[img_id], 0, 0, img_w, img_h, x, y, img_w * scale, img_h * scale, 0);

  return scale;
}

void Render(Renderer* renderer, Game* game) {
  al_set_target_bitmap(renderer->display_buffer);

  RenderBackground(game);
  RenderDeck(renderer, DRAW_DECK_X, DRAW_DECK_Y);
  RenderEnergy(renderer);
  RenderPlayer(renderer, &game->player);
  RenderEnemies(renderer, game);
  RenderPlayerHand(renderer, game);
  al_set_target_backbuffer(renderer->display);

  al_draw_scaled_bitmap(renderer->display_buffer, 0, 0, DISPLAY_BUFFER_WIDTH,
    DISPLAY_BUFFER_HEIGHT, 0, 0, DISPLAY_WIDTH,
    DISPLAY_HEIGHT, 0);

  al_flip_display();
}

void ClearRenderer(Renderer* renderer) {
  al_destroy_display(renderer->display);
  al_destroy_bitmap(renderer->display_buffer);
  al_destroy_font(renderer->font);
}