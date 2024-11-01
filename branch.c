#include "branch.h"

#include "raylib.h"
#include <stdbool.h>
#include <stddef.h>
#include "basic_math.h"
#include "custom_raylib.h"
#include "drag_screen.h"
#include "declaration.h"
#include "time.h"
#include <stdlib.h>
#include "data.h"
#include <string.h>
#include "sound.h"
#include "weather.h"
#include "event.h"

#include <stdio.h>
#include <math.h>

const int BRANCHWIDTH = 40;
const int BRANCHHEIGHT = 100;
const int BUTTONWIDTH = 50;
const int BUTTONHEIGHT = 50;

const Rectangle BRANCHRECT = {0, 0, BRANCHWIDTH, BRANCHHEIGHT};
const Rectangle BUTTONRECT = {0, 0, BUTTONWIDTH, BUTTONHEIGHT};

Branch* selectedBranch = NULL;
Branch* atMouseBranch = NULL;

bool isBranchCreating = false;
float branchCreatingRotation = 0.0f;

Branch* branch_list;
int branch_count = 0;
int branch_capacity = 0;

Product* product_list;
int product_count = 0;
int product_capacity = 0;

char warningText[40];
int warning_count = 0;

bool back_to_tree = false;

int health = 50;
int max_health = 50;

Rectangle addButtonRect;
Rectangle deleteButtonRect;
Rectangle leaveButtonRect;
Rectangle temButton1Rect;
Rectangle temButton2Rect;

void health_cal() {
    max_health = branch_count*50;
    if (health < max_health) {
        int temHP = min(max_health-health, applesToken);
        applesToken -= temHP;
        health += temHP;
        if (temHP != 0) {
            PlaySound(COLLECT_APPLE);
        }
    }
}

void waterTokenCal() {
    if (flood_left != 0) {
        waterToken = flood_left;
    }
}

void displayHealth() {
    DrawRectangle(450, 845, 700, 40, BLACK);
    DrawRectangle(450, 845, 700*health/max_health, 40, RED);
    char healthText[7];
    char maxHealthText[7];
    shortFormToken(healthText, health);
    shortFormToken(maxHealthText, max_health);

    DrawText(healthText, 755-MeasureText(healthText, 30), 851, 30, WHITE);
    DrawText("/", 800-MeasureText("/", 30), 851, 30, WHITE);
    DrawText(maxHealthText, 850-MeasureText(maxHealthText, 30), 851, 30, WHITE);
}

void branchStart() {
    branch_list = malloc(branch_capacity * sizeof(Branch));
    if (branch_list == NULL) {
        printf("Failed to initialize branch_list!\n");
        exit(1);
    }

    product_list = malloc(product_capacity * sizeof(Product));
    if (product_list == NULL) {
        printf("Failed to initialize product_list!\n");
        exit(1);
    }
}

void updateLeaves() {
    for (int i = branch_count-1; i >= 0; i--) {
        Vector2 center;
        if (branch_list[i].is_leaves) {
            if (randomNumber(0, 10) == 1 && isDay() && current_weather_id == WEATHER_SUNNY_ID) {
                sunlightToken++;
            }
            if (randomNumber(0, 100) == 1 && !isDay() && current_weather_id == WEATHER_SUNNY_ID) {
                waterToken++;
            }
            center = getMovingPoint(branch_list[i].pos.x+screen_pos.x,
                                    branch_list[i].pos.y+screen_pos.y,
                                    branch_list[i].rotation+90,
                                    BRANCHHEIGHT*branch_list[i].size);
            if (!(center.x-100*branch_list[i].size     > SCREENWIDTH   ||  // Right
                  center.x+100*branch_list[i].size     < -SCREENWIDTH  ||  // Left
                  center.y-100*branch_list[i].size     > SCREENHEIGHT  ||  // Down
                  center.y+100*branch_list[i].size     < -SCREENHEIGHT))   // Up
            {
                DrawCircleV(center, 100*branch_list[i].size, (Color){0, 70+5*branch_list[i].id%20, 0, 255});
            }
        }
    }
}

