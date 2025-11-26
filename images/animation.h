#ifndef _ANIMATION_H
#define _ANIMATION_H

#include "load-images.h"

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
    SpriteSheet sprite_sheet;

    int x, y;
    float width;
    float height;
    int invert_horizontally;

    int crr_frame;

    double last_advanced_frame_time_seconds, fps;

    int started;
    int is_playing;

    int loop;

    struct Animation* animation_to_start_after_end;
} Animation;

void InitAnimation(Animation* animation);
void RenderAnimation(Animation* animation);
void StopAnimation(Animation* animation);
#endif