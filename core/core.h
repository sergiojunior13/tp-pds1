#ifndef _CORE_H_
#define _CORE_H_

#include "entities/player/player.h"
#include "entities/enemy/enemy.h"
#include "entities/card/card.h"

typedef struct Renderer Renderer;

typedef enum EntityType { Card_Entity, Enemy_Entity } EntityType;

typedef enum GameTurn { Player_Turn, Enemy_Turn } GameTurn;

typedef struct Entity {
    EntityType type;
    int index;
} Entity;

typedef struct Game {
    int phase;
    GameTurn turn;

    Player player;
    Enemy enemies[2];

    int enemies_size;

    Card deck[20];
    Card hand[5];
    Card discard[20];
    Card buy[20];

    int buy_size;
    int discard_size;
    int hand_size;

    int selected_card_index;
    int selected_enemy_index;

    Entity focused_entity;

    unsigned char keyboard_keys[ALLEGRO_KEY_MAX];
} Game;

void AdvanceGame(Renderer* renderer, Game* game);
Game InitGame();
#endif
