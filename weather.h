#ifndef WEATHER_H
#define WEATHER_H

#include "raylib.h"

extern const int WEATHER_SUNNY_ID;
extern const int WEATHER_RAINNING_ID;
extern const int WEATHER_SNOWING_ID;

extern int current_weather_id;
extern Color background_color;

void weatherStart();
void updateWeather();
void changeWeather(int weatherID);
void updateSummonRain();
void updateSummonSnowflake();

#endif