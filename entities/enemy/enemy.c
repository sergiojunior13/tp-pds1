#include "renderer.h"
#include "constants.h"
#include "utils.h"
#include "enemy.h"
#include "images/load-images.h"
#include "core/core.h"

#include <allegro5/allegro_primitives.h>


void RenderEnemies(const Renderer* renderer, Game* game) {
    int begin_x = DISPLAY_BUFFER_WIDTH - CREATURE_DISTANCE_TO_WINDOW_CORNER - ENEMY_RADIUS;

    int is_some_enemy_focused = game->focused_entity.type == Enemy_Entity;
    int focused_enemy_index = is_some_enemy_focused ? game->focused_entity.index : -1;

    for (int i = 0; i < game->enemies_size; i++) {
        RenderEnemy(renderer, &game->enemies[i], begin_x - 140 * (1 - i), ENEMY_BEGIN_Y, focused_enemy_index == i);
    }
}

void RenderEnemy(const Renderer* renderer, const Enemy* enemy, int begin_x, int begin_y, int is_focused) {
    if (enemy->hp.crr == 0) return;

    if (is_focused) begin_y -= 40;

    int width = ENEMY_RADIUS;
    float center_x = begin_x + width / 2.0;
    float center_y = begin_y + width / 2.0;

    ALLEGRO_COLOR enemy_color = enemy->type == Enemy_Strong ? al_map_rgb(211, 175, 55) : al_map_rgb(255, 0, 0);
    ALLEGRO_COLOR health_bar_color = enemy->type == Enemy_Strong ? al_map_rgb(211, 175, 55) : al_map_rgb(0, 255, 0);

    al_draw_filled_circle(center_x, center_y, width / 2.0,
        enemy_color);
    float x_end = begin_x + width;

    float health_bar_begin_y = begin_y + width + 10;

    RenderHealthBar(&enemy->hp, begin_x, x_end, health_bar_begin_y, renderer->font, health_bar_color);

    float img_y = health_bar_begin_y + HEALTH_BAR_HEIGHT + HEALTH_BAR_PADDING * 2 + 5;
    RenderImage(Shield_Img_Id, center_x, img_y, 32);
    DrawScaledText(renderer->font, al_map_rgb(255, 255, 255), center_x - 15, img_y + 7, 2.0, 2.0, ALLEGRO_ALIGN_LEFT, "4");
}

void GenerateEnemies(Enemy gameEnemies[2]) {
    int already_gen_strong_enemy = 0;

    for (int i = 0; i < 2; i++) {

        if (!already_gen_strong_enemy) { // Chance of getting '1' is 5%, the chance of appear a strong enemy
            gameEnemies[i].type = GenRandomNum(1, 20) == 1 ? Enemy_Strong : Enemy_Weak;
            already_gen_strong_enemy = 1;
        }
        else gameEnemies[i].type = Enemy_Weak;

        gameEnemies[i].hp.max = gameEnemies[i].hp.crr =
            gameEnemies[i].type == Enemy_Strong ? GenRandomNum(40, 100) : GenRandomNum(10, 30);
    }
}