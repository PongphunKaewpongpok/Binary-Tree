#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <stdbool.h>

bool setupSerial(char *portName);
bool startArduinoCommunication();
void disableHandle();

bool getStatus();
float getCelsius();
void displayTemperature();

#endif