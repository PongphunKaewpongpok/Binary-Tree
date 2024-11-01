#include "data.h"

#include "raylib.h"
#include "declaration.h"
#include "basic_math.h"
#include "string.h"

// Permanently Token
int applesToken         = 0;

// Temporary Token
double waterToken       = 100;
double sunlightToken    = 100;

void displayTokens() {
    DrawTexture(TEXTURE_TOKEN_BACKGROUND, 300, 10, WHITE);
    DrawTexture(TEXTURE_TOKEN_BACKGROUND, 650, 10, WHITE);
    DrawTexture(TEXTURE_TOKEN_BACKGROUND, 1000, 10, WHITE);

    DrawTexture(TEXTURE_SUN_TOKEN, 560, 15, WHITE);
    DrawTexture(TEXTURE_WATER_TOKEN, 910, 10, WHITE);
    DrawTexture(TEXTURE_APPLE_TOKEN, 1275, 10, WHITE);

    char sunlightText[6];
    char waterText[6];
    char applesText[6];
    shortFormToken(sunlightText, sunlightToken);
    shortFormToken(waterText, waterToken);
    shortFormToken(applesText, applesToken);

    // Shadow
    DrawText(sunlightText, 450-10*strlen(sunlightText)+4, 21, 40, BLACK);
    DrawText(waterText, 800-10*strlen(waterText)+4, 21, 40, BLACK);
    DrawText(applesText, 1150-10*strlen(applesText)+4, 21, 40, BLACK);

    // Main
    DrawText(sunlightText, 450-10*strlen(sunlightText), 17, 40, YELLOW);
    DrawText(waterText, 800-10*strlen(waterText), 17, 40, (Color){135, 255, 255, 255});
    DrawText(applesText, 1150-10*strlen(applesText), 17, 40, RED);
}
