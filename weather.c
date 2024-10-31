#include "weather.h"

#include "raylib.h"
#include <stdio.h>
#include "custom_raylib.h"
#include "stddef.h"
#include "drag_screen.h"
#include "basic_math.h"
#include "declaration.h"

const int WEATHER_SUNNY_ID = 0;
const int WEATHER_RAINNING_ID = 1;
const int WEATHER_SNOWING_ID = 2;

const Color WEATHER_BACKGROUND_SUNNY = (Color){200, 255, 255, 255};
const Color WEATHER_BACKGROUND_RAINNING = (Color){85, 85, 85, 255};
const Color WEATHER_BACKGROUND_SNOWING = (Color){240, 255, 255, 255};

int current_weather_id = WEATHER_SUNNY_ID;
int old_weather_id = -1;
int frameFadingCount = 0;

Vector3 particle_pos[50] = {0}; // x, y, frame
int fadingParticle = 255;

Color background_color = (Color){200, 255, 255, 255};
Color fading_value;

const Vector2 RAINDROPORIGIN = {2.5, 0};

void weatherStart() {
    for (int i = 0; i < 20; i++) {
        particle_pos[i].x = -100;
        particle_pos[i].y = -100;
        particle_pos[i].z = 30;
    }
}

void updateWeather() {
    ClearBackground(background_color);

    if (current_weather_id == WEATHER_SUNNY_ID) {
        if (frameFadingCount != 0 && colorEqualsNoAlpha(background_color, WEATHER_BACKGROUND_SUNNY)) {
            frameFadingCount = 0;
            fadingParticle = 255;
            return;
        }
        if (!colorEqualsNoAlpha(background_color, WEATHER_BACKGROUND_SUNNY)) {
            background_color.r += (background_color.r == WEATHER_BACKGROUND_SUNNY.r) ? 0 : ((background_color.r > WEATHER_BACKGROUND_SUNNY.r) ? -1 : 1);
            background_color.g += (background_color.g == WEATHER_BACKGROUND_SUNNY.g) ? 0 : ((background_color.g > WEATHER_BACKGROUND_SUNNY.g) ? -1 : 1);
            background_color.b += (background_color.b == WEATHER_BACKGROUND_SUNNY.b) ? 0 : ((background_color.b > WEATHER_BACKGROUND_SUNNY.b) ? -1 : 1);
            frameFadingCount++;
        }
    } else if (current_weather_id == WEATHER_RAINNING_ID) {
        if (frameFadingCount != 0 && colorEqualsNoAlpha(background_color, WEATHER_BACKGROUND_RAINNING)) {
            frameFadingCount = 0;
            fadingParticle = 255;
            return;
        }
        if (!colorEqualsNoAlpha(background_color, WEATHER_BACKGROUND_RAINNING)) {
            background_color.r += (background_color.r == WEATHER_BACKGROUND_RAINNING.r) ? 0 : ((background_color.r > WEATHER_BACKGROUND_RAINNING.r) ? -1 : 1);
            background_color.g += (background_color.g == WEATHER_BACKGROUND_RAINNING.g) ? 0 : ((background_color.g > WEATHER_BACKGROUND_RAINNING.g) ? -1 : 1);
            background_color.b += (background_color.b == WEATHER_BACKGROUND_RAINNING.b) ? 0 : ((background_color.b > WEATHER_BACKGROUND_RAINNING.b) ? -1 : 1);
            frameFadingCount++;
        }
    } else if (current_weather_id == WEATHER_SNOWING_ID) {
        if (frameFadingCount != 0 && colorEqualsNoAlpha(background_color, WEATHER_BACKGROUND_SNOWING)) {
            frameFadingCount = 0;
            fadingParticle = 255;
            return;
        }
        if (!colorEqualsNoAlpha(background_color, WEATHER_BACKGROUND_SNOWING)) {
            background_color.r += (background_color.r == WEATHER_BACKGROUND_SNOWING.r) ? 0 : ((background_color.r > WEATHER_BACKGROUND_SNOWING.r) ? -1 : 1);
            background_color.g += (background_color.g == WEATHER_BACKGROUND_SNOWING.g) ? 0 : ((background_color.g > WEATHER_BACKGROUND_SNOWING.g) ? -1 : 1);
            background_color.b += (background_color.b == WEATHER_BACKGROUND_SNOWING.b) ? 0 : ((background_color.b > WEATHER_BACKGROUND_SNOWING.b) ? -1 : 1);
            frameFadingCount++;
        }
    }

    updateSummonRain();
    updateSummonSnowflake();
}

