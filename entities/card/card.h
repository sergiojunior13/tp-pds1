#ifndef _CARD_H_
#define _CARD_H_

typedef struct Renderer Renderer;
typedef struct Game Game;

typedef enum CardType {
    Card_Type_Attack,
    Card_Type_Defense,
    Card_Type_Special,
} CardType;

typedef enum SpecialCardEffectType {
    Special_Card_Heal_Hp,
    Special_Card_Reduce_Enemies_Life_By_Half,
} SpecialCardEffectType;

typedef struct Card {
    CardType type;
    int cost;
    int effect;
    SpecialCardEffectType effect_type; // Prop just for the special card
} Card;

void RemoveCardFromArray(Card array[], int* size_ptr, int element_index);

void AddCardToArray(Card array[], int* arr_size_ptr, Card card);

void ClearCardArray(Card array[], int* arr_size_ptr);

void RenderCard(const Renderer* renderer, const Card* card, int x, int y, int is_focused, int is_selected);

void RenderPlayerHand(Renderer* renderer, Game* game);

Card GenerateCard(CardType type, int cost);

#endif
