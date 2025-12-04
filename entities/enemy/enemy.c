#include "renderer.h"
#include "constants.h"
#include "utils.h"
#include "enemy.h"
#include "images/load-images.h"
#include "images/animation.h"
#include "core/core.h"

#include <stdio.h>

#include <allegro5/allegro_primitives.h>

int enemy_ani_size = ENEMY_SIZE + 200;

void InitEnemyIdleAnimation(Enemy* enemy, int enemy_index) {
    enemy->idle_animation.fps = 10;
    enemy->idle_animation.width = enemy_ani_size;
    enemy->idle_animation.height = enemy_ani_size;
    enemy->idle_animation.loop = 1;
    enemy->idle_animation.animation_to_start_after_end = NULL;

    enemy->idle_animation.sprite_sheet.frames_size = 8;
    enemy->idle_animation.sprite_sheet.frame_width = 128;
    enemy->idle_animation.sprite_sheet.frame_height = 128;
    enemy->idle_animation.sprite_sheet.image =
        GetImage((enemy->type == Enemy_Strong || enemy->type == Enemy_Boss) ? Strong_Enemy_Idle_Img_Id : Weak_Enemy_Idle_Img_Id);

    InitAnimation(&enemy->idle_animation);
}

void StartEnemyDeadAnimation(Enemy* enemy, int enemy_index) {
    StopAnimation(&enemy->idle_animation);

    enemy->action_animation.fps = 4;
    enemy->action_animation.width = enemy_ani_size;
    enemy->action_animation.height = enemy_ani_size;
    enemy->action_animation.loop = 0;
    enemy->action_animation.animation_to_start_after_end = NULL;

    enemy->action_animation.sprite_sheet.frames_size = 3;
    enemy->action_animation.sprite_sheet.frame_width = 128;
    enemy->action_animation.sprite_sheet.frame_height = 128;
    enemy->action_animation.sprite_sheet.image =
        GetImage((enemy->type == Enemy_Strong || enemy->type == Enemy_Boss) ? Strong_Enemy_Dead_Img_Id : Weak_Enemy_Dead_Img_Id);

    InitAnimation(&enemy->action_animation);
}

void StartEnemyAttackAnimation(Enemy* enemy, int enemy_index) {
    StopAnimation(&enemy->idle_animation);

    enemy->action_animation.fps = 6;
    enemy->action_animation.width = enemy_ani_size;
    enemy->action_animation.height = enemy_ani_size;
    enemy->action_animation.invert_horizontally = 1;
    enemy->action_animation.loop = 0;
    enemy->action_animation.animation_to_start_after_end = &enemy->idle_animation;

    enemy->action_animation.sprite_sheet.frames_size = (enemy->type == Enemy_Strong || enemy->type == Enemy_Boss) ? 5 : 4;
    enemy->action_animation.sprite_sheet.frame_width = 128;
    enemy->action_animation.sprite_sheet.frame_height = 128;
    enemy->action_animation.sprite_sheet.image =
        GetImage((enemy->type == Enemy_Strong || enemy->type == Enemy_Boss) ? Strong_Enemy_Attack_Img_Id : Weak_Enemy_Attack_Img_Id);

    InitAnimation(&enemy->action_animation);
}

void StartEnemyHurtAnimation(Enemy* enemy, int enemy_index) {
    StopAnimation(&enemy->idle_animation);

    enemy->action_animation.fps = 8;
    enemy->action_animation.width = enemy_ani_size;
    enemy->action_animation.height = enemy_ani_size;
    enemy->action_animation.invert_horizontally = 1;
    enemy->action_animation.loop = 0;
    enemy->action_animation.animation_to_start_after_end = &enemy->idle_animation;

    enemy->action_animation.sprite_sheet.frames_size = 6;
    enemy->action_animation.sprite_sheet.frame_width = 128;
    enemy->action_animation.sprite_sheet.frame_height = 128;
    enemy->action_animation.sprite_sheet.image =
        GetImage((enemy->type == Enemy_Strong || enemy->type == Enemy_Boss) ? Strong_Enemy_Hurt_Img_Id : Weak_Enemy_Hurt_Img_Id);

    InitAnimation(&enemy->action_animation);
}

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

    int begin_x = renderer->display_width - CREATURE_DISTANCE_TO_WINDOW_CORNER - ENEMY_SIZE;

    int is_some_enemy_focused = game->focused_entity.type == Enemy_Entity;
    int focused_enemy_index = is_some_enemy_focused ? game->focused_entity.index : -1;

    if (game->phase == 11) {
        RenderBoss(renderer, &game->enemies[0], begin_x, ENEMY_BEGIN_Y, focused_enemy_index == 0);
        return;
    }

    enemy_ani_size = ENEMY_SIZE + 200;

    for (int i = 0; i < game->enemies_size; i++) {
        RenderEnemy(renderer, &game->enemies[i], begin_x - (ENEMY_SIZE + 40) * (1 - i), ENEMY_BEGIN_Y + i * 100, focused_enemy_index == i, i);
    }
}

