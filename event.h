#ifndef EVENT_H
#define EVENT_H

extern const int EVENT_ID_NONE;
extern const int EVENT_ID_FLOOD;

extern int flood_left;
extern int waterTokenSave;

void nextEventTime();
void nextEventRandom();
void updateEvent();
int getCurrentEvent();

#endif
