///////////////////////////////////////////////////////////////////////////////
// FileUtils.cpp
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileUtils.h"

///////////////////////////////////////////////////////////////////////////////
// Namespaces
///////////////////////////////////////////////////////////////////////////////

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Global Functions
///////////////////////////////////////////////////////////////////////////////

// Given a file path, return the filename + extension
CString GetFnameExt(CString filename)
{
    CString fnameExt;

    TCHAR drive[_MAX_DRIVE];
    TCHAR dir[_MAX_DIR];
    TCHAR fname[_MAX_FNAME];
    TCHAR ext[_MAX_EXT];

    if (_tsplitpath_s(filename, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT) == 0)
    {
        fnameExt = fname;
        fnameExt += ext;
    }

    return fnameExt;
}

// Give a file path, return the drive + dir
CString GetDirectory(CString filename)
{
    CString driveDir;

    TCHAR drive[_MAX_DRIVE];
    TCHAR dir[_MAX_DIR];
    TCHAR fname[_MAX_FNAME];
    TCHAR ext[_MAX_EXT];

    if (_tsplitpath_s(filename, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT) == 0)
    {
        driveDir = drive;
        driveDir += dir;
    }

    return driveDir;
}

// Build a path given its component strings
CString MakePath(CString drive, CString dir, CString fname, CString ext)
{
    CString     sPath;
    TCHAR       path[_MAX_PATH];

    if (_tmakepath_s(path, _MAX_PATH, drive, dir, fname, ext) == 0)
    {
        sPath = path;
    }

    return sPath;
}

// Given a file path, return TRUE if the file exists
BOOL FileExists(CString filename)
{
    BOOL exists = FALSE;
    FILE* file;

    if (fopen_s(&file, (CStringA)filename, "rb") == 0)
    {
        exists = TRUE;
        fclose(file);
    }

    return exists;
}

// Given a directory path, return TRUE if the directory exists
BOOL DirectoryExists(CString folderPath)
{
    DWORD dwAttrib = GetFileAttributes(folderPath);

    return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

// Return the file size of an open file given a FILE pointer.
// Return -1 if the file is not open.
long FileSize(FILE* file)
{
    long size = -1;

    if (file != NULL)
    {
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        rewind(file);
    }

    return size;
}

// Given a file path, open the file and return its contents as a vector of BYTEs.
// Return TRUE on success.
BOOL ReadFile(CString filename, vector<BYTE>& bytes)
{
    BOOL success = FALSE;
    FILE* file;

    if (fopen_s(&file, (CStringA)filename, "rb") == 0)
    {
        long size = FileSize(file);
        BYTE* buffer = new BYTE[size];

        if (fread(buffer, sizeof(BYTE), size, file) == size)
        {
            bytes.clear();
            bytes.insert(bytes.begin(), buffer, buffer + size);
            success = TRUE;
        }

        delete [] buffer;

        fclose(file);
    }

    return success;
}

// Write a vector of BYTEs to a new file.
// Return TRUE on success.
BOOL WriteFile(CString filename, vector<BYTE> bytes)
{
    BOOL success = FALSE;
    FILE* file;

    if (fopen_s(&file, (CStringA)filename, "wb") == 0)
    {
        size_t size = bytes.size();
        BYTE* buffer = new BYTE[size];

        for (size_t i = 0; i < size; i++)
        {
            buffer[i] = bytes[i];
        }

        size_t written = fwrite(buffer, sizeof(BYTE), size, file);
        
        delete [] buffer;

        if (written == size)
        {
            success = TRUE;
        }

        fclose(file);
    }

    return success;
}
