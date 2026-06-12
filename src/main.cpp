#include <iostream>
#include "util/ConsoleUtil.h"

int main()
{
    ConsoleUtil::initEncoding();
    std::wcout << L"시스템 초기화 완료" << std::endl;
    return 0;
}