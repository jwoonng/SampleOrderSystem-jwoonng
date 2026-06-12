#include "ConsoleUtil.h"
#include <io.h>
#include <fcntl.h>
#include <cstdio>
#include <windows.h>

bool ConsoleUtil::initEncoding()
{
    // 콘솔 코드페이지를 UTF-8로 설정 (PowerShell 등 외부 콘솔 한글 깨짐 방지)
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // stdout/stderr: _O_U8TEXT — wcout을 UTF-8로 출력 (파이프·PowerShell 호환)
    // stdin:         _O_U16TEXT — 콘솔 한글 입력 처리
    _setmode(_fileno(stdout), _O_U8TEXT);
    _setmode(_fileno(stdin),  _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U8TEXT);
    return true;
}