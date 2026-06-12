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