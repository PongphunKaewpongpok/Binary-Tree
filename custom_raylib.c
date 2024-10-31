#include "custom_raylib.h"

#include "raylib.h"
#include "stdbool.h"
#include "math.h"

float GetRotation(Vector2 mouse_pos, Vector2 point, Vector2 origin) {

    float deltaX = mouse_pos.x - (point.x + origin.x);
    float deltaY = mouse_pos.y - (point.y + origin.y);
    float angle = atan2f(deltaY, deltaX) * (180.0f / PI);
    return angle;

}

bool CheckCollisionPointRectangleRotated(Vector2 point, Rectangle rect, Vector2 origin, float rotation) {

    Vector2 transformedOrigin = {rect.x + origin.x, rect.y + origin.y};

    float cosTheta = cos(-rotation * DEG2RAD);
    float sinTheta = sin(-rotation * DEG2RAD);

    Vector2 rotatedPoint = {
        cosTheta * (point.x - transformedOrigin.x) - sinTheta * (point.y - transformedOrigin.y) + transformedOrigin.x,
        sinTheta * (point.x - transformedOrigin.x) + cosTheta * (point.y - transformedOrigin.y) + transformedOrigin.y
    };

    return CheckCollisionPointRec(rotatedPoint, rect);
}

bool colorEqualsNoAlpha(Color color1, Color color2) {
    return (color1.r == color2.r && color1.g == color2.g && color1.b == color2.b);
}