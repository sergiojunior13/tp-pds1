#ifndef _ANIMATION_H
#define _ANIMATION_H

#include "load-images.h"

typedef enum Animation_Id {
    Player_Attack_Ani_Id,
    Player_Defense_Ani_Id,
    Player_Special_Ani_Id,
    Player_Idle_Ani_Id,
    Total_Animations // Must be the last
} Animation_Id;

typedef void(*Callback_Function)();

typedef struct Frame {
    float x, y;
} Frame;

typedef struct SpriteSheet {
    Image* image;

    Frame* frames;
    int frames_size;

    float frame_width, frame_height;
} SpriteSheet;

typedef struct Animation {
    Animation_Id id;

    SpriteSheet sprite_sheet;

    int x, y;
    float width;
    float height;

    int crr_frame;

    double last_advanced_frame_time_seconds, fps;

    int is_playing;

    int loop;
    Callback_Function OnFinish;
} Animation;

void StartAnimation(Animation animation);
void StopAnimation(Animation_Id animation_id);
void RenderAnimations();

void StopAnimations();

#endif