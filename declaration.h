#ifndef DECLARATION_H
#define DECLARATION_H

#include "raylib.h"

extern const int SCREENWIDTH;
extern const int SCREENHEIGHT;

extern Texture2D TEXTURE_BUTTON_ADD_BRANCH;
extern Texture2D TEXTURE_BUTTON_ADD_LEAVE;
extern Texture2D TEXTURE_BUTTON_ADD_FLOWER;
extern Texture2D TEXTURE_BUTTON_ADD_FRUIT;
extern Texture2D TEXTURE_BUTTON_REMOVE_BRANCH;
extern Texture2D TEXTURE_BUTTON_REMOVE_LEAVE;
extern Texture2D TEXTURE_BUTTON_REMOVE_FLOWER;
extern Texture2D TEXTURE_BUTTON_REMOVE_FRUIT;
extern Texture2D TEXTURE_BUTTON_CANT_ADD_BRANCH;
extern Texture2D TEXTURE_BUTTON_CANT_REMOVE_BRANCH;
extern Texture2D TEXTURE_BUTTON_CANT_ADD_LEAVE;
extern Texture2D TEXTURE_BUTTON_CANT_ADD_FLOWER;
extern Texture2D TEXTURE_BUTTON_CANT_ADD_FRUIT;
extern Texture2D TEXTURE_BUTTON_BACK_TO_TREE;
extern Texture2D TEXTURE_BUTTON_YES_BUTTON;
extern Texture2D TEXTURE_BUTTON_NO_BUTTON;

extern Texture2D TEXTURE_BRANCH;
extern Texture2D TEXTURE_BRANCH_SELECTED;
extern Texture2D TEXTURE_GROUND;
extern Texture2D TEXTURE_CLOUD;
extern Texture2D TEXTURE_RAINDROP;
extern Texture2D TEXTURE_SNOWFLAKE;
extern Texture2D TEXTURE_FLOWER;
extern Texture2D TEXTURE_TOKEN_BACKGROUND;
extern Texture2D TEXTURE_WARNING_BACKGROUND;

extern Texture2D TEXTURE_APPLE_TOKEN;
extern Texture2D TEXTURE_WATER_TOKEN;
extern Texture2D TEXTURE_SUN_TOKEN;

extern Texture2D TEXTURE_ICON_DAYTIME;
extern Texture2D TEXTURE_ICON_NIGHTTIME;

extern Vector2 mouse_pos;

void initTexture2D();
void unloadAllTexture2D();

#endif
