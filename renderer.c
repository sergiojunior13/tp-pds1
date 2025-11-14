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

void RenderBuyDeck(Renderer* renderer, Game* game) {
  float x = DRAW_DECK_DISTANCE_X_TO_BORDER;
  float y = DISPLAY_HEIGHT - DRAW_DECK_DISTANCE_Y_TO_BORDER - DECK_HEIGHT;

  RenderImage(Buy_Stack_Img_Id, x, y, DECK_WIDTH);

  char n_buy_cards_text[2];
  sprintf(n_buy_cards_text, "%d", game->buy_size);

  float text_scale = 2.6;
  float text_x = x + DECK_WIDTH - 33;
  float text_y = y + DECK_HEIGHT - 42;

  DrawScaledText(renderer->font, al_map_rgb(255, 255, 255), text_x, text_y, text_scale, text_scale, ALLEGRO_ALIGN_CENTER, n_buy_cards_text);
}

void RenderDiscardDeck(Renderer* renderer, Game* game) {
  float x = DISPLAY_WIDTH - DRAW_DECK_DISTANCE_X_TO_BORDER - DECK_WIDTH;
  float y = DISPLAY_HEIGHT - DRAW_DECK_DISTANCE_Y_TO_BORDER - DECK_HEIGHT;

  RenderImage(Discard_Stack_Img_Id, x, y, DECK_WIDTH);

  char n_discard_cards_text[2];
  sprintf(n_discard_cards_text, "%d", game->discard_size);

  float text_scale = 2.6;
  float text_x = x + 33;
  float text_y = y + DECK_HEIGHT - 42;

  DrawScaledText(renderer->font, al_map_rgb(255, 255, 255), text_x, text_y, text_scale, text_scale, ALLEGRO_ALIGN_CENTER, n_discard_cards_text);
}

void RenderHealthBar(const Hp* hp, float x_begin, float x_end, float y_begin,
  ALLEGRO_FONT* font, ALLEGRO_COLOR color) {
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
    HEALTH_BAR_RX, HEALTH_BAR_RY, color);

  // Make the bar color darker
  color.r /= 1.3;
  color.g /= 1.3;
  color.b /= 1.3;

  // Draw half of the bar with diff color
  al_draw_filled_rounded_rectangle(
    x_begin,
    y_begin + HEALTH_BAR_PADDING,
    health_bar_width_life_percentage,
    y_begin + (HEALTH_BAR_HEIGHT / 2.0) + HEALTH_BAR_PADDING,
    HEALTH_BAR_RX, HEALTH_BAR_RY, color);

  char text[100] = "";
  sprintf(text, "%d/%d", hp->crr, hp->max);

  float x_scale = 1.7, y_scale = 1.7;
  float center_x = (x_begin + x_end) / 2.0;
  float text_begin_y = y_begin + HEALTH_BAR_PADDING + (HEALTH_BAR_HEIGHT - DEFAULT_LINE_HEIGHT) / 2.0;

  DrawScaledText(font, al_map_rgb(0, 0, 0), center_x,
    text_begin_y, x_scale, y_scale, ALLEGRO_ALIGN_CENTER, text);
}

void RenderEnergy(Renderer* renderer) {

}

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
  RenderBuyDeck(renderer, game);
  RenderDiscardDeck(renderer, game);
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