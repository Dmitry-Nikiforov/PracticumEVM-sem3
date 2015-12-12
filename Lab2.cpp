#include <iostream>
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <winbase.h>
#include <sstream>
#include <iomanip>

#define NF <<setw(2)<<setfill('0')<<

using namespace std;

int main() {    setlocale(0, "");
    HANDLE hFile=NULL;
    WIN32_FILE_ATTRIBUTE_DATA fAttr;
    SYSTEMTIME sCreate,sAcces,sWrite, sTime, sysOneHr;
    FILETIME oneHr,fCreate,fAcces,fWrite;

    char tmpBuf[200];
    DWORD fSize;
    TIME_ZONE_INFORMATION currTZ;
    GetTimeZoneInformation(&currTZ);

    string fPath;
    cin>>fPath;

        try {


            //---------------------СЧИТАТЬ, ВЫВЕСТИ АТРИБУТЫ И ВРЕМЯ----------------------------
            GetFileAttributesEx(fPath.c_str(),GetFileExInfoStandard,&fAttr);
            GetSystemTime(&sTime);
            SystemTimeToTzSpecificLocalTime(&currTZ,&sTime,&sTime);
            FileTimeToSystemTime(&fAttr.ftCreationTime,&sCreate);
            FileTimeToSystemTime(&fAttr.ftLastAccessTime,&sAcces);
            FileTimeToSystemTime(&fAttr.ftLastWriteTime,&sWrite);
            SystemTimeToTzSpecificLocalTime(&currTZ,&sCreate,&sCreate);
            SystemTimeToTzSpecificLocalTime(&currTZ,&sAcces,&sAcces);
            SystemTimeToTzSpecificLocalTime(&currTZ,&sWrite,&sWrite);
            cout<<"Атрибуты :";
            if (fAttr.dwFileAttributes & FILE_ATTRIBUTE_READONLY)   cout<<" Read-only файл ";
            if (fAttr.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)   cout<<" Скрытый файл ";
            cout<<endl<<endl;

            cout<<"Время создания : " NF sCreate.wDay<<"." NF sCreate.wMonth<<"." NF sCreate.wYear
                    <<" " NF sCreate.wHour<<":" NF sCreate.wMinute<<":" NF sCreate.wSecond<<endl;
            cout<<"Время последнего обращения : " NF sAcces.wDay<<"." NF sAcces.wMonth<<"." NF sAcces.wYear
                    <<" " NF sAcces.wHour<<":" NF sAcces.wMinute<<":" NF sAcces.wSecond<<endl;
            cout<<"Время последнего изменения : " NF sWrite.wDay<<"." NF sWrite.wMonth<<"." NF sWrite.wYear
                    <<" " NF sWrite.wHour<<":" NF sWrite.wMinute<<":" NF sWrite.wSecond<<endl;

            //-------------СДЕЛАТЬ СКРЫТЫМ, СДЕЛАТЬ ЗАПИСЬ, ВЫВЕСТИ ВРЕМЯ ЗАПИСИ-------------------
            hFile=CreateFile(fPath.c_str(),GENERIC_READ | GENERIC_WRITE,
                             FILE_SHARE_WRITE | FILE_SHARE_READ,NULL,OPEN_EXISTING,NULL,NULL);

            SetFileAttributesA(fPath.c_str(),FILE_ATTRIBUTE_HIDDEN);
            cout<<"Файл скрыт"<<endl;
            ReadFile(hFile, tmpBuf, sizeof(tmpBuf), &fSize, NULL);
            stringstream ss;
            ss<<"\r\n"<<"changed at " NF sTime.wHour<<" : " NF sTime.wMinute<<" by Dmitry Nikiforov";
            if (WriteFile(hFile, ss.str().c_str(), ss.str().size(), &fSize, NULL))
                cout <<"файл "<<fPath.c_str()<<" успешно изменен"<<endl;

            CloseHandle(hFile);


            GetFileAttributesEx(fPath.c_str(),GetFileExInfoStandard,&fAttr);
            FileTimeToSystemTime(&fAttr.ftLastWriteTime,&sWrite);
            SystemTimeToTzSpecificLocalTime(&currTZ,&sWrite,&sWrite);
            cout<<"Время последнего изменения : " NF sWrite.wHour<<":" NF sWrite.wMinute<<":" NF sWrite.wSecond<<endl;


            // ------------------- ОТКАТИТЬ ВРЕМЯ ЗАПИСИ НА ЧАС НАЗАД---------------------------
            hFile=CreateFile(fPath.c_str(),GENERIC_READ | GENERIC_WRITE,
                            FILE_SHARE_WRITE | FILE_SHARE_READ,NULL,OPEN_EXISTING,NULL,NULL);

            GetFileTime(hFile,NULL,NULL,&oneHr);
            FileTimeToSystemTime(&oneHr,&sysOneHr);
            sysOneHr.wHour--;
            SystemTimeToFileTime(&sysOneHr,&oneHr);
            SetFileTime(hFile,NULL,NULL,&oneHr);
            CloseHandle(hFile);
            GetFileAttributesEx(fPath.c_str(),GetFileExInfoStandard,&fAttr);
            FileTimeToSystemTime(&fAttr.ftLastWriteTime,&sWrite);
            SystemTimeToTzSpecificLocalTime(&currTZ,&sWrite,&sWrite);
            cout<<"Время последнего изменения : " NF sWrite.wHour<<":" NF sWrite.wMinute<<":" NF sWrite.wSecond<<endl;

            //--------------------------- ДАТА СОЗДАНИЯ +1, ДОСТУПА И ЗАПИСИ -1
            hFile=CreateFile(fPath.c_str(),GENERIC_READ | GENERIC_WRITE,
                             FILE_SHARE_WRITE | FILE_SHARE_READ,NULL,OPEN_EXISTING,NULL,NULL);

            GetFileAttributesEx(fPath.c_str(),GetFileExInfoStandard,&fAttr);
            FileTimeToSystemTime(&fAttr.ftCreationTime,&sCreate);
            FileTimeToSystemTime(&fAttr.ftLastAccessTime,&sAcces);
            FileTimeToSystemTime(&fAttr.ftLastWriteTime,&sWrite);
            sCreate.wDay++;
            sAcces.wDay--;
            sWrite.wDay--;
            SystemTimeToFileTime(&sCreate,&fCreate);
            SystemTimeToFileTime(&sAcces,&fAcces);
            SystemTimeToFileTime(&sWrite,&fWrite);
            SetFileTime(hFile,&fCreate,&fAcces,&fWrite);



            GetFileAttributesEx(fPath.c_str(),GetFileExInfoStandard,&fAttr);
            FileTimeToSystemTime(&fAttr.ftCreationTime,&sCreate);
            FileTimeToSystemTime(&fAttr.ftLastAccessTime,&sAcces);
            FileTimeToSystemTime(&fAttr.ftLastWriteTime,&sWrite);
            CloseHandle(hFile);
            cout<<"Дата создания : " NF sCreate.wDay<<"." NF sCreate.wMonth<<"." NF sCreate.wYear<<endl;
            cout<<"Дата последнего обращения : " NF sAcces.wDay<<"." NF sAcces.wMonth<<"." NF sAcces.wYear<<endl;
            cout<<"Дата последнего изменения : " NF sWrite.wDay<<"." NF sWrite.wMonth<<"." NF sWrite.wYear<<endl;


        }
        catch(...){
            cout << "ERROR open" << endl;
            return 0;
        }

    return 0;
}
