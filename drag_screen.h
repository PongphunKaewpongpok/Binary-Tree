#ifndef DRAG_SCREEN_H
#define DRAG_SCREEN_H

#include "raylib.h"
#include "stdbool.h"

extern bool drag_screen;
extern int drag_frame_count;

extern Vector2 drag_init_pos;

extern Vector2 ground_1_pos;
extern Vector2 ground_2_pos;
extern Vector2 cloud_1_pos;
extern Vector2 cloud_2_pos;

extern Vector2 screen_pos;

void drawing_map();
void drag_screen_cal(); // V V V  Main Function to called below function  V V V

void ground_moving(double temX, double temY); // Update ground element
void cloud_moving(double temX, double temY); // Update cloud element
void update_screen_pos(double temX, double temY); // Update screen position
float getGroundY();
float getCloudY();

#endif