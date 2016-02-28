#include <stdlib.h>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <conio.h>
#include <stdexcept>

using namespace std;

int main()
{

    setlocale(LC_CTYPE, "Russian");

    const string pass("1234");
    string ps, inputName;
    char tmpBuf[200];
    HANDLE fHandle, fHandle2;
    DWORD fSize;
    bool editable = false;
    cout << "Путь к файлу: ";
    cin >> inputName;
    
    //не стоит париться с этипи кодами это просто для того чтобы разграничить вход в режиме редактирования и просмотра
    //для блокировки файла надо юзать LockFile() - тут у меня вообще всё не так
    cout << "Для редактирвоаания введите код доступа:" << endl;
    cin >> ps;
    if (ps==pass) {
        fHandle = CreateFile(inputName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, NULL, NULL);
        if (GetLastError()==2) {
            cout << "Не удалось найти файл" << endl;
            return 0;
        }
        else if (GetLastError()==32) {
            fHandle = CreateFile(inputName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
            if (GetLastError()==32) {
                cout << "Ошибка доступа: файл сейчас редактируется" << endl;
                getch();
                return 0;
            }
            cout << "Файл доступен только в режиме просмотра" << endl;
            cout << "--------------------------------------" << endl;
        }
        else if (GetLastError()==0) {
            cout << "Полный доступ к файлу" << endl;
            cout << "--------------------------------------" << endl;
            editable = true;
        }
    }
    else {
        fHandle = CreateFile(inputName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
        if (GetLastError()==2) {
            cout << "Не удалось найти файл" << endl;
            return 0;
        }

        if (GetLastError()==32) {
            cout << "Ошибка доступа: файл сейчас редактируется" << endl;
            getch();
            return 0;
        }
        cout << "Файл доступен в режиме просмотра" << endl;
        cout << "--------------------------------------" << endl;
    }

    ReadFile(fHandle, &tmpBuf, sizeof(tmpBuf), &fSize, NULL);

    stringstream ss;
    string buf(tmpBuf);
    buf = buf.substr(0, fSize);
    buf.push_back('\r'+'\n');
    int lastStr = 0;
    string fio, gend, sBirth, conv, signs;
    char edNum = -1;

    fio = buf.substr(0, buf.find('\r'));
    lastStr = buf.find('\n', lastStr)+1;
    gend = buf.substr(lastStr, buf.find('\r', lastStr)-lastStr);
    lastStr = buf.find('\n', lastStr)+1;
    sBirth = buf.substr(lastStr, buf.find('\r', lastStr)-lastStr);
    lastStr = buf.find('\n', lastStr)+1;
    conv = buf.substr(lastStr, buf.find('\r', lastStr)-lastStr);
    lastStr = buf.find('\n', lastStr)+1;
    signs = buf.substr(lastStr, fSize-lastStr);

    if (editable) {
        while (edNum!='0' && edNum!='-') {
            cout << "1. " << fio << endl;
            cout << "2. " << gend << endl;
            cout << "3. " << sBirth << endl;
            cout << "4. " << conv << endl;
            cout << "5. " << signs << endl;
            cout << "--------------------------------------" << endl;
            cout << "Введите цифру для редактирования соотвествующего поля" << endl;
            cout << "Введите 0 для выхода и схоранения или '-' для отмены изменений" << endl;
            cin >> edNum;
            switch (edNum) {
            case '1':
                cin >> fio;
                continue;
            case '2':
                cin >> gend;
                continue;
            case '3':
                cin >> sBirth;
                continue;
            case '4':
                cin >> conv;
                continue;
            case '5':
                cin >> signs;
                continue;
            }
        }

    }
    else {
        cout << fio << endl;
        cout << gend << endl;
        cout << sBirth << endl;
        cout << conv << endl;
        cout << signs << endl;
        cout << "Для выхода нажмите любую клавишу" << endl;
    }

    if (edNum=='-') {
        cout << "Изменения не были сохранены" << endl;
        CloseHandle(fHandle);
        return 0;
    }
    if (edNum=='0') {
        ss << fio << "\r\n";
        ss << gend << "\r\n";
        ss << sBirth << "\r\n";
        ss << conv << "\r\n";
        ss << signs;
        cout << "Изменения сохранены" << endl;
        CloseHandle(fHandle);
        fHandle2 = CreateFile(inputName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, NULL, NULL);
        WriteFile(fHandle2, ss.str().c_str(), ss.str().length(), &fSize, NULL);
        CloseHandle(fHandle2);
    }

    system("pause");
    CloseHandle(fHandle);
    return 0;
}
