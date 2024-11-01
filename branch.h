#ifndef BRANCH_H
#define BRANCH_H

#include "raylib.h" // Needed for [Vector2] type
#include <stdbool.h> // Needed for [bool] type

typedef struct {
    short animal_max;
} Building;

typedef struct {
    int id;
    Vector2 pos;
    bool collectable;
} Product;

struct Branch;
typedef struct Branch {
    int id;

    Vector2 pos;

    struct Branch* parent;
    struct Branch* child_first;
    struct Branch* child_second;

    bool is_leaves;
    bool is_flower;
    bool is_fruit;

    struct Building* type_building;

    short animal_count;
    
    double size;

    float rotation;
} Branch;

extern Branch* selectedBranch;

extern Branch* branch_list;
extern int branch_count;

extern Product* product_list;
extern int product_count;

extern int health;

void displayWarning();

void branchStart();

void updateBranchCreating();    // Updates branch that are during the creating
void updateBranch();            // Updates branch that already exist
void updateBranchMenu();        // Updates branch menu when selected
void updateLeaves();            // Updates branch leaves
void updateProducts();          // Updates branch flower and fruit    
void updateCollectApple();

void addBranchSize(float size);
void branchAdd(Vector2 pos, float rotation, Branch* parent); // Build a branch after select the rotation
void branchRemoveWarning(int id);                            /* Show how many branch will be disappear from remove this branch and
                                                                player must click comfirm*/
void branchRemoveConfirm(int id);                           // Remove a that branch and it child
void branchSortID();                                        // Generating new ID after delete branch and clear old branch ID
void branchCreating();                                      // Rotate the branch
void branchCreatingCancel();                                // Cancel creating branch mode
void branchSelecting(int frameCount);                       // Click to select branch
int branchCounting(int id);                                  // For counting branch itself and descendants

void buildLeaves(int id);
void buildFlower(int id);
void buildBuilding(int id);

void productRemove(int id);
void productAdd(int id);

void backToTree();
void displayBackToTree();

void health_cal();
void displayHealth();

#endif
