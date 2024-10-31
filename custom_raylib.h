#ifndef CUSTOM_RAYLIB_H
#define CUSTOM_RAYLIB_H

#include "raylib.h"
#include "stdbool.h"

float GetRotation(Vector2 mouse_pos, Vector2 point, Vector2 origin);
bool CheckCollisionPointRectangleRotated(Vector2 point, Rectangle rect, Vector2 origin, float rotation);
bool colorEqualsNoAlpha(Color color1, Color color2);

#endif