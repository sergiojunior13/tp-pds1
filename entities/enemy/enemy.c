#include "renderer.h"
#include "constants.h"
#include "utils.h"
#include "enemy.h"
#include "images/load-images.h"
#include "core/core.h"

#include <stdio.h>

#include <allegro5/allegro_primitives.h>

void RemoveEnemyFromArray(Enemy array[], int* size_ptr, int element_index) {
    if ((*size_ptr) == 1) {
        (*size_ptr)--;
        memset(array, 0, 2 * sizeof(Enemy));
        return;
    }

    for (int i = element_index; i < (*size_ptr) - 1; i++) {
        array[i] = array[i + 1];
    }

    (*size_ptr)--;
}

void RenderEnemies(const Renderer* renderer, Game* game) {
    int begin_x = renderer->display_width - CREATURE_DISTANCE_TO_WINDOW_CORNER - ENEMY_RADIUS;

    int is_some_enemy_focused = game->focused_entity.type == Enemy_Entity;
    int focused_enemy_index = is_some_enemy_focused ? game->focused_entity.index : -1;

    for (int i = 0; i < game->enemies_size; i++) {
        RenderEnemy(renderer, &game->enemies[i], begin_x - 140 * (1 - i), ENEMY_BEGIN_Y, focused_enemy_index == i);
    }
}

void RenderEnemy(const Renderer* renderer, const Enemy* enemy, int begin_x, int begin_y, int is_focused) {
    // if (enemy->hp.crr == 0) return;

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

    RenderHealthBar(&enemy->hp, begin_x, x_end, health_bar_begin_y, health_bar_color);

    // Render defense pts
    if (enemy->shield_pts > 0) {
        float defense_y = health_bar_begin_y + HEALTH_BAR_HEIGHT + HEALTH_BAR_PADDING * 2 + 5;
        float defense_img_size = 28;
        RenderImage(Shield_Img_Id, begin_x, defense_y, defense_img_size);

        char defense_text[4];
        sprintf(defense_text, "%d", enemy->shield_pts);
        DrawText(al_map_rgb(255, 255, 255), 24, begin_x + defense_img_size / 2.0 + 3,
            defense_y + defense_img_size - 22, ALLEGRO_ALIGN_LEFT, defense_text);
    }


    // Render next actions
    float img_size = 24;
    float img_gap = 12;
    for (int i = 0; i < enemy->actions_size; i++) {
        EnemyAction action = enemy->actions[i];

        int img_id = action.type == Attack_Action ? Sword_Img_Id : Shield_Img_Id;

        float img_x = center_x - ((img_size * enemy->actions_size + (enemy->actions_size - 1) * img_gap) / 2.0)
            + img_size * i + img_gap * i;
        float img_y = begin_y - 10 - img_size;
        RenderImage(img_id, img_x, img_y, img_size);

        char effect_text[4];
        sprintf(effect_text, "+%d", action.effect);
        DrawText(al_map_rgb(255, 255, 255), 20, img_x + img_size + 7,
            img_y + img_size - 10, ALLEGRO_ALIGN_RIGHT, effect_text);
    }

}

void GenerateEnemyActions(Enemy* enemy) {
    if (enemy->type == Enemy_Strong)
        enemy->actions_size = GenRandomNum(2, 3);
    else enemy->actions_size = GenRandomNum(1, 2);

    int already_has_action_of_lvl1 = 0;

    for (int i = 0; i < enemy->actions_size; i++) {
        enemy->actions[i].type = i == 0 ? Attack_Action : (Attack_Action, Defense_Action);

        if (enemy->type == Enemy_Strong) {
            enemy->actions[i].level = GenRandomNum(already_has_action_of_lvl1 ? 2 : 1, 3);
            already_has_action_of_lvl1 = enemy->actions[i].level == 1;
        }
        else enemy->actions[i].level = GenRandomNum(0, 1);

        switch (enemy->actions[i].level) {
        case 0:
            enemy->actions[i].effect = GenRandomNum(1, 5);
            break;
        case 1:
            enemy->actions[i].effect = GenRandomNum(5, 10);
            break;
        case 2:
            enemy->actions[i].effect = GenRandomNum(10, 15);
            break;
        case 3:
            enemy->actions[i].effect = GenRandomNum(15, 30);
            break;
        default:
            break;
        }
    }
}

void GenerateEnemies(Enemy gameEnemies[2]) {
    int already_gen_strong_enemy = 0;

    for (int i = 0; i < 2; i++) {
        // Enemy type
        if (!already_gen_strong_enemy) { // Chance of getting '1' is 5%, the chance of appear a strong enemy
            gameEnemies[i].type = GenRandomNum(1, 20) == 1 ? Enemy_Strong : Enemy_Weak;
            already_gen_strong_enemy = 1;
        }
        else gameEnemies[i].type = Enemy_Weak;

        // Enemy HP
        gameEnemies[i].hp.max = gameEnemies[i].hp.crr =
            gameEnemies[i].type == Enemy_Strong ? GenRandomNum(40, 100) : GenRandomNum(10, 30);

        // Enemy defense pts
        gameEnemies[i].shield_pts = 0;

        // Enemy actions
        GenerateEnemyActions(&gameEnemies[i]);
    }
}