void updateProducts() {
    for (int i = product_count-1; i >= 0; i--) {
        Vector2 center = {product_list[i].pos.x+screen_pos.x, product_list[i].pos.y+screen_pos.y};
        if (branch_list[product_list[i].id].is_flower) {
            DrawTexturePro(
                TEXTURE_FLOWER,
                (Rectangle){0, 0, 50, 50},
                (Rectangle){center.x, center.y, 50, 50},
                (Vector2){25, 25},
                0.0f,
                WHITE
            );
        }
        if (branch_list[product_list[i].id].is_fruit && product_list[i].collectable) {
            Rectangle checkRect = {center.x-25, center.y-25, 50, 50};
            DrawTexturePro(
                TEXTURE_APPLE_TOKEN, 
                (Rectangle){0, 0, 50, 50},
                (Rectangle){center.x, center.y, 50, 50},
                (Vector2){25, 25},
                0.0f,
                !(CheckCollisionPointRec(mouse_pos, checkRect)) ? WHITE : (Color){160, 255, 255, 255}
            );
        }
        if (branch_list[product_list[i].id].is_fruit && !product_list[i].collectable) {
            if (randomNumber(1, 1000) == 1) {
                product_list[i].collectable = true;
            }
        }
    }
}

void updateCollectApple() {
    for (int i = product_count-1; i >= 0; i--) {
        Rectangle temRect = {product_list[i].pos.x+screen_pos.x-25, product_list[i].pos.y+screen_pos.y-25, 50, 50};
        if (CheckCollisionPointRec(mouse_pos, temRect) && product_list[i].collectable) {
            product_list[i].collectable = false;
            PlaySound(COLLECT_APPLE);
            applesToken += 1;
        }
    }
}

void updateBranch() {
    atMouseBranch = NULL;

    updateLeaves();

    if (selectedBranch != NULL) {
        if (isBranchCreating) {
            updateBranchCreating();
        }
    }

    for (int i = branch_count-1; i >= 0; i--) {
        Vector2 temOrigin = {BRANCHWIDTH/2*branch_list[i].size, 0};

        if (branch_list[i].pos.x+screen_pos.x-100       > SCREENWIDTH   ||  // Right
            branch_list[i].pos.x+screen_pos.x+100       < -SCREENWIDTH  ||  // Left
            branch_list[i].pos.y+screen_pos.y-100       > SCREENHEIGHT  ||  // Down
            branch_list[i].pos.y+screen_pos.y+100       < -SCREENHEIGHT)    // Up
        {
            continue;
        }

        Rectangle temRect = {branch_list[i].pos.x+screen_pos.x,
                            branch_list[i].pos.y+screen_pos.y,
                            BRANCHWIDTH*branch_list[i].size,
                            BRANCHHEIGHT*branch_list[i].size};
        Rectangle checkRect = {branch_list[i].pos.x+screen_pos.x-temOrigin.x,
                            branch_list[i].pos.y+screen_pos.y-temOrigin.y,
                            BRANCHWIDTH*branch_list[i].size,
                            BRANCHHEIGHT*branch_list[i].size};

        Color temColor = WHITE;
        Texture2D* temBranchTexture = &TEXTURE_BRANCH;
        if (selectedBranch != NULL) {
            if (selectedBranch->id == branch_list[i].id) {
                if (!isBranchCreating) {
                    temColor = (Color){255, 255, 0, gameFrame > 30 ? 255-(gameFrame*1.5) : 255-(88.5-gameFrame*1.5)};
                    temBranchTexture = &TEXTURE_BRANCH_SELECTED;
                }
            }
        } else if (atMouseBranch == NULL) {
            if (!isBranchCreating) {
                if (CheckCollisionPointRectangleRotated(mouse_pos, checkRect, temOrigin, branch_list[i].rotation)) {
                    temColor = (Color){140, 255, 255, gameFrame > 30 ? 255-(gameFrame*1.5) : 255-(88.5-gameFrame*1.5)};
                    atMouseBranch = &branch_list[i];
                }
            }
        }

        DrawTexturePro(
            *temBranchTexture,
            BRANCHRECT,
            temRect,
            temOrigin,
            branch_list[i].rotation,
            temColor
        );
    }

    if (selectedBranch != NULL) {
        if (!isBranchCreating) {
            updateBranchMenu();
        }
    }

    updateProducts();

    if (warning_count != 0) {
        displayWarning(); // Warning Deleted Branch
    }
}

