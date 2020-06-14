#include <Windows.h>
#include <iostream>
#include "directory.h"
#include "utils.h"

using namespace std;

CDirectory::CDirectory(wstring path) : bIsDirectoryRead(false)
{
    mpPath = path;
}

CDirectory::~CDirectory()
{
    while (!files.empty())
    {
        auto *pFile = files.back();
        delete pFile;
        files.pop_back();
    }

    while (!children.empty())
    {
        auto child = children.back();
        delete child;
        children.pop_back();
    }
}

std::vector<CFile *> const CDirectory::Files()
{
    if (!bIsDirectoryRead)
        ReadDirectory();

    return files;
}

std::vector<std::wstring> const &CDirectory::Folders()
{
    if (!bIsDirectoryRead)
        ReadDirectory();

    return folders;
}

size_t CDirectory::TotalFiles(bool recurse)
{
    if (!bIsDirectoryRead)
        ReadDirectory();

    size_t total_files = files.size();

    if (recurse)
        for (auto child = children.begin(); child != children.end(); ++child)
            total_files += (*child)->TotalFiles(recurse);

    return total_files;
}

size_t CDirectory::TotalFolders(bool recurse)
{
    if (!bIsDirectoryRead)
        ReadDirectory();

    size_t total_folders = folders.size();

    if (recurse)
        for (auto child = children.begin(); child != children.end(); ++child)
            total_folders += (*child)->TotalFolders(recurse);

    return total_folders;
}

bool CDirectory::ReadDirectory(bool recurse, unsigned depth)
{
    wstring searchPath = mpPath;

    if (!CUtils::ends_with(searchPath, L"*"))
        searchPath += L"\\*";

    WIN32_FIND_DATAW findData;

    HANDLE hFind = FindFirstFileW(searchPath.c_str(), &findData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        CUtils::DisplayErrorBox(L"FindFirstFile");
        bIsDirectoryRead = false;
    }
    else
    {
        /* code */
        do
        {
            wstring name(findData.cFileName);
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                //cout << "<DIR> ";
                if (!name.compare(L".") || !name.compare(L".."))
                    continue;

                folders.push_back(name);
            }
            else
            {

                ULONGLONG fileSize = (static_cast<ULONGLONG>(findData.nFileSizeHigh) << static_cast<size_t>(sizeof(findData.nFileSizeLow) * 8)) |
                                     findData.nFileSizeLow;

                //size_t fileSize = (static_cast<size_t>(findData.nFileSizeHigh) << sizeof(findData.nFileSizeHigh) * 8) + findData.nFileSizeLow;

                CFile *pNewFile = new CFile(name, fileSize);
                files.push_back(pNewFile);
            }

            //wcout << name << endl;

        } while (FindNextFileW(hFind, &findData) != 0);
        bIsDirectoryRead = true;
    }
    FindClose(hFind);

    if (recurse)
        for (auto folder = folders.begin(); folder != folders.end(); ++folder)
        {
            wstring childPath = mpPath + L"\\" + *folder;
            CDirectory *pDirectory = new CDirectory(childPath);
            children.push_back(pDirectory);
            pDirectory->ReadDirectory(recurse, depth);
        }

    return bIsDirectoryRead;
}

void CDirectory::List(bool recurse, unsigned depth)
{
    if (!bIsDirectoryRead)
        ReadDirectory();

    wcout << endl
          << "Directory Listing of " << mpPath << endl;
    wcout << "==========================================================" << endl;

    try
    {
        for (auto folder = folders.begin(); folder != folders.end(); ++folder)
            wcout << "<DIR> " << *folder << endl;

        for (auto file = files.begin(); file != files.end(); ++file)
            wcout << "      " << (*file)->FileName() << endl;
    }
    catch (...)
    {
    }

    if (recurse)
    {
        for (auto child = children.begin(); child != children.end(); ++child)
            (*child)->List(recurse, depth);
    }
}