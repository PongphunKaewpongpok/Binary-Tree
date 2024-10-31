#ifndef TIME_H
#define TIME_H

#include <stdbool.h>

typedef struct {
    int minutes;
    int hours;
    int days;
} Time;

extern int gameFrame;

void timeStart();
void updateTime();
void displayDay();
Time getTime();
bool isDay();

#endif