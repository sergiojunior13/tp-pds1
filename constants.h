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
#define PLAYER_RADIUS 100

#define ENEMY_BEGIN_Y 425
#define ENEMY_RADIUS 100

#define HEALTH_BAR_HEIGHT 20
#define HEALTH_BAR_PADDING 2

#define HEALTH_BAR_RX 2
#define HEALTH_BAR_RY 2

#define DEFAULT_LINE_HEIGHT 13

/*  ------ Card ------ */

#define HAND_Y_DISTANCE_TO_BOTTOM 10
#define CARD_FOCUSED_Y_DISTANCE_TO_BOTTOM 20

#define CARD_WIDTH 140
#define CARD_HEIGHT CARD_WIDTH*4/2.88

#define CARD_X_OFFSET 150

/*  ------ Deck ------ */

#define DRAW_DECK_X 15
#define DRAW_DECK_Y 250

#define DECK_WIDTH 80
#define DECK_HEIGHT 80

// You might want to use this.
typedef enum {
  MOVE_LEFT = 0,
  MOVE_RIGHT = 1,
} CursorMovementDirection;

#endif