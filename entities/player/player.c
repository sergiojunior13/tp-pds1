#include "renderer.h"
#include "constants.h"
#include "images/load-images.h"
#include "player.h"
#include "images/animation.h"

#include <stdio.h>
#include <allegro5/allegro_primitives.h>

int started_player_animation = 0;
int x, y, size;

void StartPlayerIdleAnimation() {
    started_player_animation = 1;

    Animation animation;
    animation.id = Player_Idle_Ani_Id;

    animation.fps = 10;
    animation.x = x;
    animation.y = y;
    animation.width = PLAYER_SIZE;
    animation.height = PLAYER_SIZE;
    animation.loop = 1;
    animation.OnFinish = NULL;

    animation.sprite_sheet.frames_size = 8;
    animation.sprite_sheet.frame_width = 100;
    animation.sprite_sheet.frame_height = 100;
    animation.sprite_sheet.image = GetImage(Player_Idle_Img_Id);

    StartAnimation(animation);
}

void StartPlayerAttackAnimation() {
    StopAnimation(Player_Idle_Ani_Id);

    Animation animation;
    animation.id = Player_Attack_Ani_Id;

    animation.fps = 13;
    animation.x = x;
    animation.y = y;
    animation.width = PLAYER_SIZE * 180.0 / 100.0;
    animation.height = PLAYER_SIZE;
    animation.loop = 0;
    animation.OnFinish = StartPlayerIdleAnimation;

    animation.sprite_sheet.frames_size = 17;
    animation.sprite_sheet.frame_width = 180;
    animation.sprite_sheet.frame_height = 100;
    animation.sprite_sheet.image = GetImage(Player_Attack_Img_Id);

    StartAnimation(animation);
}

void StartPlayerDefenseAnimation() {
    Animation animation;
    animation.id = Player_Defense_Ani_Id;

    animation.fps = 11;
    animation.x = x;
    animation.y = y - PLAYER_SIZE * 80.0 / 100.0;
    animation.width = PLAYER_SIZE;
    animation.height = PLAYER_SIZE * 180.0 / 100.0;
    animation.loop = 0;
    animation.OnFinish = NULL;

    animation.sprite_sheet.frames_size = 9;
    animation.sprite_sheet.frame_width = 100;
    animation.sprite_sheet.frame_height = 180;
    animation.sprite_sheet.image = GetImage(Player_Defense_Img_Id);

    StartAnimation(animation);
}

void StartPlayerSpecialAnimation() {
    StopAnimation(Player_Idle_Ani_Id);

    Animation animation;
    animation.id = Player_Special_Ani_Id;

    animation.fps = 14;
    animation.x = x;
    animation.y = y - PLAYER_SIZE * 80.0 / 100.0;
    animation.width = PLAYER_SIZE;
    animation.height = PLAYER_SIZE * 180.0 / 100.0;
    animation.loop = 0;
    animation.OnFinish = StartPlayerIdleAnimation;

    animation.sprite_sheet.frames_size = 19;
    animation.sprite_sheet.frame_width = 100;
    animation.sprite_sheet.frame_height = 180;
    animation.sprite_sheet.image = GetImage(Player_Special_Img_Id);

    StartAnimation(animation);
}

void RenderPlayer(const Renderer* renderer, const Player* player) {
    int begin_x = x = CREATURE_DISTANCE_TO_WINDOW_CORNER,
        begin_y = y = PLAYER_BEGIN_Y,
        player_size = size = PLAYER_SIZE;

    if (!started_player_animation)
        StartPlayerIdleAnimation();

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