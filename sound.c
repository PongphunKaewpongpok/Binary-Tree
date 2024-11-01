#include "sound.h"

#include "raylib.h"
#include "weather.h"
#include "time.h"
#include <stdlib.h>
#include <stdio.h>

// Sound
Music BGM_GARDEN_DREAMS;
Music BGM_WHISPERS_OF_THE_RAIN;
Music BGM_GARDEN_OF_FROST;
Music BGM_MIDNIGHT_GARDEN;

Sound COLLECT_APPLE;

Music *current_playing_bgm = NULL;
float current_volume = 0;

void initMusic() {
    BGM_GARDEN_DREAMS           = LoadMusicStream("Sounds/Garden_Dreams.mp3");
    BGM_WHISPERS_OF_THE_RAIN    = LoadMusicStream("Sounds/Whispers_of_the_Rain.mp3");
    BGM_GARDEN_OF_FROST         = LoadMusicStream("Sounds/Garden_of_Frost.mp3");
    BGM_MIDNIGHT_GARDEN         = LoadMusicStream("Sounds/Midnight_Garden.mp3");
    COLLECT_APPLE               = LoadSound("Sounds/collect_apple.mp3");

    SetSoundVolume(COLLECT_APPLE, 0.5);
}

void unloadAllMusic() {
    UnloadMusicStream(BGM_GARDEN_DREAMS);
    UnloadMusicStream(BGM_WHISPERS_OF_THE_RAIN);
    UnloadMusicStream(BGM_GARDEN_OF_FROST);
    UnloadMusicStream(BGM_MIDNIGHT_GARDEN);
    UnloadSound(COLLECT_APPLE);
}

void updateMusic() {
    if (current_playing_bgm != NULL) {
        UpdateMusicStream(*current_playing_bgm);
        SetMusicVolume(*current_playing_bgm, current_volume);
    }
    if (current_volume <= 0) {
        if (current_playing_bgm != NULL) {
            StopMusicStream(*current_playing_bgm);
        }
        if (current_weather_id == WEATHER_SUNNY_ID) {
            current_playing_bgm = &BGM_GARDEN_DREAMS;
        } else if (current_weather_id == WEATHER_RAINNING_ID) {
            current_playing_bgm = &BGM_WHISPERS_OF_THE_RAIN;
        } else if (current_weather_id == WEATHER_SNOWING_ID) {
            current_playing_bgm = &BGM_GARDEN_OF_FROST;
        }
        PlayMusicStream(*current_playing_bgm);
    }
    if ((current_weather_id == WEATHER_SUNNY_ID && !IsMusicStreamPlaying(BGM_GARDEN_DREAMS))           ||
        (current_weather_id == WEATHER_RAINNING_ID && !IsMusicStreamPlaying(BGM_WHISPERS_OF_THE_RAIN)) ||
        (current_weather_id == WEATHER_SNOWING_ID && !IsMusicStreamPlaying(BGM_GARDEN_OF_FROST)))
    {
        current_volume -= 0.01;
    } else if (current_volume < 0.5) {
        current_volume += 0.01;
    }
}
