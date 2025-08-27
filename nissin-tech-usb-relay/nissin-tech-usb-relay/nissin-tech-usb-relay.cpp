// nissin-tech-usb-relay.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//
// 日新テクニカ社製 - USBリレー制御 サンプル
// 
// ［使い方］
// 
// ※ONにする場合
//  > nissin-tech-usb-relay.exe COM1 1
// 
// ※OFFにする場合
//  > nissin-tech-usb-relay.exe COM1 0
// 
// ［参考］http://www.nissin-tech.com/2012/03/usb-2.html
// ［参考］https://monitorgazer.blogspot.com/2013/06/carduinoled.html

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

HANDLE hRelay;
bool Ret;

int main(int argc, char const* argv[])
{
    char portname[64] = {};

    BYTE data = 0;
    if (2 < argc)
    {
        // ポート番号
        memcpy(portname, argv[1], strlen(argv[1]));

        // ON/OFF
        if (atoi(argv[2]) == 1)
        {
            // ON
            printf("ONにします\n");
            data = 1;
        }
        else
        {
            // OFF
            printf("OFFにします\n");
            data = 0;
        }
    }   

    hRelay = CreateFileA(
        portname, 
        GENERIC_WRITE, 
        0, 
        NULL, 
        OPEN_EXISTING, 
        FILE_ATTRIBUTE_NORMAL, 
        NULL);
    
    if (hRelay == INVALID_HANDLE_VALUE) 
    {
        printf("PORT COULD NOT OPEN\n");
        exit(0);
    }
    
    Ret = SetupComm(
        hRelay, 
        1024, 
        1024);
    if (!Ret) 
    {
        printf("SET UP FAILED\n");
        CloseHandle(hRelay);
        exit(0);
    }
    Ret = PurgeComm(
        hRelay, 
        PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
    if (!Ret) 
    {
        printf("CLEAR FAILED\n");
        CloseHandle(hRelay);
        exit(0);
    }
    
    DCB dcb;
    GetCommState(hRelay, &dcb);
    dcb.DCBlength = sizeof(DCB);
    dcb.BaudRate = 9600;
    dcb.fBinary = TRUE;
    dcb.ByteSize = 8;
    dcb.fParity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;

    Ret = SetCommState(hRelay, &dcb);
    if (!Ret) 
    {
        printf("SetCommState FAILED\n");
        CloseHandle(hRelay);
        exit(0);
    }
    
    DWORD dwSendSize;
    DWORD dwErrorMask;

    Ret = WriteFile(
        hRelay, 
        &data, 
        sizeof(data), 
        &dwSendSize, 
        NULL);
    if (!Ret) 
    {
        printf("SEND FAILED\n");
        CloseHandle(hRelay);
        exit(0);
    }
    printf("FINISH\n");
    CloseHandle(hRelay);
}