#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "renderer.h"
#include "utils.h"
#include "core/core.h"
#include "images/load-images.h"

int main() {
  srand(time(NULL));

  must_init(al_init(), "allegro");
  must_init(al_init_image_addon(), "images");
  must_init(al_init_primitives_addon(), "primitives");
  must_init(al_install_keyboard(), "keyboard");
  must_init(al_init_font_addon(), "init font");
  must_init(al_init_ttf_addon(), "init ttf font");

  LoadImages();

  ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
  must_init(timer, "timer");

  ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
  must_init(queue, "queue");

  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_timer_event_source(timer));

  ALLEGRO_EVENT event;

  Renderer renderer;
  FillRenderer(&renderer);
  al_register_event_source(queue,
    al_get_display_event_source(renderer.display));

  Game game = InitGame();
  ClearKeyboardKeys(game.keyboard_keys);

  al_start_timer(timer);
  while (1) {
    al_wait_for_event(queue, &event);
    int done = 0, print_combat = 0, redraw = 0;

    switch (event.type) {
    case ALLEGRO_EVENT_TIMER:
      redraw = 1;
      if (game.keyboard_keys[ALLEGRO_KEY_Q])
        done = 1;
      break;

    case ALLEGRO_EVENT_KEY_DOWN:
      game.keyboard_keys[event.keyboard.keycode] = GAME_KEY_DOWN;
      break;
    case ALLEGRO_EVENT_KEY_UP:
      game.keyboard_keys[event.keyboard.keycode] = 0;
      break;
    case ALLEGRO_EVENT_DISPLAY_CLOSE:
      done = 1;
      break;
    }

    if (done) {
      break;
    }
    // You want to put your combat logic here.
    if (redraw) {
      AdvanceGame(&renderer, &game);
      Render(&renderer, timer, &game);
      redraw = 0;
    }
  }
  al_destroy_timer(timer);
  al_destroy_event_queue(queue);
  DestroyImages();
  ClearRenderer(&renderer);

  StopAnimation(&game.player.action_animation);
  StopAnimation(&game.player.idle_animation);

  for (int i = 0; i < game.enemies_size; i++) {
    StopAnimation(&game.enemies[i].action_animation);
    StopAnimation(&game.enemies[i].idle_animation);
  }
  return 0;
}