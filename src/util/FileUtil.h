// encoding: UTF-8 with BOM
#pragma once
#include <string>

// 순수 파일 I/O 유틸리티 (JSON 관련 로직 포함 금지)
class FileUtil
{
public:
    // UTF-8 파일을 읽어 wstring으로 반환. BOM이 있으면 제거한다.
    // 파일이 없거나 읽기 실패 시 빈 문자열 반환.
    static std::wstring readUtf8FileAsWString(const std::wstring& filePath);

    // wstring 내용을 UTF-8로 인코딩하여 파일에 쓴다.
    // 성공 시 true, 실패 시 false 반환.
    static bool writeWStringToUtf8File(const std::wstring& filePath,
                                       const std::wstring& content);
};
