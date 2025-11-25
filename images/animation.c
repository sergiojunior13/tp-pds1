#include <stdio.h>
#include <stdlib.h>
#include "animation.h"
#include "renderer.h"

Animation* loaded_animations;
int loaded_animations_size = 0;
int initialized_loaded_animations = 0;

// Animation array

void InitAnimations() {
    loaded_animations = (Animation*)malloc(sizeof(Animation) * 1);

    initialized_loaded_animations = 1;
}

void ResizeAnimations(int new_size) {
    if (initialized_loaded_animations)
        loaded_animations = (Animation*)realloc(loaded_animations, sizeof(Animation) * new_size);
}

void FreeAnimations() {
    if (!initialized_loaded_animations)
        return;

    for (int i = 0; i < loaded_animations_size; i++)
        if (loaded_animations[i].is_playing) free(loaded_animations[i].sprite_sheet.frames);

    free(loaded_animations);
}

// Frames array

void InitFrames(Animation* animation) {
    animation->sprite_sheet.frames = (Frame*)malloc(sizeof(Frame) * animation->sprite_sheet.frames_size);

    for (int i = 0; i < animation->sprite_sheet.frames_size; i++) {
        animation->sprite_sheet.frames[i].x = i * animation->sprite_sheet.frame_width;
        animation->sprite_sheet.frames[i].y = 0;
    }
}

// ----------

animation_id StartAnimation(Animation animation) {
    loaded_animations_size++;

    if (initialized_loaded_animations)
        ResizeAnimations(loaded_animations_size);
    else
        InitAnimations();

    animation.id = loaded_animations_size - 1;
    animation.is_playing = 1;
    animation.crr_frame = 0;
    animation.last_advanced_frame_time_seconds = al_get_time();

    InitFrames(&animation);

    loaded_animations[loaded_animations_size - 1] = animation;

    return animation.id;
}

void StopAnimation(animation_id id) {
    if (id == -1)  return;

    free(loaded_animations[id].sprite_sheet.frames);

    loaded_animations[id].is_playing = 0;

    if (loaded_animations[id].OnFinish != NULL) {
        loaded_animations[id].OnFinish();
    }
}

void AdvanceAnimation(Animation* animation) {
    animation->crr_frame++;

    if (animation->loop)
        animation->crr_frame = animation->crr_frame % animation->sprite_sheet.frames_size;
    else if (animation->crr_frame == animation->sprite_sheet.frames_size)
        StopAnimation(animation->id);

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
        animation->height, 0
    );
}

void RenderAnimations() {
    for (int i = 0; i < loaded_animations_size; i++) {
        if (loaded_animations[i].is_playing)
            RenderAnimation(&loaded_animations[i]);
    }
}
