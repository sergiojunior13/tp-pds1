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
    {Player_Idle_Img_Id, "images/player/player-idle.png"},
    {Player_Attack_Img_Id, "images/player/player-attack.png"},
    {Player_Defense_Img_Id, "images/player/player-defense.png"},
    {Player_Special_Img_Id, "images/player/player-special.png"},
    {Weak_Enemy_Idle_Img_Id, "images/enemies/weak_enemy_idle.png"},
    {Weak_Enemy_Attack_Img_Id, "images/enemies/weak_enemy_attack.png"},
    {Weak_Enemy_Hurt_Img_Id, "images/enemies/weak_enemy_hurt.png"},
    {Weak_Enemy_Dead_Img_Id, "images/enemies/weak_enemy_dead.png"},
    {Strong_Enemy_Idle_Img_Id, "images/enemies/strong_enemy_idle.png"},
    {Strong_Enemy_Attack_Img_Id, "images/enemies/strong_enemy_attack.png"},
    {Strong_Enemy_Hurt_Img_Id, "images/enemies/strong_enemy_hurt.png"},
    {Strong_Enemy_Dead_Img_Id, "images/enemies/strong_enemy_dead.png"},
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
    for (int i = 0; i < Total_Images; i++)
        DestroyImage(i);
}

void DestroyImage(Image_Id image_id) {
    al_destroy_bitmap(images[image_id].bitmap);
    images[image_id].bitmap = NULL;
}