void addBranchSize(float size) {
    for (int i = branch_count-1; i >= 0; i--) {
        branch_list[i].size += size;
    }
}

void branchAdd(Vector2 pos, float rotation, Branch* parent) {
    if (branch_count >= branch_capacity) {
        branch_capacity += 10;
        Branch* new_list = realloc(branch_list, branch_capacity*sizeof(Branch));
        if (new_list != NULL) {
            branch_list = new_list;
        } else {
            printf("Memory allocation failed for branch_list!\n");
        }
    }
    // Declaration temBranch and data
    rotation -= 180;

    Branch temBranch;
    temBranch.id = branch_count;

    temBranch.pos = pos;

    temBranch.parent = parent;
    temBranch.child_first = NULL;
    temBranch.child_second = NULL;

    temBranch.is_leaves = false;
    temBranch.is_flower = false;
    temBranch.is_fruit = false;

    temBranch.type_building = NULL;

    temBranch.animal_count = 0;

    temBranch.rotation = rotation;

    temBranch.size = 1;
    
    // Add to branch_list
    branch_list[branch_count] = temBranch;

    if (parent != NULL) {
        if (parent->child_first == NULL) {
            parent->child_first = &branch_list[branch_count];
        } else if (parent->child_second == NULL) {
            parent->child_second = &branch_list[branch_count];
        }
    }

    branch_count++;
}

void branchRemoveConfirm(int id) {
    if (branch_list[id].parent != NULL) {
        if (branch_list[id].parent->child_first == &branch_list[id]) {
            branch_list[id].parent->child_first = NULL;
        } else if (branch_list[id].parent->child_second == &branch_list[id]) {
            branch_list[id].parent->child_second = NULL;
        }
    }
    if (branch_list[id].child_first != NULL) {
        branchRemoveConfirm(branch_list[id].child_first->id);
    }
    if (branch_list[id].child_second != NULL) {
        branchRemoveConfirm(branch_list[id].child_second->id);
    }
    for (int i = id; i < branch_count - 1; i++) {
        branch_list[i] = branch_list[i + 1];
    }
    branch_count--;
}

void branchRemoveWarning(int id) {
    warning_count = branchCounting(id);
    sprintf(warningText, "This action will remove %d branches.", warning_count);
}

void displayWarning() {
    DrawTexture(TEXTURE_WARNING_BACKGROUND, 400, 250, WHITE);
    // Shadow
    DrawText("Are you sure?", 1100-MeasureText("Are you sure?", 80)+4, 354, 80, BLACK);
    DrawText(warningText    , 1050-MeasureText(warningText, 30)+4, 494, 30, BLACK);

    // Main
    DrawText("Are you sure?", 1100-MeasureText("Are you sure?", 80), 350, 80, RED);
    DrawText(warningText    , 1050-MeasureText(warningText, 30), 490, 30, GRAY);

    DrawTexture(TEXTURE_BUTTON_YES_BUTTON, 600, 600, WHITE);
    DrawTexture(TEXTURE_BUTTON_NO_BUTTON, 900, 600, WHITE);
}

void productAdd(int id) {
    for (int i = 0; i < 5; i++) {
        if (product_count >= product_capacity) {
            product_capacity += 10;
            Product* new_list = realloc(product_list, product_capacity*sizeof(Product));
            if (new_list != NULL) {
                product_list = new_list;
            } else {
                printf("Memory allocation failed for product_list!\n");
            }
        }
        Vector2 headPos = getMovingPoint(branch_list[id].pos.x, branch_list[id].pos.y,
                                    branch_list[id].rotation+90, BRANCHHEIGHT*branch_list[id].size);
        Vector2 temPos = getRandomPositionCircle(headPos, 100);
        Product temProduct;

        temProduct.id = id;
        temProduct.pos = temPos;
        temProduct.collectable = false;

        product_list[product_count] = temProduct;
        product_count++;
    }
}

