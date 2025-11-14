#include "renderer.h"
#include "core.h"
#include "entities/enemy/enemy.h"
#include "entities/card/card.h"
#include "constants.h"
#include <stdio.h>

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

void GenerateDeck(Card game_deck[]) {
    // Attack cards
    for (int i = 0; i < 10; i++) {
        Card card;
        if (i == 0) card = GenerateCard(Card_Type_Attack, 0); // At least 1 card of cost 0
        else if (i == 1) card = GenerateCard(Card_Type_Attack, 2); // At least 1 card of cost 2
        else if (i == 2) card = GenerateCard(Card_Type_Attack, 3); // At least 1 card of cost 3
        else if (i <= 5) card = GenerateCard(Card_Type_Attack, 1); // At least 3 cards of cost 1
        else card = GenerateCard(Card_Type_Attack, GenRandomNum(0, 3)); // Gen the last 4 atk cards randomly

        game_deck[i] = card;
    }

    // Defense cards
    for (int i = 10; i < 18; i++) {
        Card card;
        if (i == 10) card = GenerateCard(Card_Type_Defense, 0); // At least 1 card of cost 0
        else if (i == 11) card = GenerateCard(Card_Type_Defense, 2); // At least 1 card of cost 2
        else if (i == 12) card = GenerateCard(Card_Type_Defense, 3); // At least 1 card of cost 3
        else if (i <= 15) card = GenerateCard(Card_Type_Defense, 1); // At least 3 cards of cost 1
        else card = GenerateCard(Card_Type_Defense, GenRandomNum(0, 3)); // Gen the last 4 def cards randomly

        game_deck[i] = card;
    }

    // Special cards
    for (int i = 18; i < 20; i++) {
        game_deck[i] = GenerateCard(Card_Type_Special, 0);
    }
}

void GenerateBuyStack(Card buy_deck[], Card game_deck[]) {
    memcpy(buy_deck, game_deck, 20 * sizeof(Card));

    ShuffleCards(buy_deck, 20);
}

void GenerateHand(Card game_hand[], Card buy_deck[], int* buy_deck_size_ptr) {
    for (int i = 0; i < 5; i++) {
        int random_index = GenRandomNum(0, (*buy_deck_size_ptr) - 1);

        game_hand[i] = buy_deck[random_index];
        RemoveCardFromArray(buy_deck, buy_deck_size_ptr, random_index);
    }
}

Game InitGame() {
    Game game;

    game.player.energy = 3;
    game.player.shield_pts = 0;
    game.player.hp = (Hp){ 100, 100 };

    GenerateEnemies(game.enemies);

    game.enemies_size = 2;

    game.buy_size = 20;
    game.hand_size = 5;
    game.discard_size = 0;

    GenerateDeck(game.deck);
    GenerateBuyStack(game.buy, game.deck);
    GenerateHand(game.hand, game.buy, &game.buy_size);

    game.phase = 0;

    game.focused_entity.index = -1;
    game.focused_entity.type = -1;
    game.selected_card_index = -1;
    game.selected_enemy_index = -1;

    return game;
}

void useCard(Game* game) {
    game->keyboard_keys[ALLEGRO_KEY_ENTER] |= GAME_KEY_SEEN; // Set that already processed this key

    int used_card = 0;
    Card card;

    if (game->focused_entity.type == Card_Entity) {
        Card focused_card = game->hand[game->focused_entity.index];
        card = focused_card;


        if (focused_card.cost > game->player.energy) {
            // TODO: display this msg into screen
            printf("Não possui energia suficiente!\n");
            return;
        }

        switch (focused_card.type) {
        case Card_Type_Attack:
            game->selected_card_index = game->focused_entity.index;
            break;
        case Card_Type_Defense:
            game->player.shield_pts = focused_card.effect;
            used_card = 1;
            break;
        case Card_Type_Special:
            // TODO: apply special card effect
            used_card = 1;
            break;
        }
    }
    else if (game->focused_entity.type == Enemy_Entity) {
        Card selected_card = game->hand[game->selected_card_index];
        Enemy* selected_enemy_ptr = &game->enemies[game->focused_entity.index];

        card = selected_card;

        if (game->selected_card_index == -1) {
            // TODO: display this msg into screen
            printf("Selecione uma carta primeiro!\n");
            return;
        }

        if (selected_card.cost > game->player.energy) {
            // TODO: display this msg into screen
            printf("Não possui energia suficiente!\n");
            return;
        }

        used_card = 1;

        selected_enemy_ptr->hp.crr -= selected_card.effect;
        if (selected_enemy_ptr->hp.crr < 0) selected_enemy_ptr->hp.crr = 0; // Deal the card damage to enemy


        game->selected_card_index = -1; // Unselect the card
    }

    if (used_card) {
        game->player.energy -= card.cost;
        RemoveCardFromArray(game->hand, &game->hand_size, game->selected_card_index);
        AddCardToArray(game->discard, &game->discard_size, card);
    }
}

