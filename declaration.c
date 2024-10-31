#include "declaration.h"

#include "raylib.h"

const int SCREENWIDTH = 1600;
const int SCREENHEIGHT = 900;

// Texture2D

Texture2D TEXTURE_BUTTON_ADD_BRANCH;
Texture2D TEXTURE_BUTTON_ADD_LEAVE;
Texture2D TEXTURE_BUTTON_ADD_FLOWER;
Texture2D TEXTURE_BUTTON_ADD_FRUIT;
Texture2D TEXTURE_BUTTON_REMOVE_BRANCH;
Texture2D TEXTURE_BUTTON_REMOVE_LEAVE;
Texture2D TEXTURE_BUTTON_REMOVE_FLOWER;
Texture2D TEXTURE_BUTTON_REMOVE_FRUIT;
Texture2D TEXTURE_BUTTON_CANT_ADD_BRANCH;
Texture2D TEXTURE_BUTTON_CANT_ADD_LEAVE;
Texture2D TEXTURE_BUTTON_CANT_ADD_FLOWER;
Texture2D TEXTURE_BUTTON_CANT_ADD_FRUIT;
Texture2D TEXTURE_BUTTON_CANT_REMOVE_BRANCH;

Texture2D TEXTURE_BRANCH;
Texture2D TEXTURE_BRANCH_SELECTED;
Texture2D TEXTURE_GROUND;
Texture2D TEXTURE_CLOUD;
Texture2D TEXTURE_RAINDROP;
Texture2D TEXTURE_SNOWFLAKE;
Texture2D TEXTURE_TOKEN_BACKGROUND;
Texture2D TEXTURE_ICON_SETTING;

Texture2D TEXTURE_APPLE_TOKEN;
Texture2D TEXTURE_WATER_TOKEN;
Texture2D TEXTURE_SUN_TOKEN;

Texture2D TEXTURE_ICON_DAYTIME;
Texture2D TEXTURE_ICON_NIGHTTIME;

// Importance Game Data
Vector2 mouse_pos;

void initTexture2D() {
    TEXTURE_BUTTON_ADD_BRANCH           = LoadTexture("Resources/button/add_button.png");
    TEXTURE_BUTTON_ADD_LEAVE            = LoadTexture("Resources/button/add_leave.png");
    TEXTURE_BUTTON_ADD_FLOWER           = LoadTexture("Resources/button/add_flower.png");
    TEXTURE_BUTTON_ADD_FRUIT            = LoadTexture("Resources/button/add_fruit.png");
    TEXTURE_BUTTON_REMOVE_BRANCH        = LoadTexture("Resources/button/remove_button.png");
    TEXTURE_BUTTON_REMOVE_LEAVE         = LoadTexture("Resources/button/remove_leave.png");
    TEXTURE_BUTTON_REMOVE_FLOWER        = LoadTexture("Resources/button/remove_flower.png");
    TEXTURE_BUTTON_REMOVE_FRUIT         = LoadTexture("Resources/button/remove_fruit.png");
    TEXTURE_BUTTON_CANT_ADD_BRANCH      = LoadTexture("Resources/button/cant_add_button.png");
    TEXTURE_BUTTON_CANT_ADD_LEAVE       = LoadTexture("Resources/button/cant_add_leave.png");
    TEXTURE_BUTTON_CANT_ADD_FLOWER      = LoadTexture("Resources/button/cant_add_flower.png");
    TEXTURE_BUTTON_CANT_ADD_FRUIT       = LoadTexture("Resources/button/cant_add_fruit.png");
    TEXTURE_BUTTON_CANT_REMOVE_BRANCH   = LoadTexture("Resources/button/cant_remove_button.png");
    TEXTURE_BRANCH                      = LoadTexture("Resources/branch.png");
    TEXTURE_BRANCH_SELECTED             = LoadTexture("Resources/branch_selected.png");
    TEXTURE_GROUND                      = LoadTexture("Resources/ground.png");
    TEXTURE_CLOUD                       = LoadTexture("Resources/cloud.png");
    TEXTURE_RAINDROP                    = LoadTexture("Resources/raindrops.png");
    TEXTURE_SNOWFLAKE                   = LoadTexture("Resources/snowflakes.png");

    TEXTURE_APPLE_TOKEN                 = LoadTexture("Resources/icon/apple_token.png");
    TEXTURE_WATER_TOKEN                 = LoadTexture("Resources/icon/water_token.png");
    TEXTURE_SUN_TOKEN                   = LoadTexture("Resources/icon/daytime.png");

    TEXTURE_ICON_SETTING                = LoadTexture("Resources/button/setting_button.png");
    TEXTURE_ICON_DAYTIME                = TEXTURE_SUN_TOKEN;
    TEXTURE_ICON_NIGHTTIME              = LoadTexture("Resources/icon/nighttime.png");
    TEXTURE_TOKEN_BACKGROUND            = LoadTexture("Resources/gui/token_bg.png");
}

void unloadAllTexture2D() {
    UnloadTexture(TEXTURE_BUTTON_ADD_BRANCH);
    UnloadTexture(TEXTURE_BUTTON_ADD_LEAVE);
    UnloadTexture(TEXTURE_BUTTON_ADD_FLOWER);
    UnloadTexture(TEXTURE_BUTTON_ADD_FRUIT);
    UnloadTexture(TEXTURE_BUTTON_REMOVE_BRANCH);
    UnloadTexture(TEXTURE_BUTTON_REMOVE_LEAVE);
    UnloadTexture(TEXTURE_BUTTON_REMOVE_FLOWER);
    UnloadTexture(TEXTURE_BUTTON_REMOVE_FRUIT);
    UnloadTexture(TEXTURE_BUTTON_CANT_ADD_BRANCH);
    UnloadTexture(TEXTURE_BUTTON_CANT_ADD_LEAVE);
    UnloadTexture(TEXTURE_BUTTON_CANT_ADD_FLOWER);
    UnloadTexture(TEXTURE_BUTTON_CANT_ADD_FRUIT);
    UnloadTexture(TEXTURE_BUTTON_CANT_REMOVE_BRANCH);

    UnloadTexture(TEXTURE_BRANCH);
    UnloadTexture(TEXTURE_BRANCH_SELECTED);
    UnloadTexture(TEXTURE_GROUND);
    UnloadTexture(TEXTURE_CLOUD);
    UnloadTexture(TEXTURE_RAINDROP);
    UnloadTexture(TEXTURE_SNOWFLAKE);

    UnloadTexture(TEXTURE_APPLE_TOKEN);
    UnloadTexture(TEXTURE_WATER_TOKEN);
    UnloadTexture(TEXTURE_SUN_TOKEN);

    UnloadTexture(TEXTURE_ICON_SETTING);
    UnloadTexture(TEXTURE_ICON_DAYTIME);
    UnloadTexture(TEXTURE_ICON_NIGHTTIME);
    UnloadTexture(TEXTURE_TOKEN_BACKGROUND);
}