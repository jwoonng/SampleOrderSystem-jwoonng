// encoding: UTF-8 with BOM
#define NOMINMAX
#include "util/InputHelper.h"
#include "util/ConsoleHelper.h"
#include <iostream>
#include <limits>

std::wstring InputHelper::promptWString(const std::wstring& label)
{
    ConsoleHelper::printYellow(label + L" > ");
    std::wstring val;
    std::wcin >> val;
    return val;
}

int InputHelper::promptInt(const std::wstring& label)
{
    while (true)
    {
        ConsoleHelper::printYellow(label + L" > ");
        int val = 0;
        if (std::wcin >> val)
            return val;

        std::wcin.clear();
        std::wcin.ignore((std::numeric_limits<std::streamsize>::max)(), L'\n');
        ConsoleHelper::printRed(L"숫자를 입력해 주세요.\n");
    }
}

double InputHelper::promptDouble(const std::wstring& label)
{
    while (true)
    {
        ConsoleHelper::printYellow(label + L" > ");
        double val = 0.0;
        if (std::wcin >> val)
            return val;

        std::wcin.clear();
        std::wcin.ignore((std::numeric_limits<std::streamsize>::max)(), L'\n');
        ConsoleHelper::printRed(L"숫자를 입력해 주세요.\n");
    }
}

bool InputHelper::promptYN(const std::wstring& label)
{
    while (true)
    {
        ConsoleHelper::printYellow(label + L" > ");
        std::wstring val;
        std::wcin >> val;
        if (val == L"Y" || val == L"y") return true;
        if (val == L"N" || val == L"n") return false;
        ConsoleHelper::printRed(L"Y 또는 N 을 입력해 주세요.\n");
    }
}