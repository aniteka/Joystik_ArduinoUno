#include <iostream>
#include <Windows.h>
#include <thread>
#include <mutex>
std::mutex mtx;
#define fun F


HANDLE hSerial;
LPCTSTR sPortName = L"COM5";


void ComSetup() {
    hSerial = ::CreateFile(sPortName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hSerial == INVALID_HANDLE_VALUE)
    {
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            std::cout << "serial port does not exist.\n";
        }
        std::cout << "some other error occurred.\n";
    }

    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams))
    {
        std::cout << "getting state error\n";
    }
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if (!SetCommState(hSerial, &dcbSerialParams))
    {
        std::cout << "error setting serial port state\n";
    }
}

byte readCom() {
    DWORD iSize;
    char sReceivedChar;
    ReadFile(hSerial, &sReceivedChar, 1, &iSize, 0);
    return sReceivedChar;
}

int charToInt(char c) {
    return c - 48;
}

void KeyClick(char key) {
    keybd_event(key, 0, 0, 0);
}
void KeyUp(char key) {
    keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
}

bool AButtonState = false;
bool BButtonState = false;
bool CButtonState = false;
bool DButtonState = false;
bool AButtonIsClicked = false;
bool BButtonIsClicked = false;
bool CButtonIsClicked = false;
bool DButtonIsClicked = false;


int joystik_X = 60;
int joystik_Y = 60;
bool joystikTouch_X = false;
bool joystikTouch_Y = false;

void ButtonsSetup(char ABut, char BBut, char CBut, char DBut) {
    if (AButtonState && !AButtonIsClicked) KeyClick(ABut), AButtonIsClicked = true;      else if (!AButtonState && AButtonIsClicked) KeyUp(ABut), AButtonIsClicked = false;
    if (BButtonState && !BButtonIsClicked) KeyClick(BBut), BButtonIsClicked = true;      else if (!BButtonState && BButtonIsClicked) KeyUp(BBut), BButtonIsClicked = false;
    if (CButtonState && !CButtonIsClicked) KeyClick(CBut), CButtonIsClicked = true;      else if (!CButtonState && CButtonIsClicked) KeyUp(CBut), CButtonIsClicked = false;
    if (DButtonState && !DButtonIsClicked) KeyClick(DBut), DButtonIsClicked = true;      else if (!DButtonState && DButtonIsClicked) KeyUp(DBut), DButtonIsClicked = false;
}

void JoystikSetup(char Left, char Right, char Up, char Down) {
    if (!(joystik_X > 50 && joystik_X < 70) && !joystikTouch_X) {
        if (joystik_X < 50) KeyClick(Left);
        else if (joystik_X > 70) KeyClick(Right);
        joystikTouch_X = true;
    } else if((joystik_X > 50 && joystik_X < 70) && joystikTouch_X){
        KeyUp(Left);
        KeyUp(Right);
        joystikTouch_X = false;
    }

    if (!(joystik_Y > 50 && joystik_Y < 70) && !joystikTouch_Y) {
        if (joystik_Y < 50) KeyClick(Up);
        else if (joystik_Y > 70) KeyClick(Down);
        joystikTouch_Y = true;
    }
    else if ((joystik_Y > 50 && joystik_Y < 70) && joystikTouch_Y) {
        KeyUp(Down);
        KeyUp(Up);
        joystikTouch_Y = false;
    }
}

void ChekOnButton() {
    while (true)
    {
        if (readCom() == '\n') {
            AButtonState = charToInt(readCom());
            BButtonState = charToInt(readCom());
            CButtonState = charToInt(readCom());
            DButtonState = charToInt(readCom());

            joystik_X = charToInt(readCom()) + 48;
            joystik_Y = charToInt(readCom()) + 48;
        }
    }
}
void ButtonOnClick() {
    while (1)
    {
        //std::cout << AButtonState << " " << BButtonState << " " << CButtonState << " " << DButtonState << " " << joystik_X << " " << joystik_Y << std::endl;
        
        ButtonsSetup(VK_LEFT,'S','A',VK_RIGHT);
        JoystikSetup(VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN);
    }
}

int main() {
    ComSetup();
    std::thread th(ChekOnButton);th.detach();

    ButtonOnClick();

	return 0;
}