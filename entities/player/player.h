#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "utils.h"

typedef struct Renderer Renderer;

typedef struct Player {
    Hp hp;
    int energy;
} Player;

void RenderPlayer(const Renderer* renderer, const Player* player);
#endif
