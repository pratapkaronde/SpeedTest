#include <Windows.h>
#include <string>

class CUtils
{
public:
    static void DisplayErrorBox(const std::wstring functionName);

    inline static bool ends_with(std::wstring const &value, std::wstring const &ending)
    {
        if (ending.size() > value.size())
            return false;

        return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
    }
};