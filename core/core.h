#ifndef _CORE_H_
#define _CORE_H_

#include "entities/player/player.h"
#include "entities/enemy/enemy.h"
#include "entities/card/card.h"

typedef struct Renderer Renderer;

typedef enum { Card_Element, Enemy_Element } Element_Type;

typedef struct FocusedElement {
    Element_Type type;
    int index;
} FocusedElement;


typedef struct Game {
    Player player;
    Enemy enemies[2];

    int phase;

    Card deck[20];
    Card hand[5];
    Card discard[20];

    FocusedElement focused_element;

    unsigned char keyboard_keys[ALLEGRO_KEY_MAX];
} Game;

void AdvanceGame(Renderer* renderer, Game* game);
Game InitGame();
#endif
