#include "renderer.h"

#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "utils.h"
#include "core/core.h"

#include "images/load-images.h"

#include "entities/player/player.h"
#include "entities/enemy/enemy.h"
#include "entities/card/card.h"

Font fonts_loaded[20];
int fonts_size = 0;

Message message;

Font* LoadFont(int size) {
  // If already loaded font of that size
  for (int i = 0; i < fonts_size; i++) {
    if (fonts_loaded[i].size == size)
      return &fonts_loaded[i];
  }

  Font font;
  font.ptr = al_load_font("Jersey10-Regular.ttf", size, 0);
  must_init(font.ptr, "load font");
  font.size = size;

  fonts_loaded[fonts_size] = font;
  fonts_size++;

  return &fonts_loaded[fonts_size - 1];
}

void SetMessage(char* msg, float duration_in_seconds) {
  strcpy(message.value, msg);
  message.end_time_seconds = al_get_time() + duration_in_seconds;
  message.is_to_show = 1;
}

void DrawMessage(Renderer* renderer) {
  float x = renderer->display_width / 2.0,
    y = renderer->display_height / 2.0 + 120;

  DrawMultilineText(al_map_rgb(255, 255, 0), 52, x,
    y, renderer->display_width * 0.7, ALLEGRO_ALIGN_CENTER, message.value);

  if (al_get_time() >= message.end_time_seconds)
    message.is_to_show = 0;
}

void DrawMultilineText(ALLEGRO_COLOR color, int font_size, float x, float y, float max_width, int alignment, const char* text) {
  Font* font = LoadFont(font_size);

  al_draw_multiline_text(font->ptr, color, x, y, max_width, DEFAULT_LINE_HEIGHT * font_size, alignment, text);
}

void DrawText(ALLEGRO_COLOR color, int font_size, float x, float y, int alignment, const char* text) {
  Font* font = LoadFont(font_size);

  al_draw_text(font->ptr, color, x, y, alignment, text);
}

void FillRenderer(Renderer* renderer) {
  al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
  al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
  al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

  al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
  renderer->display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
  must_init(renderer->display, "display");

  renderer->display_width = al_get_display_width(renderer->display);
  renderer->display_height = al_get_display_height(renderer->display);

  renderer->display_buffer = al_create_bitmap(renderer->display_width, renderer->display_height);
  must_init(renderer->display_buffer, "display buffer");
}

void RenderBackground(Renderer* renderer, Game* game) {
  int bgId = game->phase < 11 ? Normal_Bg_Img_Id : End_Bg_Img_Id;

  ALLEGRO_BITMAP* bg_img = GetImage(bgId)->bitmap;

  al_draw_scaled_bitmap(
    bg_img,
    0, 0,
    al_get_bitmap_width(bg_img),
    al_get_bitmap_height(bg_img),
    0, 0,
    renderer->display_width, renderer->display_height,
    0);
}

void RenderBuyDeck(Renderer* renderer, Game* game) {
  float x = DECK_DISTANCE_X_TO_BORDER;
  float y = renderer->display_height - DECK_DISTANCE_Y_TO_BORDER - DECK_HEIGHT;

  RenderImage(Buy_Stack_Img_Id, x, y, DECK_WIDTH);

  char n_buy_cards_text[2];
  sprintf(n_buy_cards_text, "%d", game->buy_size);

  int font_size = 52;

  float text_x = x + DECK_WIDTH - 34;
  float text_y = y + DECK_HEIGHT - 60;

  DrawText(al_map_rgb(255, 255, 255), font_size, text_x, text_y, ALLEGRO_ALIGN_CENTER, n_buy_cards_text);
}

void RenderDiscardDeck(Renderer* renderer, Game* game) {
  float x = renderer->display_width - DECK_DISTANCE_X_TO_BORDER - DECK_WIDTH;
  float y = renderer->display_height - DECK_DISTANCE_Y_TO_BORDER - DECK_HEIGHT;

  RenderImage(Discard_Stack_Img_Id, x, y, DECK_WIDTH);

  char n_discard_cards_text[2];
  sprintf(n_discard_cards_text, "%d", game->discard_size);

  int font_size = 52;
  float text_x = x + 34;
  float text_y = y + DECK_HEIGHT - 60;

  DrawText(al_map_rgb(255, 255, 255), font_size, text_x, text_y, ALLEGRO_ALIGN_CENTER, n_discard_cards_text);
}

void RenderHealthBar(const Hp* hp, float x_begin, float x_end, float y_begin,
  ALLEGRO_COLOR color) {
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

  // Make half of the bar with a darker color
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

  int font_size = 28;
  float center_x = (x_begin + x_end) / 2.0;
  float text_begin_y = y_begin + HEALTH_BAR_PADDING + (HEALTH_BAR_HEIGHT - font_size - 3) / 2.0;

  DrawText(al_map_rgb(0, 0, 0), font_size, center_x,
    text_begin_y, ALLEGRO_ALIGN_CENTER, text);
}

void RenderEnergy(Renderer* renderer, Game* game) {
  RenderImage(Energy_Img_Id, ENERGY_X, ENERGY_Y, ENERGY_WIDTH);

  char energyText[2];
  sprintf(energyText, "%d", game->player.energy);

  ALLEGRO_COLOR color = al_map_rgb(0, 0, 0);

  int font_size = 80;

  float x = ENERGY_X + (ENERGY_WIDTH / 2.0);
  float y = ENERGY_Y + (ENERGY_WIDTH - font_size) / 2.0 + 8;

  DrawText(color, 72, x, y, ALLEGRO_ALIGN_CENTER, energyText);
}

void RenderPhase(Renderer* renderer, Game* game) {
  char phase_text[30];
  sprintf(phase_text, "Fase %d", game->phase);

  DrawText(al_map_rgb(245, 208, 47), 48, renderer->display_width - 80, 10, ALLEGRO_ALIGN_CENTER, phase_text);
}

float RenderImage(Image_Id img_id, float x, float y, float width) {
  Image* image = GetImage(img_id);

  float img_w = al_get_bitmap_width(image->bitmap);
  float img_h = al_get_bitmap_height(image->bitmap);

  float scale = width / img_w;

  al_draw_scaled_bitmap(image->bitmap, 0, 0, img_w, img_h, x, y, img_w * scale, img_h * scale, 0);

  return scale;
}

void Render(Renderer* renderer, ALLEGRO_TIMER* timer, Game* game) {
  al_set_target_bitmap(renderer->display_buffer);


  RenderBackground(renderer, game);
  RenderPhase(renderer, game);

  if (game->turn == Player_Turn) {
    RenderBuyDeck(renderer, game);
    RenderDiscardDeck(renderer, game);
    RenderPlayerHand(renderer, game);
  }

  RenderEnergy(renderer, game);
  RenderPlayer(renderer, &game->player);
  RenderEnemies(renderer, game);

  if (message.is_to_show)
    DrawMessage(renderer);

  al_set_target_backbuffer(renderer->display);

  al_draw_bitmap(renderer->display_buffer, 0, 0, 0);

  al_flip_display();
}

void ClearRenderer(Renderer* renderer) {
  al_destroy_display(renderer->display);
  al_destroy_bitmap(renderer->display_buffer);

  for (int i = 0; i < fonts_size; i++) {
    al_destroy_font(fonts_loaded[i].ptr);
  }
}