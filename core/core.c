#include "renderer.h"
#include "core.h"
#include "entities/enemy/enemy.h"
#include "entities/card/card.h"
#include "constants.h"

void removeCardFromArray(Card array[], int size, int element_index) {
    for (int i = element_index; i < size - 1; i++) {
        array[i] = array[i + 1];
    }
}

void GenerateDeck(Card gameDeck[]) {
    // Attack cards
    for (int i = 0; i < 10; i++) {
        Card card;
        if (i == 0) card = GenerateCard(Card_Type_Attack, 0); // At least 1 card of cost 0
        else if (i == 1) card = GenerateCard(Card_Type_Attack, 2); // At least 1 card of cost 2
        else if (i == 2) card = GenerateCard(Card_Type_Attack, 3); // At least 1 card of cost 3
        else if (i <= 5) card = GenerateCard(Card_Type_Attack, 1); // At least 3 cards of cost 1
        else card = GenerateCard(Card_Type_Attack, GenRandomNum(0, 3)); // Gen the last 4 atk cards randomly

        gameDeck[i] = card;
    }

    // Defense cards
    for (int i = 10; i < 18; i++) {
        Card card;
        if (i == 10) card = GenerateCard(Card_Type_Defense, 0); // At least 1 card of cost 0
        else if (i == 11) card = GenerateCard(Card_Type_Defense, 2); // At least 1 card of cost 2
        else if (i == 12) card = GenerateCard(Card_Type_Defense, 3); // At least 1 card of cost 3
        else if (i <= 15) card = GenerateCard(Card_Type_Defense, 1); // At least 3 cards of cost 1
        else card = GenerateCard(Card_Type_Defense, GenRandomNum(0, 3)); // Gen the last 4 def cards randomly

        gameDeck[i] = card;
    }

    // Special cards
    for (int i = 18; i < 20; i++) {
        gameDeck[i] = GenerateCard(Card_Type_Special, 0);
    }
}

void GenerateHand(Card gameHand[], Card gameDeck[]) {
    for (int i = 0; i < 5;i++) {
        int random_index = GenRandomNum(0, 19 - i);

        gameHand[i] = gameDeck[random_index];
        removeCardFromArray(gameDeck, 20 - i, random_index);
    }
}

Game InitGame() {
    Game game;

    game.player.energy = 3;
    game.player.hp = (Hp){ 100, 100 };

    GenerateEnemies(game.enemies);
    GenerateDeck(game.deck);
    GenerateHand(game.hand, game.deck);

    game.phase = 0;

    game.focused_element.index = -1;
    game.focused_element.type = -1;

    return game;
}

void checkArrowsKeys(Game* game) {
    // Check if the keys were pressed (GAME_KEY_DOWN) and weren't processed (GAME_KEY_SEEN)
    int left_key_was_pressed = (game->keyboard_keys[ALLEGRO_KEY_LEFT] & GAME_KEY_DOWN) &&
        !(game->keyboard_keys[ALLEGRO_KEY_LEFT] & GAME_KEY_SEEN);

    int right_key_was_pressed = (game->keyboard_keys[ALLEGRO_KEY_RIGHT] & GAME_KEY_DOWN) &&
        !(game->keyboard_keys[ALLEGRO_KEY_RIGHT] & GAME_KEY_SEEN);

    if (right_key_was_pressed) {
        game->focused_element.index++;

        switch (game->focused_element.type) {
        case Card_Element:
            if (game->focused_element.index > 4) {
                // Focus into enemies
                game->focused_element.type = Enemy_Element;
                game->focused_element.index = 0;
            }
            break;
        case Enemy_Element:
            if (game->focused_element.index > 1) {
                // Focus into nothing
                game->focused_element.type = -1;
            }
            break;
        default:
            // If is focused in nothing
            game->focused_element.type = Card_Element;
            game->focused_element.index = 0;
            break;
        };

        // Set now that the key were processed
        game->keyboard_keys[ALLEGRO_KEY_RIGHT] |= GAME_KEY_SEEN;
    }

    else if (left_key_was_pressed) {
        game->focused_element.index--;

        switch (game->focused_element.type) {
        case Card_Element:
            if (game->focused_element.index < 0) {
                // Focus into nothing
                game->focused_element.type = -1;
            }
            break;
        case Enemy_Element:
            if (game->focused_element.index < 0) {
                // Focus into cards
                game->focused_element.type = Card_Element;
                game->focused_element.index = 4;
            }
            break;
        default:
            // If is focused in nothing
            game->focused_element.type = Enemy_Element;
            game->focused_element.index = 1;

            break;
        };

        // Set now that the key were processed
        game->keyboard_keys[ALLEGRO_KEY_LEFT] |= GAME_KEY_SEEN;
    }
}

void AdvanceGame(Renderer* renderer, Game* game) {
    checkArrowsKeys(game);
}