void RenderEnemy(const Renderer* renderer, Enemy* enemy, int begin_x, int begin_y, int is_focused, int index) {
    if (is_focused) begin_y -= 40;

    enemy->idle_animation.x = begin_x - (enemy_ani_size - ENEMY_SIZE) / 2.0;
    enemy->idle_animation.y = begin_y - (enemy_ani_size - ENEMY_SIZE);
    enemy->action_animation.x = begin_x - (enemy_ani_size - ENEMY_SIZE) / 2.0;
    enemy->action_animation.y = begin_y - (enemy_ani_size - ENEMY_SIZE);

    if (enemy->idle_animation.started != 1)
        InitEnemyIdleAnimation(enemy, index);

    RenderAnimation(&enemy->action_animation);
    RenderAnimation(&enemy->idle_animation);

    int width = ENEMY_SIZE;
    float center_x = begin_x + width / 2.0;
    float center_y = begin_y + width / 2.0;

    ALLEGRO_COLOR health_bar_color = enemy->type == Enemy_Strong ? al_map_rgb(211, 175, 55) : al_map_rgb(0, 255, 0);

    float x_end = begin_x + width;

    float health_bar_begin_y = begin_y + width + 10;

    RenderHealthBar(&enemy->hp, begin_x, x_end, health_bar_begin_y, health_bar_color);

    // Render defense pts
    if (enemy->shield_pts > 0) {
        float defense_y = health_bar_begin_y + HEALTH_BAR_HEIGHT + HEALTH_BAR_PADDING * 2 + 5;
        float defense_img_size = 28;
        RenderImage(Shield_Img_Id, begin_x, defense_y, defense_img_size);

        char shield_text[4];
        sprintf(shield_text, "%d", enemy->shield_pts);

        float x = begin_x + 20;
        float y = health_bar_begin_y + HEALTH_BAR_HEIGHT + 26;
        DrawText(al_map_rgb(255, 255, 255), 24, x, y, 0, shield_text);
    }


    // Render next actions
    float img_size = 24;
    float img_gap = 12;
    for (int i = 0; i < enemy->actions_size; i++) {
        EnemyAction action = enemy->actions[i];

        int img_id = action.type == Attack_Action ? Sword_Img_Id : Shield_Img_Id;

        float img_x = center_x - ((img_size * enemy->actions_size + (enemy->actions_size - 1) * img_gap) / 2.0)
            + img_size * i + img_gap * i;
        float img_y = begin_y + 15 - img_size;

        RenderImage(img_id, img_x, img_y, img_size);

        char effect_text[4];
        sprintf(effect_text, "+%d", action.effect);
        DrawText(al_map_rgb(255, 255, 255), 20, img_x + img_size + 7,
            img_y + img_size - 10, ALLEGRO_ALIGN_RIGHT, effect_text);
    }

}

void RenderBoss(const Renderer* renderer, Enemy* boss, int begin_x, int begin_y, int is_focused) {
    boss->type = Enemy_Strong; // The boss and strong enemy render process are basically the same
    RenderEnemy(renderer, boss, begin_x, begin_y + 100, is_focused, 0);
    boss->type = Enemy_Boss;
}


void GenerateEnemyActions(Enemy* enemy) {
    if (enemy->type == Enemy_Boss) {
        enemy->actions_size = 3;
        for (int i = 0; i < enemy->actions_size; i++) {
            if (i == 0) // At least 1 attack action
                enemy->actions[i].type = Attack_Action;
            else
                enemy->actions[i].type = GenRandomNum(Attack_Action, Defense_Action);

            enemy->actions[i].effect = GenRandomNum(10, 20);
        }

        return;
    }

    if (enemy->type == Enemy_Strong)
        enemy->actions_size = GenRandomNum(2, 3);
    else enemy->actions_size = GenRandomNum(1, 2);

    int already_has_action_of_lvl1 = 0;

    for (int i = 0; i < enemy->actions_size; i++) {
        if (i == 0) // At least 1 attack action
            enemy->actions[i].type = Attack_Action;
        else
            enemy->actions[i].type = GenRandomNum(Attack_Action, Defense_Action);

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

void GenerateEnemies(Enemy enemies[2]) {
    int already_gen_strong_enemy = 0;

    for (int i = 0; i < 2; i++) {
        // Enemy type
        if (!already_gen_strong_enemy) {
            // Chance of getting '1' of 20 is 5%, the chance to appear a strong enemy
            enemies[i].type = GenRandomNum(1, 20) == 1 ? Enemy_Strong : Enemy_Weak;

            if (enemies[i].type == Enemy_Strong)
                already_gen_strong_enemy = 1;
        }
        else enemies[i].type = Enemy_Weak;

        // Enemy HP
        enemies[i].hp.max = enemies[i].hp.crr =
            enemies[i].type == Enemy_Strong ? GenRandomNum(40, 100) : GenRandomNum(10, 30);

        // Enemy defense pts
        enemies[i].shield_pts = 0;

        // Enemy actions
        GenerateEnemyActions(&enemies[i]);
    }
}

void GenerateBoss(Enemy enemies[2]) {
    Enemy* boss = &enemies[0];

    boss->type = Enemy_Boss;
    boss->hp.max = 150;
    boss->hp.crr = 150;

    GenerateEnemyActions(boss);

    enemy_ani_size += 150; // Make the boss bigger
}