#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#define GAME_KEY_SEEN 1
#define GAME_KEY_DOWN 2

/*  ------ Display ------ */

#define DISPLAY_BUFFER_WIDTH 1600.0
#define DISPLAY_BUFFER_HEIGHT 900.0

#define DISPLAY_SCALE 1.0
#define DISPLAY_WIDTH DISPLAY_SCALE* DISPLAY_BUFFER_WIDTH
#define DISPLAY_HEIGHT DISPLAY_SCALE* DISPLAY_BUFFER_HEIGHT

/*  ------ Creatures: Player/Enemy ------ */

#define CREATURE_DISTANCE_TO_WINDOW_CORNER 300

#define PLAYER_BEGIN_Y 425
#define PLAYER_SIZE 250

#define ENEMY_BEGIN_Y 425
#define ENEMY_RADIUS 100

#define HEALTH_BAR_HEIGHT 20
#define HEALTH_BAR_PADDING 2

#define HEALTH_BAR_RX 2
#define HEALTH_BAR_RY 2

#define DEFAULT_LINE_HEIGHT 0.7

/*  ------ Card ------ */

#define HAND_Y_DISTANCE_TO_BOTTOM 10
#define CARD_FOCUSED_Y_DISTANCE_TO_BOTTOM 20

#define CARD_WIDTH 140
#define CARD_HEIGHT CARD_WIDTH*4/2.88

#define CARD_X_OFFSET 150

/*  ------ Decks (Buy/Discard) ------ */

#define DECK_DISTANCE_X_TO_BORDER 30
#define DECK_DISTANCE_Y_TO_BORDER 10

#define DECK_WIDTH 140
#define DECK_HEIGHT DECK_WIDTH*274/300

/*  ------ Energy ------ */

#define ENERGY_X 50
#define ENERGY_Y 50

#define ENERGY_WIDTH 100

// You might want to use this.
typedef enum {
  MOVE_LEFT = 0,
  MOVE_RIGHT = 1,
} CursorMovementDirection;

#endif