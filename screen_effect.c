#include "screen_effect.h"

#include "declaration.h"
#include "time.h"
#include "basic_math.h"
#include "weather.h"
#include "custom_raylib.h"

#include "raylib.h"

int fading_time = 0;
int fade_alpha = 255;
short fade_direction = 0;

Color current_bg = (Color){5, 0, 50, 255};
Color nighttime_bg = (Color){5, 0, 50, 255};

void update_fading() {
    if (fading_time >= 1) {
        if (fade_alpha == 0) {
            fade_direction = 1;
            fading_time--;
            fade_alpha += 2;
            DrawRectangle(0, 0, SCREENWIDTH, SCREENHEIGHT, (Color){0, 0, 0, fade_alpha});
            return;
        } else if (fade_alpha == 255) {
            fade_direction = 0;
            fading_time--;
            fade_alpha -= 2;
            DrawRectangle(0, 0, SCREENWIDTH, SCREENHEIGHT, (Color){0, 0, 0, fade_alpha});
            return;
        }
    }
    if (fade_alpha > 0 && fade_direction == 0) {
        fade_alpha -= 2;
        if (fade_alpha < 0)
            fade_alpha = 0;
    } else if (fade_alpha < 255 && fade_direction == 1) {
        fade_alpha += 2;
        if (fade_alpha > 255)
            fade_alpha = 255;
    }
    DrawRectangle(0, 0, SCREENWIDTH, SCREENHEIGHT, (Color){0, 0, 0, fade_alpha});
}

void fade_screen() {
    fading_time++;
}

void updateLightSource() {
    Time temTime = getTime();
    ClearBackground(current_bg);
    if (temTime.hours >= 6 && temTime.hours <= 18) {
        if (!colorEqualsNoAlpha(current_bg, background_color)) {
            current_bg.r += (current_bg.r == background_color.r) ? 0 : ((current_bg.r > background_color.r) ? -1 : 1);
            current_bg.g += (current_bg.g == background_color.g) ? 0 : ((current_bg.g > background_color.g) ? -1 : 1);
            current_bg.b += (current_bg.b == background_color.b) ? 0 : ((current_bg.b > background_color.b) ? -1 : 1);
        }
        return;
    } else if ((temTime.hours >= 19 && temTime.hours < 24) || (temTime.hours >= 0 && temTime.hours <= 5) ) {
        if (!colorEqualsNoAlpha(current_bg, nighttime_bg)) {
            current_bg.r += (current_bg.r == nighttime_bg.r) ? 0 : ((current_bg.r > nighttime_bg.r) ? -1 : 1);
            current_bg.g += (current_bg.g == nighttime_bg.g) ? 0 : ((current_bg.g > nighttime_bg.g) ? -1 : 1);
            current_bg.b += (current_bg.b == nighttime_bg.b) ? 0 : ((current_bg.b > nighttime_bg.b) ? -1 : 1);
        }
    }
}