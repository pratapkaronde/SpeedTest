#include <Windows.h>
#include <iostream>
#include "directory.h"
#include "utils.h"

using namespace std;

/**
 * @brief Construct a new CDirectory::CDirectory object
 * 
 * @param path --> Path of the directory object
 */
CDirectory::CDirectory(wstring path) : bIsDirectoryRead(false)
{
    mpPath = path;
}

/**
 * @brief Destroy the CDirectory::CDirectory object
 * 
 */
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

/**
 * @brief Access the list of files in this directory 
 * 
 * @return std::vector<CFile *> const --> Returns a vector of pointers to all CFile Objects 
 */
std::vector<CFile *> const CDirectory::Files()
{
    if (!bIsDirectoryRead)
        ReadDirectory();

    return files;
}

/**
 * @brief Access the list of sub-folders in this directory 
 * 
 * @return std::vector<std::wstring> const& --> Returns a vector of all folder names 
 */
std::vector<std::wstring> const &CDirectory::Folders()
{
    if (!bIsDirectoryRead)
        ReadDirectory();

    return folders;
}

/**
 * @brief Get count of files in this directory 
 * 
 * @param recurse --> If true, Include files in the sub-folders 
 * @return size_t --> Returns the count of files 
 */
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

/**
 * @brief Get count of sub-folders in this directory 
 * 
 * @param recurse --> If true, count folders inside the sub-folders
 * @return size_t --> Count of sub-folders 
 */
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

/**
 * @brief Scan the directory to discover files and sub-folders 
 * 
 * @param recurse --> If true, scan sub-folders
 * @param depth --> If recurse set to true, limit scan to specified depth. By default scans till the very end 
 * @return true --> If scan succeded
 * @return false --> If scan failed 
 */
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
                if (!name.compare(L".") || !name.compare(L".."))
                    continue;

                folders.push_back(name);
            }
            else
            {

                ULONGLONG fileSize = (static_cast<ULONGLONG>(findData.nFileSizeHigh) << static_cast<size_t>(sizeof(findData.nFileSizeLow) * 8)) |
                                     findData.nFileSizeLow;

                CFile *pNewFile = new CFile(name, fileSize);
                files.push_back(pNewFile);
            }

        } while (FindNextFileW(hFind, &findData) != 0);
        bIsDirectoryRead = true;
    }
    FindClose(hFind);

    /**
     * @brief Scan sub-folders if recurse set to true 
     */
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

/**
 * @brief Display the list of files / folders in this directory 
 * 
 * @param recurse --> Include the files and folders in all the sub-directories 
 * @param depth --> If set, limit scanning to the specified depth 
 */
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