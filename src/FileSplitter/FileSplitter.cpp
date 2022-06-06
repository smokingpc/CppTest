// FileSplitter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <errno.h>

#define BIG_BUFFER_SIZE     4096
#define SMALL_BUFFER_SIZE     1024

#define SIZE_IN_MB  1048576
#define SIZE_IN_GB  1073741824

using namespace std;
typedef basic_string<TCHAR> tstring;

typedef struct _CMD_OPTION
{
    tstring FilePath = _T("");
    INT64 SplitSize = 0;
}CMD_OPTION, * PCMD_OPTION;


class CAutoHandle
{
public :
    CAutoHandle(HANDLE handle){this->Handle = handle;}
    ~CAutoHandle()
    {
        if(this->Handle != INVALID_HANDLE_VALUE)
            CloseHandle(this->Handle);
    }
    bool IsValid()
    {
        if(this->Handle == INVALID_HANDLE_VALUE || this->Handle == NULL)
            return false;
        return true;
    }
private:
    HANDLE Handle = INVALID_HANDLE_VALUE;
};

void Usage()
{
    _tprintf(_T("Usage: FileSplitter.exe [split size] [fullpath of file]\n"));
    _tprintf(_T("       [split size] can be size_in_bytes, size_in_MB, or size_in_GB\n"));
    _tprintf(_T("       size_in_MB only have append 'M' suffix after size number.\n"));
    _tprintf(_T("       size_in_GB only have append 'G' suffix after size number.\n"));
    _tprintf(_T("       MAX split size == 2G\n"));
    _tprintf(_T("e.g. to split c:\\data\\myfile.dat into 6MB sized split files, use command  \n"));
    _tprintf(_T("     FileSplitter.exe 6M c:\\data\\myfile.dat\n"));
}

BOOL ParseArgs(CMD_OPTION& option, int argc, TCHAR* argv[])
{
    if(argc < 3)
        return FALSE;

    option.FilePath = argv[2];
    TCHAR buffer[SMALL_BUFFER_SIZE] = {0};
    _tcscpy_s(buffer, SMALL_BUFFER_SIZE, argv[1]);
    DWORD factor = 1;    //multiply factor to convert input size to size_in_bytes
    INT64 real_size = 0;

    //2 char, not G / M , oversize
    _tcslwr_s(buffer, SMALL_BUFFER_SIZE);
    TCHAR *cursor = _tcschr(buffer, _T('g'));
    if(cursor != NULL)
    {
        factor = SIZE_IN_GB;
        cursor = _T('\0');
    }
    else 
    {
        cursor = _tcschr(buffer, _T('m'));
        if (cursor != NULL)
        {
            cursor = _T('\0');
            factor = SIZE_IN_MB;
        }
        else
            factor = 1;
    }

    real_size = _ttoi64(buffer) * factor;
    if(errno != 0 || real_size == 0)
    {
        _tprintf(_T(""));
        return FALSE; 
    }

    return TRUE;
}

void CutFiles(CMD_OPTION &option)
{
    int counter = 1;
    TCHAR dest_fname[SMALL_BUFFER_SIZE] = {0};
    HANDLE src_handle = CreateFile(option.FilePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
    HANDLE dest_handle = CreateFile(option.FilePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    CAutoHandle src(src_handle);
    CAutoHandle dest(dest_handle);

    if(!src.IsValid())
    {
        _tprintf(_T("Open source file failed. LastError=%d\n"), GetLastError());
        return;
    }

    if (!dest.IsValid())
    {
        _tprintf(_T("Open destination file failed. LastError=%d\n"), GetLastError());
        return;
    }

    BYTE buffer[BIG_BUFFER_SIZE] = {0};



}

int _tmain(int argc, TCHAR* argv[])
{
    CMD_OPTION option;

    if(!ParseArgs(option, argc, argv))
    {
        Usage();
        return -1;
    }

    CutFiles(option);

    return 0;
}
