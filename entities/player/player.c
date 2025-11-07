#include "renderer.h"
#include "constants.h"
#include "player.h"

#include <allegro5/allegro_primitives.h>


void RenderPlayer(const Renderer* renderer, const Player* player) {
    int begin_x = CREATURE_DISTANCE_TO_WINDOW_CORNER;
    int begin_y = PLAYER_BEGIN_Y;
    int width = PLAYER_RADIUS;


    al_draw_filled_circle(begin_x + width / 2.0, begin_y + width / 2.0, width / 2.0,
        al_map_rgb(0, 255, 255));
    float x_end = begin_x + width;

    float health_bar_begin_y = begin_y + width + 10;

    Hp hp = { 80, 100 };

    RenderHealthBar(&player->hp, begin_x, x_end, health_bar_begin_y, renderer->font);
}