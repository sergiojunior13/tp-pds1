#include "renderer.h"
#include "constants.h"
#include "images/load-images.h"
#include "card.h"
#include "core/core.h"

#include <allegro5/allegro_primitives.h>
#include <stdio.h>

int focused_card_index = -1;

void RemoveCardFromArray(Card array[], int* size_ptr, int element_index) {
    for (int i = element_index; i < (*size_ptr) - 1; i++) {
        array[i] = array[i + 1];
    }

    (*size_ptr)--;
}

void AddCardToArray(Card array[], int* arr_size_ptr, Card card) {
    array[*arr_size_ptr] = card;

    (*arr_size_ptr)++;
}

void ClearCardArray(Card array[], int* arr_size_ptr) {
    memset(array, 0, sizeof(Card) * (*arr_size_ptr));

    (*arr_size_ptr) = 0;
}

void RenderCard(const Renderer* renderer, const Card* card, int x_left, int y_top, int is_focused, int is_selected) {
    if (is_focused) y_top -= 40;

    Image_Id card_img_id;

    switch (card->type) {
    case Card_Type_Attack:
        card_img_id = Card_Atk_Img_Id;
        break;
    case Card_Type_Defense:
        card_img_id = Card_Def_Img_Id;
        break;
    case Card_Type_Special:
        card_img_id = Card_Spe_Img_Id;
        break;
    default:
        return;
        break;
    }

    if (is_selected)
        al_draw_filled_rounded_rectangle(x_left + 3, y_top + 2, x_left + CARD_WIDTH + 3, y_top + CARD_HEIGHT + 4, 10, 10, al_map_rgb(255, 255, 255));

    float scale = RenderImage(card_img_id, x_left, y_top, CARD_WIDTH);

    char card_cost_text[10] = "";
    sprintf(card_cost_text, "%d", card->cost);

    DrawText(al_map_rgb(255, 255, 255), 32, x_left + 18,
        y_top - 1, ALLEGRO_ALIGN_CENTER, card_cost_text);

    char card_effect_text[30] = "";
    char attack_format[] = "%d\nAtaque";
    char defense_format[] = "%d\nDefesa";
    char special_hp_format[] = "+%d\nVida";
    char special_enemies_format[] = "Remova metade da vida dos inimigos";

    char format[100];
    int font_size = 40;

    switch (card->type) {
    case Card_Type_Attack:
        strcpy(format, attack_format);
        break;
    case Card_Type_Defense:
        strcpy(format, defense_format);
        break;
    case Card_Type_Special:
        if (card->effect_type == Special_Card_Heal_Hp)
            strcpy(format, special_hp_format);
        else {
            strcpy(format, special_enemies_format);
            font_size = 32;
        }
        break;
    default:
        break;
    }

    sprintf(card_effect_text, format, card->effect);
    float x = x_left + 3 + CARD_WIDTH / 2.0;
    float y = y_top + CARD_HEIGHT / 2.0 - (font_size == 40 ? 1.5 : 2.5) * font_size * DEFAULT_LINE_HEIGHT - 5;

    DrawMultilineText(al_map_rgb(255, 255, 255), font_size, x,
        y, CARD_WIDTH - 40, ALLEGRO_ALIGN_CENTER, card_effect_text);
}

void RenderPlayerHand(Renderer* renderer, Game* game) {
    float start_x = (renderer->display_width / 2.0) - (CARD_X_OFFSET * (game->hand_size - 1) + CARD_WIDTH) / 2.0;
    float y = renderer->display_height - HAND_Y_DISTANCE_TO_BOTTOM - CARD_HEIGHT;

    int is_some_card_focused = game->focused_entity.type == Card_Entity;
    int focused_card_index = is_some_card_focused ? game->focused_entity.index : -1;

    for (int i = 0; i < game->hand_size; i++) {
        RenderCard(renderer, &game->hand[i], start_x + i * CARD_X_OFFSET, y, focused_card_index == i, game->selected_card_index == i);
    }
}

Card GenerateCard(CardType type, int cost) {
    Card card;
    card.type = type;
    card.cost = cost;

    if (type != Card_Type_Special) {
        card.effect_type = -1;

        switch (cost) {
        case 0:
            card.effect = GenRandomNum(1, 5);
            break;
        case 1:
            card.effect = GenRandomNum(5, 10);
            break;
        case 2:
            card.effect = GenRandomNum(10, 15);
            break;
        case 3:
            card.effect = GenRandomNum(15, 30);
            break;
        }
    }
    else {
        card.effect_type = GenRandomNum(Special_Card_Heal_Hp, Special_Card_Reduce_Enemies_Life_By_Half);

        switch (card.effect_type) {
        case Special_Card_Heal_Hp:
            card.effect = GenRandomNum(10, 20);
            break;
        case Special_Card_Reduce_Enemies_Life_By_Half:
            card.effect = 2;
            break;
        }
    }

    return card;
}