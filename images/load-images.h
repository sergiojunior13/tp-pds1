#ifndef _LOADIMAGES_
#define _LOADIMAGES_

#include <allegro5/allegro5.h>

typedef enum Image_Id {
    Sword_Img_Id,
    Shield_Img_Id,
    Card_Atk_Img_Id,
    Card_Def_Img_Id,
    Card_Spe_Img_Id,
    Buy_Stack_Img_Id,
    Discard_Stack_Img_Id,
    Energy_Img_Id,
    Normal_Bg_Img_Id,
    End_Bg_Img_Id,
    Player_Idle_Img_Id,
    Player_Attack_Img_Id,
    Player_Defense_Img_Id,
    Player_Special_Img_Id,
    Weak_Enemy_Idle_Img_Id,
    Weak_Enemy_Attack_Img_Id,
    Weak_Enemy_Hurt_Img_Id,
    Weak_Enemy_Dead_Img_Id,
    Strong_Enemy_Idle_Img_Id,
    Strong_Enemy_Attack_Img_Id,
    Strong_Enemy_Hurt_Img_Id,
    Strong_Enemy_Dead_Img_Id,
    Total_Images // Need's to be the last
} Image_Id;

typedef struct Image {
    Image_Id id;
    char* src;
    ALLEGRO_BITMAP* bitmap;
} Image;

Image* GetImage(Image_Id image_id);
void LoadImages();
void DestroyImages();
void DestroyImage(Image_Id image_id);

#endif