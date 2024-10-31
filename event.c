#include "weather.h"

#include "raylib.h"
#include "data.h"
#include "declaration.h"
#include "drag_screen.h"
#include "stdbool.h"
#include "basic_math.h"

const int EVENT_ID_NONE = 0;
const int EVENT_ID_FLOOD = 1;
const int EVENT_ID_HUMAN = 2;

int current_event = EVENT_ID_NONE;
int flood_left = 0;
int human_left = 0;

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
    current_event = randomNumber(0, 2);
    if (current_event == EVENT_ID_FLOOD) {
        
    } else if (current_event == EVENT_ID_HUMAN) {
        
    }
}

// void updateEvent() {
//     if (current_event == EVENT_ID_NONE) {
//         return;
//     } else if (current_event == EVENT_ID_FLOOD) {
//         DrawRectangle(0, SCREENHEIGHT, SCREENWIDTH, SCREENHEIGHT+screen_pos.y);
//     } else if (current_event == EVENT_ID_HUMAN) {

//     }
// }