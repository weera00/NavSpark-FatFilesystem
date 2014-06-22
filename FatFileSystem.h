#pragma once

#include <string.h>
#include <wchar.h>
#include "diskio.h" 
#include "ff.h" 

namespace FAT {

  class with_result {
  protected:
    FRESULT _result;

  public:
    const FRESULT result(void) const { return _result; }

  }; // class with_results

  class FileSystem : public with_result {
  protected:
    bool _mmcInit;
    FATFS _fatFs;

  public:
    FileSystem();
    virtual ~FileSystem();

    void initialize();
    void make_dir(const TCHAR *path);
  }; // class FileSystem

  class File : public with_result {
  protected:
    FIL _sdFile;

  public:
    File(const TCHAR *path, BYTE mode);
    ~File();

    void close(void);

    void lseek(DWORD ofs);
    DWORD tell(void);
    UINT write(const BYTE* buf_p, UINT len);
    UINT write(const char* buf_p) { return write((BYTE*)buf_p, strlen(buf_p)); }
    UINT write(const wchar_t* buf_p) { return write((BYTE*)buf_p, wcslen(buf_p)); }
    void read (void* buf_p, UINT len, UINT* len_p);
  }; // class File

  class Directory;
  class FileInfo : public with_result {
  protected:
    FILINFO _fileInfo;

    friend class Directory;
    FileInfo();

  public:
    FileInfo(const TCHAR* path);

    const DWORD size(void) const { return _fileInfo.fsize; }
    const WORD date(void) const { return _fileInfo.fdate; }
    const WORD time(void) const { return _fileInfo.ftime; }
    const BYTE attrib(void) const { return _fileInfo.fattrib; }
    const TCHAR* name(void) const { return _fileInfo.fname; }
#if _USE_LFN
    const TCHAR* long_name(void) const { return _fileInfo.lfname; }
    const int long_name_size(void) const { return _fileInfo.lfsize; }
#endif
  };

  class Directory : public with_result {
  protected:
    DIR _Dir;

  public:
    Directory(const TCHAR *path);

    FileInfo* next_entry(void);

  }; // class Directory

}; // namespace FAT
