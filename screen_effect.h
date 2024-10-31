#ifndef SCREEN_EFFECT_H
#define SCREEN_EFFECT_H

extern int fading_time;
extern int fade_alpha;
extern short fade_direction;

void update_fading();
void fade_screen();
void updateLightSource();

#endif