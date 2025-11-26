#include "renderer.h"
#include "constants.h"
#include "images/load-images.h"
#include "player.h"
#include "images/animation.h"

#include <stdio.h>
#include <allegro5/allegro_primitives.h>

int x, y, size;

void InitPlayerIdleAnimation(Player* player) {
    player->idle_animation.fps = 10;
    player->idle_animation.x = x;
    player->idle_animation.y = y;
    player->idle_animation.width = PLAYER_SIZE;
    player->idle_animation.height = PLAYER_SIZE;
    player->idle_animation.loop = 1;
    player->idle_animation.animation_to_start_after_end = NULL;

    player->idle_animation.sprite_sheet.frames_size = 8;
    player->idle_animation.sprite_sheet.frame_width = 100;
    player->idle_animation.sprite_sheet.frame_height = 100;
    player->idle_animation.sprite_sheet.image = GetImage(Player_Idle_Img_Id);

    InitAnimation(&player->idle_animation);
}

void StartPlayerAttackAnimation(Player* player) {
    StopAnimation(&player->idle_animation);

    player->action_animation.fps = 13;
    player->action_animation.x = x;
    player->action_animation.y = y;
    player->action_animation.width = PLAYER_SIZE * 180.0 / 100.0;
    player->action_animation.height = PLAYER_SIZE;
    player->action_animation.loop = 0;
    player->action_animation.animation_to_start_after_end = &player->idle_animation;

    player->action_animation.sprite_sheet.frames_size = 17;
    player->action_animation.sprite_sheet.frame_width = 180;
    player->action_animation.sprite_sheet.frame_height = 100;
    player->action_animation.sprite_sheet.image = GetImage(Player_Attack_Img_Id);

    InitAnimation(&player->action_animation);
}

void StartPlayerDefenseAnimation(Player* player) {
    player->action_animation.fps = 9;
    player->action_animation.x = x;
    player->action_animation.y = y - PLAYER_SIZE * 80.0 / 100.0;
    player->action_animation.width = PLAYER_SIZE;
    player->action_animation.height = PLAYER_SIZE * 180.0 / 100.0;
    player->action_animation.loop = 0;
    player->action_animation.animation_to_start_after_end = NULL;

    player->action_animation.sprite_sheet.frames_size = 9;
    player->action_animation.sprite_sheet.frame_width = 100;
    player->action_animation.sprite_sheet.frame_height = 180;
    player->action_animation.sprite_sheet.image = GetImage(Player_Defense_Img_Id);

    InitAnimation(&player->action_animation);
}

void StartPlayerSpecialAnimation(Player* player) {
    StopAnimation(&player->idle_animation);

    player->action_animation.fps = 14;
    player->action_animation.x = x;
    player->action_animation.y = y - PLAYER_SIZE * 80.0 / 100.0;
    player->action_animation.width = PLAYER_SIZE;
    player->action_animation.height = PLAYER_SIZE * 180.0 / 100.0;
    player->action_animation.loop = 0;
    player->action_animation.animation_to_start_after_end = &player->idle_animation;

    player->action_animation.sprite_sheet.frames_size = 19;
    player->action_animation.sprite_sheet.frame_width = 100;
    player->action_animation.sprite_sheet.frame_height = 180;
    player->action_animation.sprite_sheet.image = GetImage(Player_Special_Img_Id);

    InitAnimation(&player->action_animation);
}

void RenderPlayer(const Renderer* renderer, Player* player) {
    int begin_x = x = CREATURE_DISTANCE_TO_WINDOW_CORNER,
        begin_y = y = PLAYER_BEGIN_Y,
        player_size = size = PLAYER_SIZE;

    if (player->idle_animation.started != 1)
        InitPlayerIdleAnimation(player);

    RenderAnimation(&player->idle_animation);
    RenderAnimation(&player->action_animation);

    float health_bar_begin_y = begin_y + player_size + 10;

    RenderHealthBar(&player->hp, begin_x, begin_x + player_size, health_bar_begin_y, al_map_rgb(0, 255, 0));

    if (player->shield_pts > 0) {
        RenderImage(Shield_Img_Id, begin_x, health_bar_begin_y + HEALTH_BAR_HEIGHT + 10, 25);

        char shield_text[2];
        sprintf(shield_text, "%d", player->shield_pts);

        float x = begin_x + 20;
        float y = health_bar_begin_y + HEALTH_BAR_HEIGHT + 26;
        DrawText(al_map_rgb(255, 255, 255), 24, x, y, 0, shield_text);
    }
}