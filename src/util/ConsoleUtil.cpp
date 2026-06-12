#include "ConsoleUtil.h"
#include <io.h>
#include <fcntl.h>
#include <cstdio>

bool ConsoleUtil::initEncoding()
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin),  _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);
    return true;
}