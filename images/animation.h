#ifndef _ANIMATION_H
#define _ANIMATION_H

#include "load-images.h"

typedef int animation_id;

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
    animation_id id;

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

animation_id StartAnimation(Animation animation);
void StopAnimation(animation_id animation_id);
void RenderAnimations();

void FreeAnimations();

#endif