void productRemove(int id) {
    for (int i = product_count-1; i >= 0; i--) {
        if (product_list[i].id == id) {
            product_count--;
            for (int i = id; i < product_count-1; i++) {
                product_list[i] = product_list[i + 1];
            }
        }
    }
}

int branchCounting(int id) { // For counting branch itself and descendants
    int count = 1;
    if (branch_list[id].child_first != NULL) {
        count += branchCounting(branch_list[id].child_first->id);
    }
    if (branch_list[id].child_second != NULL) {
        count += branchCounting(branch_list[id].child_second->id);
    }
    return count;
}

void branchSelecting(int frameCount) {
    Rectangle backToTreeRect = {SCREENWIDTH-125, SCREENHEIGHT-125, 100, 100};

    if (isBranchCreating) {
        if (frameCount < 10) {
            Vector2 parentHeadPos = getMovingPoint(selectedBranch->pos.x, selectedBranch->pos.y,
                                                    selectedBranch->rotation+90, BRANCHHEIGHT*selectedBranch->size);

            branchAdd(parentHeadPos, branchCreatingRotation, selectedBranch);
            branchCreatingCancel();
            branchCreatingRotation = 0.0f;
            sunlightToken -= 100*selectedBranch->id;
            waterToken -= 100*selectedBranch->id;
        }
    } else if (back_to_tree) {
        if (CheckCollisionPointRec(mouse_pos, backToTreeRect) && frameCount < 10) {    // Build a Branch
            back_to_tree = false;
        }
    } else {
        if (frameCount < 10) { // Clicked
            updateCollectApple();

            Rectangle yesButtonRect = {600, 600, 100, 100};
            Rectangle noButtonRect = {900, 600, 100, 100};
            if (warning_count != 0) {
                if (CheckCollisionPointRec(mouse_pos, yesButtonRect) && selectedBranch != NULL) {                                     // Confirm Remove Branch
                    branchRemoveConfirm(selectedBranch->id);
                    selectedBranch = NULL;
                    warning_count = 0;
                } else if (CheckCollisionPointRec(mouse_pos, noButtonRect)  && selectedBranch != NULL) {                               // Cancel Remove Branch
                    warning_count = 0;
                }
            } else if (CheckCollisionPointRec(mouse_pos, backToTreeRect)) {    // Build a Branch
                back_to_tree = true;
            } else if (CheckCollisionPointRec(mouse_pos, addButtonRect) && selectedBranch != NULL) {    // Build a Branch
                if (selectedBranch->child_first != NULL && selectedBranch->child_second != NULL) {
                    return;
                }
                if (sunlightToken < 100*selectedBranch->id && waterToken < 100*selectedBranch->id) {
                    return;
                }
                branchCreating();
            } else if (CheckCollisionPointRec(mouse_pos, leaveButtonRect) && selectedBranch != NULL) { 
                if (selectedBranch->is_leaves) {                                                        // Remove Leaves
                    selectedBranch->is_leaves = false;
                    selectedBranch->is_flower = false;
                    selectedBranch->is_fruit = false;
                    productRemove(selectedBranch->id);
                } else {                                                                                // Add Leaves
                    if (sunlightToken < 100*selectedBranch->id && waterToken < 100*selectedBranch->id) {
                        return;
                    }
                    sunlightToken -= 100*selectedBranch->id;
                    waterToken -= 100*selectedBranch->id;
                    selectedBranch->is_leaves = true;
                }
            } else if (CheckCollisionPointRec(mouse_pos, temButton1Rect) && selectedBranch != NULL) {   
                if (selectedBranch->is_flower) {                                                        // Remove a Flower
                    selectedBranch->is_flower = false;
                    productRemove(selectedBranch->id);
                } else if (selectedBranch->is_fruit) {                                                  // Remove a Fruit
                    selectedBranch->is_fruit = false;
                    productRemove(selectedBranch->id);
                } else if (!selectedBranch->is_flower && !selectedBranch->is_fruit) {                   // Add a Flower
                    if (sunlightToken < 100*selectedBranch->id && waterToken < 100*selectedBranch->id) {
                        return;
                    }
                    sunlightToken -= 100*selectedBranch->id;
                    waterToken -= 100*selectedBranch->id;
                    selectedBranch->is_flower = true;
                    productAdd(selectedBranch->id);
                }
            } else if (CheckCollisionPointRec(mouse_pos, temButton2Rect) && selectedBranch != NULL) {   
                if (selectedBranch->is_flower) {                                                        // Upgrade to Fruit
                    if (sunlightToken < 100*selectedBranch->id && waterToken < 100*selectedBranch->id) {
                            return;
                    }
                    selectedBranch->is_flower = false;
                    selectedBranch->is_fruit = true;
                }
            } else if (CheckCollisionPointRec(mouse_pos, deleteButtonRect) && selectedBranch != NULL) { // Remove a Branch
                if (selectedBranch->parent != NULL) {
                    branchRemoveWarning(selectedBranch->id);
                }
            } else if (atMouseBranch != NULL) {                                                         // Selected Branch
                selectedBranch = atMouseBranch;
            } else {
                selectedBranch = NULL;
            }
        }
    }
}

