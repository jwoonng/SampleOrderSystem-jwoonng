// encoding: UTF-8 with BOM
#include "util/ConsoleHelper.h"
#include <chrono>
#include <ctime>
#include <iostream>

// ── 진행바 ────────────────────────────────────────────────────
std::wstring ConsoleHelper::buildProgressBar(int percent, int width)
{
    int filled = (percent * width) / 100;
    std::wstring bar;
    bar.reserve(static_cast<std::wstring::size_type>(width));
    for (int i = 0; i < width; ++i)
        bar += (i < filled) ? L'█' : L'░'; // █ / ░
    return bar;
}

// ── 재고 상태 ─────────────────────────────────────────────────
std::wstring ConsoleHelper::getStockStatus(int stock, int pendingQuantity)
{
    if (stock == 0)                  return L"고갈"; // 고갈
    if (stock < pendingQuantity)     return L"부족"; // 부족
    return L"여유";                                  // 여유
}

// ── 시각 유틸 ─────────────────────────────────────────────────
std::wstring ConsoleHelper::getCurrentTimeString()
{
    auto now = std::chrono::system_clock::now();
    auto t   = std::chrono::system_clock::to_time_t(now);
    struct tm tm_buf{};
    localtime_s(&tm_buf, &t);
    wchar_t buf[32];
    wcsftime(buf, 32, L"%Y-%m-%d %H:%M:%S", &tm_buf);
    return buf;
}

std::wstring ConsoleHelper::getCurrentDateString()
{
    auto now = std::chrono::system_clock::now();
    auto t   = std::chrono::system_clock::to_time_t(now);
    struct tm tm_buf{};
    localtime_s(&tm_buf, &t);
    wchar_t buf[16];
    wcsftime(buf, 16, L"%Y%m%d", &tm_buf);
    return buf;
}

// ── 색상 헬퍼 ─────────────────────────────────────────────────
void ConsoleHelper::setColor(WORD color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void ConsoleHelper::resetColor()
{
    setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

// ── 색상 출력 ─────────────────────────────────────────────────
void ConsoleHelper::printCyan(const std::wstring& text)
{
    setColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::wcout << text;
    resetColor();
}

void ConsoleHelper::printGreen(const std::wstring& text)
{
    setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::wcout << text;
    resetColor();
}

void ConsoleHelper::printYellow(const std::wstring& text)
{
    setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::wcout << text;
    resetColor();
}

void ConsoleHelper::printRed(const std::wstring& text)
{
    setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
    std::wcout << text;
    resetColor();
}

void ConsoleHelper::printMagenta(const std::wstring& text)
{
    setColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    std::wcout << text;
    resetColor();
}

void ConsoleHelper::printWhite(const std::wstring& text)
{
    setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    std::wcout << text;
    resetColor();
}

// ── 구분선 ────────────────────────────────────────────────────
void ConsoleHelper::printDoubleDivider()
{
    std::wcout << L"══════════"
                  L"══════════"
                  L"══════════"
                  L"══════════"
                  L"══════════"
                  L"══════════"
               << L"\n";
}

void ConsoleHelper::printDivider()
{
    std::wcout << L"──────────"
                  L"──────────"
                  L"──────────"
                  L"──────────"
                  L"──────────"
                  L"──────────"
               << L"\n";
}

// ── 상태 배지 ─────────────────────────────────────────────────
void ConsoleHelper::printBadge(const std::wstring& text, WORD color)
{
    setColor(color);
    std::wcout << L"[" << text << L"]";
    resetColor();
}

// ── 정렬 유틸 ─────────────────────────────────────────────────
int ConsoleHelper::displayWidth(const std::wstring& s)
{
    int w = 0;
    for (wchar_t c : s)
    {
        // 한글 자모, 한글 음절, CJK 통합 한자, 전각 문자 등 → 2칸
        if ((c >= 0x1100 && c <= 0x11FF) ||   // 한글 자모
            (c >= 0x2E80 && c <= 0x303F) ||   // CJK 부수
            (c >= 0x3040 && c <= 0x33FF) ||   // 히라가나/가타카나/CJK 호환
            (c >= 0x3400 && c <= 0x4DBF) ||   // CJK 확장 A
            (c >= 0x4E00 && c <= 0x9FFF) ||   // CJK 통합 한자
            (c >= 0xAC00 && c <= 0xD7AF) ||   // 한글 음절
            (c >= 0xF900 && c <= 0xFAFF) ||   // CJK 호환 한자
            (c >= 0xFF00 && c <= 0xFFEF))      // 전각 문자
            w += 2;
        else
            w += 1;
    }
    return w;
}

std::wstring ConsoleHelper::padRight(const std::wstring& s, int width)
{
    int padding = width - displayWidth(s);
    if (padding <= 0) return s;
    return s + std::wstring(static_cast<std::wstring::size_type>(padding), L' ');
}