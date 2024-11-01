#ifndef SOUND_H
#define SOUND_H

#include "raylib.h"

extern Music BGM_GARDEN_DREAMS;
extern Music BGM_GARDEN_OF_FROST;
extern Music BGM_MIDNIGHT_GARDEN;
extern Music BGM_WHISPERS_OF_THE_RAIN;

extern Sound COLLECT_APPLE;

void initMusic();
void unloadAllMusic();
void updateMusic();

#endif
