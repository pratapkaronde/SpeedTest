#include <string>

class CFile
{
public:
    CFile(const std::wstring &fileName, const size_t fileSize);
    inline const std::wstring &FileName() const
    {
        return msFileName;
    }

private:
    std::wstring msFileName;
    size_t miFileSize;
};