void changeWeather(int weatherID) {
    frameFadingCount = 0;
    old_weather_id = current_weather_id;    // Old Weather
    current_weather_id = weatherID;         // New Weather
}

void updateSummonRain() {
    if (old_weather_id == WEATHER_RAINNING_ID && frameFadingCount != 0) {
        fadingParticle -= 5;
        if (fadingParticle < 0) {
            fadingParticle = 0;
        }
    }
    for (int i = 0; i < 50; i++) {
        if ((particle_pos[i].z >= 30 && 
            (current_weather_id == WEATHER_RAINNING_ID) && frameFadingCount == 0)) {
            float rain_y = getCloudY();
            particle_pos[i].x = randomNumber(-1200, 1500);
            particle_pos[i].y = randomNumber(min(rain_y, 0), min(rain_y, 0)-700);
            particle_pos[i].z = 0;
        }
        if ((particle_pos[i].y >= getGroundY() || particle_pos[i].z >= 1) &&
                ((old_weather_id == WEATHER_RAINNING_ID && frameFadingCount != 0) || (current_weather_id == WEATHER_RAINNING_ID && frameFadingCount == 0))) {
            DrawTexture(TEXTURE_RAINDROP, particle_pos[i].x, getGroundY()+5, (Color){100, 255, 255, fadingParticle});
            particle_pos[i].z += 1;
            continue;
        }
        if ((particle_pos[i].y > 1000 || particle_pos[i].x > 1600) && 
            ((old_weather_id == WEATHER_RAINNING_ID && frameFadingCount != 0) || (current_weather_id == WEATHER_RAINNING_ID && frameFadingCount == 0))) {
            particle_pos[i].z = 30;
        } if ((particle_pos[i].z == 0) && 
                    ((old_weather_id == WEATHER_RAINNING_ID && frameFadingCount != 0) || (current_weather_id == WEATHER_RAINNING_ID && frameFadingCount == 0))) {
            particle_pos[i].x += 10;
            particle_pos[i].y += 10;
        }
        if (old_weather_id == WEATHER_RAINNING_ID && frameFadingCount != 0) {
            if (fadingParticle <= 0) {
                particle_pos[i].z = 30;
                continue;
            }
        }
        if ((old_weather_id == WEATHER_RAINNING_ID && frameFadingCount != 0) || (current_weather_id == WEATHER_RAINNING_ID && frameFadingCount == 0)) {
            Rectangle temRect = {particle_pos[i].x, particle_pos[i].y, 5, 25};
            DrawRectanglePro(temRect, RAINDROPORIGIN, 135, (Color){68, 75, 202, fadingParticle});
        }
    }
}

void updateSummonSnowflake() {
    if (old_weather_id == WEATHER_SNOWING_ID && frameFadingCount != 0) {
        fadingParticle -= 5;
    }
    for (int i = 0; i < 50; i++) {
        if ((particle_pos[i].z >= 30 && 
            (current_weather_id == WEATHER_SNOWING_ID) && frameFadingCount == 0)) {
            float snow_y = getCloudY();
            particle_pos[i].x = randomNumber(0, 1600);
            particle_pos[i].y = randomNumber(min(snow_y, 0), min(snow_y, 0)-700);
            particle_pos[i].z = 0;
        }
        if ((particle_pos[i].y > 1000 || particle_pos[i].x > 1600) && 
            ((old_weather_id == WEATHER_SNOWING_ID && frameFadingCount != 0) || (current_weather_id == WEATHER_SNOWING_ID && frameFadingCount == 0))) {
            particle_pos[i].z = 30;
        } if ((particle_pos[i].z == 0) && 
                    ((old_weather_id == WEATHER_SNOWING_ID && frameFadingCount != 0) || (current_weather_id == WEATHER_SNOWING_ID && frameFadingCount == 0))) {
            particle_pos[i].x += randomNumber(-5, 5);
            particle_pos[i].y += 7;
        }
        if (old_weather_id == WEATHER_SNOWING_ID && frameFadingCount != 0) {
            if (fadingParticle <= 0) {
                particle_pos[i].z = 30;
                continue;
            }
        }
        if ((old_weather_id == WEATHER_SNOWING_ID && frameFadingCount != 0) || (current_weather_id == WEATHER_SNOWING_ID && frameFadingCount == 0)) {
            DrawTexture(TEXTURE_SNOWFLAKE, particle_pos[i].x, particle_pos[i].y, (Color){100, 255, 255, fadingParticle});
        }
    }
}