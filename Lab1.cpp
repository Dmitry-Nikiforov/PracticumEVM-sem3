#include <stdlib.h>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

using namespace std;

int main (){
    setlocale(LC_CTYPE, "Russian");

    TCHAR inputName[]="C:\\SiPPO-ioDir\\LAB1-in.txt";
    TCHAR outputName[]="C:\\SiPPO-ioDir\\LAB1-out.txt";
    char tmpBuf[200];
    HANDLE fHandle;
    DWORD fSize;
    if (GetFileAttributes(inputName) == 0xFFFFFFFF) {
        cout << inputName << " - file not exists" << endl;
        return 0;
    }

        fHandle = CreateFile(inputName, GENERIC_READ | GENERIC_WRITE,
                             FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
        ReadFile(fHandle, &tmpBuf, sizeof(tmpBuf), &fSize, NULL);
        CloseHandle(fHandle);


    stringstream ss;
    string buf(tmpBuf);
    buf = buf.substr(0, fSize);
    buf.push_back('\r' + '\n');
    int lastStr=0;
    double tmpDig=0, Max=0;
    string fioMax;

    int end=0;
    int digPos=0;
    DWORD error=GetLastError();
    try {
        while (end < fSize + 1) {
            end = buf.find('\n', lastStr);
            digPos = buf.rfind(' ', end - 2);
            tmpDig = atof(buf.substr(digPos, end - digPos - 1).c_str());
            if (tmpDig > Max) {
                Max = tmpDig;

                fioMax = buf.substr(lastStr, digPos - lastStr);
            }
            ss << buf.substr(lastStr, digPos - lastStr + 1) << lround(tmpDig) << "\r\n";
            lastStr = end + 1;
        }

        cout<<"Max  -- "<<fioMax<<endl;
    }catch(...){
        cout<<"BAD INPUT";
        return 0;
    }


    fHandle = CreateFile(outputName, GENERIC_READ | GENERIC_WRITE,
                            FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
    WriteFile(fHandle, ss.str().c_str(), fSize - 6 , &fSize, NULL);
    CloseHandle(fHandle);


    return 0;
}