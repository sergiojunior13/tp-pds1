#include <stdio.h>
#include <stdlib.h>
#include "animation.h"
#include "renderer.h"

void InitFrames(Animation* animation) {
    animation->sprite_sheet.frames = (Frame*)malloc(sizeof(Frame) * animation->sprite_sheet.frames_size);

    for (int i = 0; i < animation->sprite_sheet.frames_size; i++) {
        animation->sprite_sheet.frames[i].x = i * animation->sprite_sheet.frame_width;
        animation->sprite_sheet.frames[i].y = 0;
    }
}
void InitAnimation(Animation* animation) {
    animation->is_playing = 1;
    animation->started = 1;

    animation->crr_frame = 0;
    animation->last_advanced_frame_time_seconds = al_get_time();

    InitFrames(animation);
}

void StopAnimation(Animation* animation) {
    if (animation->is_playing)
        free(animation->sprite_sheet.frames);

    animation->is_playing = 0;

    if (animation->animation_to_start_after_end != NULL) {
        InitAnimation(animation->animation_to_start_after_end);
    }
}

void AdvanceAnimation(Animation* animation) {
    animation->crr_frame++;

    if (animation->loop)
        animation->crr_frame = animation->crr_frame % animation->sprite_sheet.frames_size;
    else if (animation->crr_frame == animation->sprite_sheet.frames_size) // If not is loop and the ani. ended
        StopAnimation(animation);

    animation->last_advanced_frame_time_seconds += 1.0 / animation->fps;
}


void RenderAnimation(Animation* animation) {
    if (al_get_time() >= (animation->last_advanced_frame_time_seconds + 1.0 / animation->fps))
        AdvanceAnimation(animation);

    if (!animation->is_playing) return;


    Frame crr_frame = animation->sprite_sheet.frames[animation->crr_frame];

    al_draw_scaled_bitmap(
        animation->sprite_sheet.image->bitmap,
        crr_frame.x,
        crr_frame.y,
        animation->sprite_sheet.frame_width,
        animation->sprite_sheet.frame_height,
        animation->x,
        animation->y,
        animation->width,
        animation->height, animation->invert_horizontally ? ALLEGRO_FLIP_HORIZONTAL : 0
    );
}