#include "load-images.h"
#include "utils.h"
#include <allegro5/allegro5.h>

ALLEGRO_BITMAP* imgs_bitmaps[Total_Images];


// The image id/index in the Imgs_Ids enum MUST correspond to the index in this array.
const char* imgs_src[] = {
    "images/shield.png",
    "images/card-attack.png",
    "images/card-defense.png",
    "images/card-special.png",
    "images/buy-stack.png",
    "images/discard-stack.png",
    "images/normal-bg.png",
    "images/end-bg.png",
};

void load_images() {
    for (int i = 0; i < Total_Images; i++) {
        imgs_bitmaps[i] = al_load_bitmap(imgs_src[i]);
        must_init(imgs_bitmaps[i], "load image");
    }
};

void destroy_images() {
    for (int i = 0; i < Total_Images; i++) {
        al_destroy_bitmap(imgs_bitmaps[i]);
        imgs_bitmaps[i] = NULL;
    }
}