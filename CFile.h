#include <string>

class CFile
{
public:
    CFile(const std::wstring &fileName, const size_t fileSize);

private:
    std::wstring msFileName;
    size_t miFileSize;
};