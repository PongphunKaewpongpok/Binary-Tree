#include "time.h"

#include <time.h>
#include <stdio.h>
#include "stdlib.h"
#include "raylib.h"
#include "declaration.h"
#include <stdbool.h>

int gameFrame = 0;
char textDisplayDay[10];

Time game_time = {0, 0, 0};

/*

    1  game_time.minute                         (1 game_time.minute = 10 seconds real-time)
    60 game_time.minute = 1 game_time.hour      (1 day_hour = 10 seconds real-time)
    24 game_time.hour   = 1 game_time.day       (1 day = 4 minutes real-time)

*/
void timeStart() {
    srand(time(NULL));
    sprintf(textDisplayDay, "DAY %d", game_time.days);
}

void updateTime() { // Update every 1 second
    game_time.minutes += 6;
    if (game_time.minutes >= 60) {
        game_time.hours += game_time.minutes/60;
        game_time.minutes %= 60;
    }
    if (game_time.hours >= 24) {
        game_time.days += game_time.hours/24;
        game_time.hours %= 24;
        sprintf(textDisplayDay, "DAY %d", game_time.days);
    }
}

void displayDay() {
    if (game_time.hours >= 6 && game_time.hours <= 18) {
        DrawTexture(TEXTURE_ICON_DAYTIME, 69, 16, BLACK); // Shadow
        DrawTexture(TEXTURE_ICON_DAYTIME, 65, 12, WHITE);
    } else {
        DrawTexture(TEXTURE_ICON_NIGHTTIME, 69, 17, BLACK); // Shadow
        DrawTexture(TEXTURE_ICON_NIGHTTIME, 65, 13, WHITE);
    }
    
    DrawText(textDisplayDay, 129, 16, 50, BLACK); // Shadow
    DrawText(textDisplayDay, 125, 10, 50, WHITE);
}

Time getTime() {
    return game_time;
}

bool isDay() {
    return (game_time.hours >= 6 && game_time.hours <= 18);
}