// encoding: UTF-8 with BOM
#include "util/FileUtil.h"
#include <fstream>
#include <windows.h>

static constexpr unsigned char UTF8_BOM_BYTE_1 = 0xEF;
static constexpr unsigned char UTF8_BOM_BYTE_2 = 0xBB;
static constexpr unsigned char UTF8_BOM_BYTE_3 = 0xBF;
static constexpr size_t        UTF8_BOM_SIZE   = 3;

std::wstring FileUtil::readUtf8FileAsWString(const std::wstring& filePath)
{
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open())
        return L"";

    std::string bytes((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
    file.close();

    if (bytes.empty())
        return L"";

    // BOM 제거 (EF BB BF)
    if (bytes.size() >= UTF8_BOM_SIZE &&
        static_cast<unsigned char>(bytes[0]) == UTF8_BOM_BYTE_1 &&
        static_cast<unsigned char>(bytes[1]) == UTF8_BOM_BYTE_2 &&
        static_cast<unsigned char>(bytes[2]) == UTF8_BOM_BYTE_3)
    {
        bytes = bytes.substr(UTF8_BOM_SIZE);
    }

    int wlen = MultiByteToWideChar(CP_UTF8, 0,
                                   bytes.c_str(), static_cast<int>(bytes.size()),
                                   nullptr, 0);
    if (wlen <= 0)
        return L"";

    std::wstring result(wlen, L'\0');
    MultiByteToWideChar(CP_UTF8, 0,
                        bytes.c_str(), static_cast<int>(bytes.size()),
                        &result[0], wlen);
    return result;
}

bool FileUtil::writeWStringToUtf8File(const std::wstring& filePath,
                                      const std::wstring& content)
{
    int mlen = WideCharToMultiByte(CP_UTF8, 0,
                                   content.c_str(), static_cast<int>(content.size()),
                                   nullptr, 0, nullptr, nullptr);
    if (mlen <= 0)
        return false;

    std::string bytes(mlen, '\0');
    WideCharToMultiByte(CP_UTF8, 0,
                        content.c_str(), static_cast<int>(content.size()),
                        &bytes[0], mlen, nullptr, nullptr);

    std::ofstream file(filePath, std::ios::binary);
    if (!file.is_open())
        return false;

    file.write(bytes.c_str(), bytes.size());
    return true;
}