void updateBranchMenu() {

    Texture2D* temAddButton     = NULL;
    Texture2D* temDeleteButton  = NULL;
    Texture2D* temLeaveButton   = NULL;

    Texture2D* temButton1       = NULL; // For Upgrade || Delete
    Texture2D* temButton2       = NULL; // For Upgrade

    int price = 100*selectedBranch->id;

    if (selectedBranch->parent != NULL) {
        temDeleteButton = &TEXTURE_BUTTON_REMOVE_BRANCH;
    } else {
        temDeleteButton = &TEXTURE_BUTTON_CANT_REMOVE_BRANCH;
    }

    if ((selectedBranch->child_first == NULL || selectedBranch->child_second == NULL)
        && (sunlightToken >= price && waterToken >= price)) {
        temAddButton = &TEXTURE_BUTTON_ADD_BRANCH;
    } else {
        temAddButton = &TEXTURE_BUTTON_CANT_ADD_BRANCH;
    }

    if (!selectedBranch->is_leaves) {
        if (sunlightToken >= price && waterToken >= price) {
            temLeaveButton = &TEXTURE_BUTTON_ADD_LEAVE;
        } else {
            temLeaveButton = &TEXTURE_BUTTON_CANT_ADD_LEAVE;
        }
    } else {
        temLeaveButton = &TEXTURE_BUTTON_REMOVE_LEAVE;
    }

    if (!selectedBranch->is_flower && !selectedBranch->is_fruit && selectedBranch->is_leaves) {
        if (sunlightToken >= price && waterToken >= price) {
            temButton1 = &TEXTURE_BUTTON_ADD_FLOWER;
        } else {
            temButton1 = &TEXTURE_BUTTON_CANT_ADD_FLOWER;
        }
    } else if (selectedBranch->is_flower) {
        temButton1 = &TEXTURE_BUTTON_REMOVE_FLOWER;
    } else if (selectedBranch->is_fruit) {
        temButton1 = &TEXTURE_BUTTON_REMOVE_FRUIT;
    }

    if (selectedBranch->is_flower) {
        if (sunlightToken >= price && waterToken >= price) {
            temButton2 = &TEXTURE_BUTTON_ADD_FRUIT;
        } else {
            temButton2 = &TEXTURE_BUTTON_CANT_ADD_FRUIT;
        }
    }
    
    Vector2 headPos = getMovingPoint(selectedBranch->pos.x, selectedBranch->pos.y,
                                    selectedBranch->rotation+90, BRANCHHEIGHT*selectedBranch->size);

    headPos.x = (headPos.x+screen_pos.x)-(BRANCHWIDTH/2*selectedBranch->size);
    headPos.y = (headPos.y+screen_pos.y)-(BRANCHHEIGHT*selectedBranch->size);

    if (temAddButton != NULL && temDeleteButton != NULL && temLeaveButton != NULL) {
        if (temButton1 != NULL) {
            if (temButton2 != NULL) {
                addButtonRect       = (Rectangle){headPos.x-125, headPos.y+25, BUTTONWIDTH, BUTTONHEIGHT};
                leaveButtonRect     = (Rectangle){headPos.x-65, headPos.y+25, BUTTONWIDTH, BUTTONHEIGHT};
                temButton1Rect      = (Rectangle){headPos.x-5, headPos.y+25, BUTTONWIDTH, BUTTONHEIGHT};
                temButton2Rect      = (Rectangle){headPos.x+55, headPos.y+25, BUTTONWIDTH, BUTTONHEIGHT};
                deleteButtonRect    = (Rectangle){headPos.x+115, headPos.y+25, BUTTONWIDTH, BUTTONHEIGHT};
            } else {
                addButtonRect       = (Rectangle){headPos.x-90, headPos.y+25, BUTTONWIDTH, BUTTONHEIGHT};
                leaveButtonRect     = (Rectangle){headPos.x-30, headPos.y+25, BUTTONWIDTH, BUTTONHEIGHT};
                temButton1Rect      = (Rectangle){headPos.x+30, headPos.y+25, BUTTONWIDTH, BUTTONHEIGHT};
                deleteButtonRect    = (Rectangle){headPos.x+90, headPos.y+25, BUTTONWIDTH, BUTTONHEIGHT};
            }
        } else {
            addButtonRect       = (Rectangle){headPos.x-65, headPos.y+25, BUTTONWIDTH, BUTTONHEIGHT};
            leaveButtonRect     = (Rectangle){headPos.x-5, headPos.y+25, BUTTONWIDTH, BUTTONHEIGHT};
            deleteButtonRect    = (Rectangle){headPos.x+55, headPos.y+25, BUTTONWIDTH, BUTTONHEIGHT};
        }
    }

    DrawTexturePro( // Add Branch Button
        *temAddButton,
        BUTTONRECT,
        addButtonRect,
        (Vector2){0, 0},
        0.0f,
        !(CheckCollisionPointRec(mouse_pos, addButtonRect)) ? WHITE : (Color){150, 150, 150, 150}
    );
    DrawTexturePro( // Leave Branch Button
        *temLeaveButton,
        BUTTONRECT,
        leaveButtonRect,
        (Vector2){0, 0},
        0.0f,
        !(CheckCollisionPointRec(mouse_pos, leaveButtonRect)) ? WHITE : (Color){150, 150, 150, 150}
    );
    if (temButton1 != NULL) {
        DrawTexturePro( // temButton1 Branch Button
            *temButton1,
            BUTTONRECT,
            temButton1Rect,
            (Vector2){0, 0},
            0.0f,
            !(CheckCollisionPointRec(mouse_pos, temButton1Rect)) ? WHITE : (Color){150, 150, 150, 150}
        );
    }
    if (temButton2 != NULL) {
        DrawTexturePro( // temButton2 Branch Button
            *temButton2,
            BUTTONRECT,
            temButton2Rect,
            (Vector2){0, 0},
            0.0f,
            !(CheckCollisionPointRec(mouse_pos, temButton2Rect)) ? WHITE : (Color){150, 150, 150, 150}
        );
    }
    DrawTexturePro( // Delete Branch Button
        *temDeleteButton,
        BUTTONRECT,
        deleteButtonRect,
        (Vector2){0, 0},
        0.0f,
        !(CheckCollisionPointRec(mouse_pos, deleteButtonRect)) ? WHITE : (Color){150, 150, 150, 150}
    );
    // Price
    char priceText[6];
    shortFormToken(priceText, price);

    // Price Shadow
    DrawText(priceText, headPos.x-50-MeasureText(priceText, 40)+4, headPos.y-16, 40, BLACK);
    DrawText(priceText, headPos.x+80-MeasureText(priceText, 40)+4, headPos.y-16, 40, BLACK);

    // Price
    if (price <= sunlightToken) {
        DrawText(priceText, headPos.x-50-MeasureText(priceText, 40), headPos.y-20, 40, YELLOW);
    } else {
        DrawText(priceText, headPos.x-50-MeasureText(priceText, 40), headPos.y-20, 40, RED);
    }

    if (price <= waterToken) {
        DrawText(priceText, headPos.x+80-MeasureText(priceText, 40), headPos.y-20, 40, (Color){135, 255, 255, 255});
    } else {
        DrawText(priceText, headPos.x+80-MeasureText(priceText, 40), headPos.y-20, 40, RED);
    }

    // Token Shadow
    DrawTexture(TEXTURE_SUN_TOKEN, headPos.x-41, headPos.y-16, BLACK);
    DrawTexture(TEXTURE_WATER_TOKEN, headPos.x+88, headPos.y-23, BLACK);

    // Token
    if (price <= sunlightToken) {
        DrawTexture(TEXTURE_SUN_TOKEN, headPos.x-45, headPos.y-20, WHITE);
    } else {
        DrawTexture(TEXTURE_SUN_TOKEN, headPos.x-45, headPos.y-20, RED);
    }

    if (price <= waterToken) {
        DrawTexture(TEXTURE_WATER_TOKEN, headPos.x+84, headPos.y-27, WHITE);
    } else {
        DrawTexture(TEXTURE_WATER_TOKEN, headPos.x+84, headPos.y-27, RED);
    }
    
}

