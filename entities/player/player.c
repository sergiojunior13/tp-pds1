#include "renderer.h"
#include "constants.h"
#include "images/load-images.h"
#include "player.h"
#include <stdio.h>

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

    RenderHealthBar(&player->hp, begin_x, x_end, health_bar_begin_y, renderer->font, al_map_rgb(0, 255, 0));

    if (player->shield_pts > 0) {
        RenderImage(Shield_Img_Id, begin_x, health_bar_begin_y + HEALTH_BAR_HEIGHT + 10, 25);

        char shieldTxt[2];
        sprintf(shieldTxt, "%d", player->shield_pts);

        float scale = 1.8;
        float x = begin_x + 15;
        float y = health_bar_begin_y + HEALTH_BAR_HEIGHT + 25;
        DrawScaledText(renderer->font, al_map_rgb(255, 255, 255),
            x, y, scale, scale, 0, shieldTxt);
    }
}