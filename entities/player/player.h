#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "utils.h"
#include "images/animation.h"

typedef struct Renderer Renderer;

typedef struct Player {
    Hp hp;
    int energy;
    int shield_pts;

    Animation idle_animation;
    Animation action_animation; // Attack, defense or special
} Player;

void StartPlayerAttackAnimation(Player* player);
void StartPlayerDefenseAnimation(Player* player);
void StartPlayerSpecialAnimation(Player* player);

void RenderPlayer(const Renderer* renderer, Player* player);
#endif