void branchCreating() {
    isBranchCreating = true;
}

void branchCreatingCancel() {
    isBranchCreating = false;
}

void updateBranchCreating() {
    if (IsKeyPressed(KEY_ESCAPE)) {
        branchCreatingCancel();
        return;
    }
    Vector2 temOrigin = (Vector2){BRANCHWIDTH/2*selectedBranch->size, 0};
    Vector2 parentHeadPos = getMovingPoint(selectedBranch->pos.x+screen_pos.x,
                                            selectedBranch->pos.y+screen_pos.y,
                                            selectedBranch->rotation+90, BRANCHHEIGHT*selectedBranch->size);
    Rectangle temRect = {parentHeadPos.x,
                        parentHeadPos.y,
                        BRANCHWIDTH*(selectedBranch->size),
                        BRANCHHEIGHT*(selectedBranch->size)};
    DrawCircleV((Vector2){temRect.x, temRect.y}, 20, RED);
    
    branchCreatingRotation = GetRotation(mouse_pos, (Vector2){temRect.x, temRect.y}, temOrigin)+90;
    DrawTexturePro(
        TEXTURE_BRANCH,
        BRANCHRECT,
        temRect,
        temOrigin,
        branchCreatingRotation-180,
        (Color){0, 255, 255, 255}
    );
}

void backToTree() {
    if (back_to_tree) {
        int temX = (screen_pos.x > 100) ? -100 : (screen_pos.x < -100) ? 100 : 0;
        int temY = (screen_pos.y > 100) ? -100 : 0;
        if (temX == 0 && temY == 0) {
            back_to_tree = false;
            return;
        }
        drag_screen = true;
        ground_moving(temX, temY);
        cloud_moving(temX, temY);
        update_screen_pos(temX, temY);
        drag_screen = false;
    }
    
}

void displayBackToTree() {
    Rectangle temRect = {SCREENWIDTH-125, SCREENHEIGHT-125, 100, 100};
    DrawTexturePro(TEXTURE_BUTTON_BACK_TO_TREE, (Rectangle){0, 0, 50, 50}, temRect, (Vector2){0, 0},
                    0.0f, !(CheckCollisionPointRec(mouse_pos, temRect)) ? WHITE : (Color){150, 150, 150, 150});
}
