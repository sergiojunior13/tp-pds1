#include "load-images.h"
#include "utils.h"
#include <allegro5/allegro5.h>

Image images[] = {
    {Sword_Img_Id, "images/sword.png"},
    {Shield_Img_Id, "images/shield.png"},
    {Card_Atk_Img_Id, "images/card-attack.png"},
    {Card_Def_Img_Id, "images/card-defense.png"},
    {Card_Spe_Img_Id, "images/card-special.png"},
    {Buy_Stack_Img_Id, "images/buy-stack.png"},
    {Discard_Stack_Img_Id, "images/discard-stack.png"},
    {Energy_Img_Id, "images/energy.png"},
    {Normal_Bg_Img_Id, "images/normal-bg.png"},
    {End_Bg_Img_Id, "images/end-bg.png"},
};

void LoadImages() {
    for (int i = 0; i < Total_Images; i++) {
        images[i].bitmap = al_load_bitmap(images[i].src);
        must_init(images[i].bitmap, "load image");
    }
};

Image* GetImage(Image_Id image_id) {
    for (int i = 0; i < Total_Images; i++)
        if (images[i].id == image_id) return &images[i];

    return NULL;
}

void DestroyImages() {
    for (int i = 0; i < Total_Images; i++) {
        al_destroy_bitmap(images[i].bitmap);
        images[i].bitmap = NULL;
    }
}