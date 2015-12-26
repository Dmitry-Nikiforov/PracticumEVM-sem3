#include <iostream>
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <winbase.h>
#include <sstream>
#include <iomanip>
using namespace std;

struct section_header
{
  char owner[15];//имя владельца
  unsigned int type;
  unsigned int section_offset; //Смещение секции относительно начала файла
  unsigned int section_size; //Размер секции
};


  struct file_header
  {
    unsigned int sig;
    unsigned int n_sections;
    unsigned int offset;
  };


int main(){

    HANDLE hFile, hFileMapping;
    TCHAR fileName[17]="C:\\dir\\in.txt";
    DWORD fSize;
    char buf[100];
    file_header * file_header1;
    section_header * section_headers;
    char ** pp;


    hFile=CreateFile(fileName,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);

    hFileMapping=CreateFileMapping(hFile,NULL,PAGE_READONLY,0,0,NULL);

    file_header1 =( file_header* ) MapViewOfFile(hFileMapping,FILE_MAP_READ,0,0,12);

    section_headers=new section_header  [file_header1->n_sections];

    section_headers = (section_header*) MapViewOfFile(hFileMapping,FILE_MAP_READ,0,file_header1->offset,sizeof(section_header));

    cout<<section_headers[0].owner;

    pp=new char* [file_header1->n_sections];

    for (int i=0; i<file_header1->n_sections; i++){
//        *pp[i]=new char ;
    }


    cout<<section_headers[1]->owner;
    return 0;
}