void CheckArrowsKeys(Game* game) {
    // Check if the keys were pressed (GAME_KEY_DOWN) and weren't processed (GAME_KEY_SEEN)
    int left_key_was_pressed = (game->keyboard_keys[ALLEGRO_KEY_LEFT] & GAME_KEY_DOWN) &&
        !(game->keyboard_keys[ALLEGRO_KEY_LEFT] & GAME_KEY_SEEN);

    int right_key_was_pressed = (game->keyboard_keys[ALLEGRO_KEY_RIGHT] & GAME_KEY_DOWN) &&
        !(game->keyboard_keys[ALLEGRO_KEY_RIGHT] & GAME_KEY_SEEN);

    if (right_key_was_pressed) {
        game->focused_entity.index++;

        switch (game->focused_entity.type) {
        case Card_Entity:
            if (game->focused_entity.index > game->hand_size - 1) {
                // Focus into enemies
                game->focused_entity.type = Enemy_Entity;
                game->focused_entity.index = 0;
            }
            break;
        case Enemy_Entity:
            if (game->focused_entity.index > game->enemies_size - 1) {
                // Focus into nothing
                game->focused_entity.type = -1;
            }
            break;
        default:
            // If is focused in nothing
            game->focused_entity.type = Card_Entity;
            game->focused_entity.index = 0;
            break;
        };

        // Set now that the key were processed
        game->keyboard_keys[ALLEGRO_KEY_RIGHT] |= GAME_KEY_SEEN;
    }

    else if (left_key_was_pressed) {
        game->focused_entity.index--;

        switch (game->focused_entity.type) {
        case Card_Entity:
            if (game->focused_entity.index < 0) {
                // Focus into nothing
                game->focused_entity.type = -1;
            }
            break;
        case Enemy_Entity:
            if (game->focused_entity.index < 0) {
                // Focus into cards
                game->focused_entity.type = Card_Entity;
                game->focused_entity.index = game->hand_size - 1;
            }
            break;
        default:
            // If is focused in nothing, focus into enemies
            game->focused_entity.type = Enemy_Entity;
            game->focused_entity.index = game->enemies_size - 1;

            break;
        };

        // Set now that the key were processed
        game->keyboard_keys[ALLEGRO_KEY_LEFT] |= GAME_KEY_SEEN;
    }
}

void CheckKeys(Game* game) {
    CheckArrowsKeys(game);

    // Debug instant damage
    if (game->keyboard_keys[ALLEGRO_KEY_X] & GAME_KEY_DOWN) {
        if (game->player.hp.crr > 1) game->player.hp.crr = 1;
    }

    if (game->keyboard_keys[ALLEGRO_KEY_SPACE] & GAME_KEY_DOWN) {
        for (int i = 0; i < game->enemies_size; i++) {
            game->enemies[i].hp.crr = 0;
        }
    }

    // If pressed the ENTER key and if still it wasn't processed
    if (!(game->keyboard_keys[ALLEGRO_KEY_ENTER] & GAME_KEY_SEEN) && (game->keyboard_keys[ALLEGRO_KEY_ENTER] & GAME_KEY_DOWN)) {
        useCard(game);
    }

}

void AdvanceGame(Renderer* renderer, Game* game) {
    CheckKeys(game);
}
