#include "renderer.h"
#include "constants.h"
#include "images/load-images.h"
#include "card.h"
#include "core/core.h"

#include <allegro5/allegro_primitives.h>
#include <stdio.h>

int focused_card_index = -1;

void RenderCard(const Renderer* renderer, const Card* card, int x_left, int y_top, int is_focused, int is_selected) {
    if (is_focused) y_top -= 40;

    Imgs_Ids card_img_id;

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
    }

    if (is_selected)
        al_draw_filled_rounded_rectangle(x_left + 3, y_top + 2, x_left + CARD_WIDTH + 3, y_top + CARD_HEIGHT + 4, 10, 10, al_map_rgb(255, 255, 255));

    float scale = RenderImage(card_img_id, x_left, y_top, CARD_WIDTH);

    char card_cost_text[10] = "";
    sprintf(card_cost_text, "%d", card->cost);

    DrawScaledText(renderer->font, al_map_rgb(255, 255, 255), x_left + 19,
        y_top + 9, 1 / scale, 1 / scale, ALLEGRO_ALIGN_CENTER, card_cost_text);

    char card_effect_text[30] = "";
    char attack_format[] = "%d\nAtaque";
    char defense_format[] = "%d\nDefesa";
    char special_format[] = "%d\nEfeito";

    char format[40];
    strcpy(format, (card->type == Card_Type_Attack) ? attack_format : (card->type == Card_Type_Defense ? defense_format : special_format));

    sprintf(card_effect_text, format, card->effect);

    DrawMultilineScaledText(renderer->font, al_map_rgb(255, 255, 255), x_left + 3 + CARD_WIDTH / 2.0,
        y_top + CARD_HEIGHT / 2.0 - 2 * DEFAULT_LINE_HEIGHT, CARD_WIDTH - 30, 1.7, 1.7, ALLEGRO_ALIGN_CENTER, card_effect_text);
}

void RenderPlayerHand(Renderer* renderer, Game* game) {
    float start_x = (DISPLAY_BUFFER_WIDTH / 2.0) - (CARD_X_OFFSET * (game->hand_size - 1) + CARD_WIDTH) / 2.0;
    float y = DISPLAY_BUFFER_HEIGHT - HAND_Y_DISTANCE_TO_BOTTOM - CARD_HEIGHT;

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
    else card.effect = 100;

    return card;
}