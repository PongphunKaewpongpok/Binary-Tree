#include "temperature.h"

#define NOMINMAX
#define NOGDI

// เปลื่ยนชื่อเพื่อเตรียมสำหรับลบ
#define CloseWindow WinAPICloseWindow
#define ShowCursor WinAPICShowCursor
#define DrawTextA WinAPICDrawTextA
#define LoadImageA WinAPICLoadImageA
#define DrawTextExA WinAPICDrawTextExA
#define PlaySoundA WinAPICPlaySoundA

#include <windows.h>

#undef CloseWindow  // ลบ CloseWindow จาก windows.h เพื่อไม่ให้ประกาศชนกับ raylib.h
#undef ShowCursor   // ลบ ShowCursor จาก windows.h เพื่อไม่ให้ประกาศชนกับ raylib.h
#undef DrawTextA    // ลบ DrawTextA จาก windows.h เพื่อไม่ให้ประกาศชนกับ raylib.h
#undef LoadImageA   // ลบ LoadImageA จาก windows.h เพื่อไม่ให้ประกาศชนกับ raylib.h
#undef DrawTextExA  // ลบ DrawTextExA จาก windows.h เพื่อไม่ให้ประกาศชนกับ raylib.h
#undef PlaySoundA   // ลบ PlaySoundA จาก windows.h เพื่อไม่ให้ประกาศชนกับ raylib.h
#ifdef Rectangle
#undef Rectangle    // ลบ Rectangle จาก windows.h เพื่อไม่ให้ประกาศชนกับ raylib.h
#endif

#include <stdio.h>
#include <stdbool.h>

static HANDLE hSerial;
static DCB dcbSerialParams = {0};
static COMMTIMEOUTS timeouts = {0};
bool status = false;

// ฟังก์ชันเปิด Serial Communication
bool setupSerial(char *portName) {
    hSerial = CreateFile(portName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hSerial == INVALID_HANDLE_VALUE) return false;

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) return false;

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) return false;

    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) return false;
    status = true;
    return true;
}

bool getStatus() {
    return status;
}

void disableHandle() {
    CloseHandle(hSerial);
}

float getCelsius() {
    DWORD bytesRead;
    char buffer[8];
    if (!ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
        status = false;
    }
    buffer[bytesRead] = '\0'; // เติม null terminator
    return atof(buffer);      // แปลงค่าเป็น float
}