#include "renderer.h"
#include "constants.h"
#include "images/load-images.h"
#include "card.h"
#include "core/core.h"

#include <allegro5/allegro_primitives.h>
#include <stdio.h>

int focused_card_index = -1;

void RenderCard(const Renderer* renderer, const Card* card, int x_left, int y_top, int is_focused) {
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

    float scale = 0.5;

    RenderImage(card_img_id, x_left, y_top, scale);


    char card_cost_text[10] = "";
    sprintf(card_cost_text, "%d", card->cost);

    DrawScaledText(renderer->font, al_map_rgb(255, 255, 255), x_left + 19,
        y_top + 9, 1 / scale, 1 / scale,
        ALLEGRO_ALIGN_CENTER, card_cost_text);

    char card_effect_text[30] = "";
    char attack_format[] = "%d Ataque\n\nDê %d de dano no seu oponente";
    char defense_format[] = "%d Defesa\n\nReceba %d de escudo";
    char special_format[] = "%d Efeito";

    char format[40];
    strcpy(format, (card->type == Card_Type_Attack) ? attack_format : (card->type == Card_Type_Defense ? defense_format : special_format));

    sprintf(card_effect_text, format, card->effect, card->effect);

    al_draw_multiline_text(renderer->font, al_map_rgb(255, 255, 255), x_left + 25, y_top + 45, CARD_WIDTH - 60, DEFAULT_LINE_HEIGHT, 0, card_effect_text);
}

void RenderPlayerHand(Renderer* renderer, Game* game) {
    float start_x = (DISPLAY_BUFFER_WIDTH / 2) - (CARD_X_OFFSET * 4 + CARD_WIDTH) / 2;
    float y = DISPLAY_BUFFER_HEIGHT - HAND_Y_DISTANCE_TO_BOTTOM - CARD_HEIGHT;

    int is_some_card_focused = game->focused_element.type == Card_Element;
    int focused_card_index = is_some_card_focused ? game->focused_element.index : -1;

    for (int i = 0; i < 5; i++) {
        RenderCard(renderer, &game->hand[i], start_x + i * CARD_X_OFFSET, y, focused_card_index == i);
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