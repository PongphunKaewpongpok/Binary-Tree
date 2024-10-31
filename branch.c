#include "branch.h"

#include "raylib.h"
#include <stdbool.h>
#include "stddef.h"
#include "basic_math.h"
#include "custom_raylib.h"
#include "drag_screen.h"
#include "declaration.h"
#include "time.h"
#include <stdlib.h>
#include "data.h"

#include "stdio.h"
#include "math.h"

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

int warning_count = 0;

Rectangle addButtonRect;
Rectangle deleteButtonRect;
Rectangle leaveButtonRect;
Rectangle temButton1Rect;
Rectangle temButton2Rect;

void branchStart() {
    branch_list = malloc(branch_capacity * sizeof(Branch));
    if (branch_list == NULL) {
        printf("Failed to initialize branch_list!\n");
        exit(1); // Exit the program if memory allocation fails
    }
}

void updateLeaves() {
    for (int i = branch_count-1; i >= 0; i--) {
        Vector2 center;
        if (branch_list[i].is_leaves) {
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
            // Handle memory allocation failure if necessary
            // For now, we just leave branch_list unchanged
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
}

void displayWarning() {
    
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
    } else {
        if (frameCount < 10) { // Clicked
            if (CheckCollisionPointRec(mouse_pos, addButtonRect) && selectedBranch != NULL) { // Build a Branch
                if (selectedBranch->child_first != NULL && selectedBranch->child_second != NULL) {
                    return;
                }
                if (sunlightToken < 100*selectedBranch->id && waterToken < 100*selectedBranch->id) {
                    return;
                }
                branchCreating();
            } else if (CheckCollisionPointRec(mouse_pos, leaveButtonRect) && selectedBranch != NULL) { 
                if (selectedBranch->is_leaves) {    // Remove Leaves
                    selectedBranch->is_leaves = false;
                    selectedBranch->is_flower = false;
                    selectedBranch->is_fruit = false;
                } else {    // Add Leaves
                    if (sunlightToken < 100*selectedBranch->id && waterToken < 100*selectedBranch->id) {
                        return;
                    }
                    sunlightToken -= 100*selectedBranch->id;
                    waterToken -= 100*selectedBranch->id;
                    selectedBranch->is_leaves = true;
                }
            } else if (CheckCollisionPointRec(mouse_pos, deleteButtonRect) && selectedBranch != NULL) { // Remove a Branch
                if (selectedBranch->parent != NULL) {
                    branchRemoveWarning(selectedBranch->id);
                }
            } else if (atMouseBranch != NULL) { // Selected Branch
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

    if (selectedBranch->parent != NULL) {
        temDeleteButton = &TEXTURE_BUTTON_REMOVE_BRANCH;
    } else {
        temDeleteButton = &TEXTURE_BUTTON_CANT_REMOVE_BRANCH;
    }

    if ((selectedBranch->child_first == NULL || selectedBranch->child_second == NULL)
        && (sunlightToken >= 100*selectedBranch->id && waterToken >= 100*selectedBranch->id)) {
        temAddButton = &TEXTURE_BUTTON_ADD_BRANCH;
    } else {
        temAddButton = &TEXTURE_BUTTON_CANT_ADD_BRANCH;
    }

    if (!selectedBranch->is_leaves) {
        if (sunlightToken >= 100*selectedBranch->id && waterToken >= 100*selectedBranch->id) {
            temLeaveButton = &TEXTURE_BUTTON_ADD_LEAVE;
        } else {
            temLeaveButton = &TEXTURE_BUTTON_CANT_ADD_LEAVE;
        }
    } else {
        temLeaveButton = &TEXTURE_BUTTON_REMOVE_LEAVE;
    }

    if (!selectedBranch->is_flower && selectedBranch->is_leaves) {
        if (sunlightToken >= 100*selectedBranch->id && waterToken >= 100*selectedBranch->id) {
            temButton1 = &TEXTURE_BUTTON_ADD_FLOWER;
        } else {
            temButton1 = &TEXTURE_BUTTON_CANT_ADD_FLOWER;
        }
    } else if (selectedBranch->is_flower) {
        temButton1 = &TEXTURE_BUTTON_REMOVE_FLOWER;
    }

    if (selectedBranch->is_flower) {
        if (sunlightToken >= 100*selectedBranch->id && waterToken >= 100*selectedBranch->id) {
            temButton2 = &TEXTURE_BUTTON_ADD_FRUIT;
        } else {
            temButton2 = &TEXTURE_BUTTON_CANT_ADD_FRUIT;
        }
    } else if (selectedBranch->is_fruit) {
        temButton2 = &TEXTURE_BUTTON_REMOVE_FRUIT;
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
    // Shadow
    DrawText(sunlightText, 450-10*strlen(sunlightText)+4, 21, 40, BLACK);
    DrawText(waterText, 800-10*strlen(waterText)+4, 21, 40, BLACK);
    DrawText(applesText, 1150-10*strlen(applesText)+4, 21, 40, BLACK);

    // Main
    DrawText(sunlightText, 450-10*strlen(sunlightText), 17, 40, YELLOW);
    DrawText(waterText, 800-10*strlen(waterText), 17, 40, (Color){135, 255, 255, 255});
    DrawText(applesText, 1150-10*strlen(applesText), 17, 40, RED);
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