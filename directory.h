#include <vector>
#include <string>

class CDirectory
{
public:
    CDirectory(std::wstring path);
    ~CDirectory();

    bool ReadDirectory(bool recurse = false, unsigned depth = 0);
    void List(bool recurse = false, unsigned depth = 0);

    size_t TotalFiles(bool recurse = false);
    size_t TotalFolders(bool recurse = false);

    std::vector<std::wstring> const &Files();
    std::vector<std::wstring> const &Folders();

private:
    std::vector<std::wstring> files;
    std::vector<std::wstring> folders;
    std::vector<CDirectory *> children;

    bool bIsDirectoryRead;

    std::wstring mpPath;
};