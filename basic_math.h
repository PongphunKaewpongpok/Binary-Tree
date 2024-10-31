#ifndef BASIC_MATH_H
#define BASIC_MATH_H

#include "raylib.h"

double min(double a, double b);
double max(double a, double b);
float getRotation(float x1, float y1, float x2, float y2);
float radiansToDegrees(float radians);
float degreesToRadians(float degrees);
Vector2 getMovingPoint(float x1, float y1, float rotation, float distance);
int randomNumber(int min, int max);
void shortFormToken(char *text, int tokenCount);

#endif