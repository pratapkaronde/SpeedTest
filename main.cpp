#define UNICODE
#pragma comment(lib, "user32")

#include <iostream>
#include "directory.h"
#include "utils.h"

using namespace std;

/**
 * Program Entry Point 
 */
int wmain(int argc, wchar_t *argv[])
{
    if (argc < 3)
    {
        std::wcerr << endl
                   << "Usage: " << argv[0] << " <source path> <target>" << std::endl;
        return -1;
    }

    CDirectory *pRoot_directory = new CDirectory(argv[1]);
    pRoot_directory->ReadDirectory(true);
    std::wcout << pRoot_directory->TotalFiles(true) << " files and " << pRoot_directory->TotalFolders(true) << " folders." << endl;
    pRoot_directory->List();

    delete pRoot_directory;

    return 0;
}