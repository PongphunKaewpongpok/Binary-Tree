#include "temperature.h"

#include "raylib.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"

#include "time.h"
#include "declaration.h"
#include "sound.h"
#include "screen_effect.h"
#include "drag_screen.h"
#include "branch.h"
#include "weather.h"
#include "data.h"
#include "temperature.h"
#include "string.h"

int main(void) {
    char *portName = "\\\\.\\COM3";
    printf("%s", setupSerial(portName) ? "Connected to COM3.\n" : "Failed to connect to COM3.\n");

    char temperatureText[10];

    timeStart();
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Binary Tree");
    InitAudioDevice();
    SetTargetFPS(60);
    SetExitKey(0); // Avoid exit by pressing ESC button

    initTexture2D();
    initMusic();
    branchStart();
    branchAdd((Vector2){780, 725}, 0.0f, NULL);

    fade_screen();
    // PlaySound(BGM_GARDEN_DREAMS);

    weatherStart();
    
    while (!WindowShouldClose()) {
        mouse_pos = GetMousePosition();
        updateMusic();

        BeginDrawing();     // Starting Draw Elements This Frame.

        drag_screen_cal();  // Calculate drawing_map()
        updateWeather();
        drawing_map();      // Starting Draw Ground & Cloud

        updateBranch();
        updateLightSource();
        update_fading();
        
        displayDay();           // Display Current Day
        displayTokens();        // Display Tokens

        DrawText(temperatureText, 29, SCREENHEIGHT-71, 50, BLACK); // Shadow
        DrawText(temperatureText, 25, SCREENHEIGHT-75, 50, getStatus() ? WHITE : RED); // Display Temperature

        gameFrame++;
        if (gameFrame >= 60) {
            gameFrame = 0;
            updateTime();   // Update Time
            if (getStatus()) {
                sprintf(temperatureText, "%.2f °C", getCelsius());
            } else {
                strcpy(temperatureText, "Lost °C");
                setupSerial(portName);
            }
        }

        EndDrawing();       // Ending Drawing Elements This Frame.
    }

    unloadAllTexture2D();
    unloadAllMusic();

    disableHandle();

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
