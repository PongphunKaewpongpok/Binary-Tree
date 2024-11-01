#include "event.h"

#include "raylib.h"
#include "data.h"
#include "declaration.h"
#include "drag_screen.h"
#include <stdbool.h>
#include "basic_math.h"
#include "branch.h"
#include <stdio.h>

const int EVENT_ID_NONE = 0;
const int EVENT_ID_FLOOD = 1;

int current_event = EVENT_ID_NONE;
int flood_left = 0;
int human_left = 0;

int waterTokenSave = 0;

int next_event_time = 720;


void nextEventTime() {
    if (next_event_time > 0) {
        next_event_time--;
    } else {
        next_event_time = randomNumber(720, 1200); // Random Next Event Will Happen In 3-5 In-Game Days
        nextEventRandom();
    }
}

void nextEventRandom() {
    current_event = randomNumber(0, 1);
    if (current_event == EVENT_ID_FLOOD) {
        flood_left = randomNumber(100*branch_count, 150*branch_count);
        waterTokenSave = waterToken;
        waterToken += flood_left;
    }
}

void updateEvent() {
    if (current_event == EVENT_ID_NONE) {
        return;
    } else if (current_event == EVENT_ID_FLOOD) {
        DrawRectangle(0, 725-((waterToken-waterTokenSave)/5)+screen_pos.y, SCREENWIDTH, ((waterToken-waterTokenSave)/5), (Color){0, 0, 255, 150});
    }
    if (waterToken-waterTokenSave <= 0) {
        flood_left = 0;
        current_event = EVENT_ID_NONE;
    }
}

int getCurrentEvent() {
    return current_event;
}
