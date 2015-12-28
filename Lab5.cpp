#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <iomanip>
#include <vector>
using namespace std;

struct section_header {
  char owner[15];
  unsigned int type;
  unsigned int section_offset;
  unsigned int section_size;
};

struct file_header {
  unsigned int sig;
  unsigned int n_sections;
  unsigned int offset;
};

int main()
{
    setlocale(LC_ALL, "Russian");
    HANDLE hFile, hFileMapping;
    string fileName;
    BYTE* bf;
    file_header* file_header1;
    vector<section_header*> section_headers;
    vector<string> section;
    char* sectionPtr;

    cout << "Путь к файлу: ";
    cin >> fileName;
    try {
        hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
        if (GetLastError()!=0) throw "Bad file";
        hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
        bf = (BYTE*) MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, 0);
        file_header1 = (file_header*) &bf[0];
        for (int i = 0; i<file_header1->n_sections; i++) {
            section_headers.push_back((section_header*) &bf[file_header1->offset+i*sizeof(section_header)]);

        }
        sectionPtr = (char*) &bf[section_headers[0]->section_offset];
        string allSections(sectionPtr);
        for (int i = 0; i<file_header1->n_sections; i++) {
            section.push_back(allSections.substr(section_headers[i]->section_offset-section_headers[0]->section_offset,
                    section_headers[i]->section_size));

        }
    }
    catch (...) {
        cout << "Ошибка чтения файла";
        return 0;
    }
    cout << "введите номер секции" << endl;
    int n;
    cin >> n;
    cout << "Заголовок cекции 1: " << endl;
    cout << "owner: " << section_headers[n-1]->owner << endl;
    cout << "type: " << section_headers[n-1]->type << endl;
    cout << "section_offset: " << section_headers[n-1]->section_offset << endl;
    cout << "section_size: " << section_headers[n-1]->section_size << endl;
    cout << "Секция 1: " << endl;
    cout << section[n-1];

    return 0;
}
