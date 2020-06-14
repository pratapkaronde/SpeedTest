#include <string>

/**
 * @brief Class to hold information of a file 
 * 
 */
class CFile
{
public:
    /**
     * @brief Construct a new CFile object
     * 
     * @param fileName - Name of the file 
     * @param fileSize - Size of the file 
     */
    CFile(const std::wstring &fileName, const size_t fileSize);

    /**
     * @brief Access the stored file name 
     * 
     * @return const std::wstring& --> Name of the file 
     */
    inline const std::wstring &FileName() const
    {
        return msFileName;
    }

    /**
     * @brief Access the stored file size 
     * 
     * @return const size_t& --> Size of the file
     */
    inline const size_t &FileSize() const
    {
        return miFileSize;
    }

private:
    std::wstring msFileName;
    size_t miFileSize;
};