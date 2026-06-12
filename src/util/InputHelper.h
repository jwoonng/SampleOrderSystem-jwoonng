#pragma once
// encoding: UTF-8 with BOM
#include <string>

class InputHelper
{
public:
    static std::wstring promptWString(const std::wstring& label);
    static int          promptInt   (const std::wstring& label);
    static double       promptDouble(const std::wstring& label);
    static bool         promptYN    (const std::wstring& label); // true=Y, false=N
};