#include "basic_math.h"

#include <math.h>
#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>

char short_form[5] = {'k', 'm', 'b', 't', 'q'};

double min(double a, double b) {
    return a < b ? a : b;
}

double max(double a, double b) {
    return a > b ? a : b;
}

float getRotation(float x1, float y1, float x2, float y2) {
    return radiansToDegrees(atan2(y2 - y1, x2 - x1));
}

float radiansToDegrees(float radians) {
    return radians * (180.0 / M_PI);
}

float degreesToRadians(float degrees) {
    return degrees * (M_PI / 180.0);
}

Vector2 getMovingPoint(float x1, float y1, float rotation, float distance) {
    float radians = degreesToRadians(rotation);

    return (Vector2){x1+(distance*cos(radians)), y1+(distance*sin(radians))};
}

int randomNumber(int min, int max) {
    return rand() % (max - min + 1) + min;
}

void shortFormToken(char *text, int tokenCount) {
    int count = 0;
    float tokenCountFloat = tokenCount;
    while (tokenCountFloat/1000.0 >= 1) {
        tokenCountFloat /= 1000;
        count++;
    }
    if (count == 0) {
        sprintf(text, "%d", tokenCount);
    } else {
        sprintf(text, "%.1f%c", tokenCountFloat, short_form[count-1]);
    }
}

Vector2 getRandomPositionCircle(Vector2 pos, float radius) {
    float distance = sqrt((float)rand() / RAND_MAX) * radius;
    float angle = ((float)rand() / RAND_MAX) * 2.0f * (float)M_PI;

    Vector2 randomPos = {pos.x + distance * cos(angle), pos.y + distance * sin(angle)};
    return randomPos;
}
