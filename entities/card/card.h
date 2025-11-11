#ifndef _CARD_H_
#define _CARD_H_

typedef struct Renderer Renderer;
typedef struct Game Game;

typedef enum CardType {
    Card_Type_Attack,
    Card_Type_Defense,
    Card_Type_Special,
} CardType;

typedef struct Card {
    CardType type;
    int cost;
    int effect;
} Card;

void RenderCard(const Renderer* renderer, const Card* card, int x, int y, int is_focused, int is_selected);

void RenderPlayerHand(Renderer* renderer, Game* game);

Card GenerateCard(CardType type, int cost);

#endif
