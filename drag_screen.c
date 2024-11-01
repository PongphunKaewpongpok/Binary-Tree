#include "drag_screen.h"

#include "raylib.h"
#include "declaration.h"
#include <stdbool.h>
#include "branch.h"
#include "basic_math.h"

bool drag_screen = false;

int drag_frame_count = 0;

Vector2 drag_init_pos = {0, 0};
Vector2 ground_1_pos = {-800, 700};
Vector2 ground_2_pos = {800, 700};
Vector2 cloud_1_pos = {-800, -850};
Vector2 cloud_2_pos = {800, -850};

Vector2 screen_pos = {0, 0};

void drag_screen_cal() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            drag_init_pos.x = mouse_pos.x;
            drag_init_pos.y = mouse_pos.y;
            drag_screen = true;
    } if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        branchSelecting(drag_frame_count);
        drag_init_pos.x = 0;
        drag_init_pos.y = 0;
        drag_screen = false;
        drag_frame_count = 0;
    }

    if (drag_screen) {
        drag_frame_count++;
        double temX = mouse_pos.x - drag_init_pos.x;
        double temY = mouse_pos.y - drag_init_pos.y;
        ground_moving(temX, temY);
        cloud_moving(temX, temY);
        update_screen_pos(temX, temY);
        drag_init_pos.x = mouse_pos.x;
        drag_init_pos.y = mouse_pos.y;
    }
}

void drawing_map() {
    if (ground_1_pos.y <= 900) {
        DrawTexture(TEXTURE_GROUND, ground_1_pos.x, ground_1_pos.y, WHITE);
        DrawTexture(TEXTURE_GROUND, ground_2_pos.x, ground_2_pos.y, WHITE);
    }
    DrawTexture(TEXTURE_CLOUD, cloud_1_pos.x, min(cloud_1_pos.y, 0), WHITE);
    DrawTexture(TEXTURE_CLOUD, cloud_2_pos.x, min(cloud_2_pos.y, 0), WHITE);
}

void ground_moving(double temX, double temY) {
    if (drag_screen) {
        ground_1_pos.x += temX; ground_1_pos.y += temY;
        ground_2_pos.x += temX; ground_2_pos.y += temY;
        if (ground_1_pos.y < 500) {
            ground_1_pos.y = 500;
            ground_2_pos.y = 500;
        }
        if (ground_1_pos.x > 1600) {
            ground_1_pos.x -= 3200;
        } else if (ground_1_pos.x < -1600) {
            ground_1_pos.x += 3200;
        }
        if (ground_2_pos.x > 1600) {
            ground_2_pos.x -= 3200;
        } else if (ground_2_pos.x < -1600) {
            ground_2_pos.x += 3200;
        }
    }
}

void cloud_moving(double temX, double temY) {
    temX /= 10; temY /= 10;
    if (drag_screen) {
        cloud_1_pos.x += temX; cloud_1_pos.y += temY;
        cloud_2_pos.x += temX; cloud_2_pos.y += temY;
        if (cloud_1_pos.y < -850) {
            cloud_1_pos.y = -850;
            cloud_2_pos.y = -850;
        }
        if (cloud_1_pos.x > 1600) {
            cloud_1_pos.x -= 3200;
        } else if (cloud_1_pos.x < -1600) {
            cloud_1_pos.x += 3200;
        }
        if (cloud_2_pos.x > 1600) {
            cloud_2_pos.x -= 3200;
        } else if (cloud_2_pos.x < -1600) {
            cloud_2_pos.x += 3200;
        }
    }
}

void update_screen_pos(double temX, double temY) {
    screen_pos.x += temX;
    screen_pos.y += temY;
    if (screen_pos.y < -200) {
        screen_pos.y = -200;
    }
}

float getGroundY() {
    return ground_1_pos.y;
}

float getCloudY() {
    return cloud_1_pos.y;
}
