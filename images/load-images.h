#ifndef _LOADIMAGES_
#define _LOADIMAGES_

#include <allegro5/allegro5.h>

typedef enum Imgs_Ids {
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
    Total_Images
} Imgs_Ids;

extern ALLEGRO_BITMAP* imgs_bitmaps[Total_Images];

void load_images();
void destroy_images();

#endif