#pragma once
// encoding: UTF-8 with BOM
#include <string>
#include <windows.h>

class ConsoleHelper
{
public:
    // ── 순수 함수 (테스트 대상) ──────────────────────────────
    // 진행바: percent(0~100), width 칸
    // 예: buildProgressBar(80, 10) → L"████████░░"
    static std::wstring buildProgressBar(int percent, int width = 10);

    // 재고 상태: stock==0 → L"고갈" / stock<pending → L"부족" / else → L"여유"
    static std::wstring getStockStatus(int stock, int pendingQuantity);

    // ── 시각 유틸 ────────────────────────────────────────────
    // "YYYY-MM-DD HH:MM:SS" 반환
    static std::wstring getCurrentTimeString();
    // "YYYYMMDD" 반환
    static std::wstring getCurrentDateString();

    // ── 색상 출력 ────────────────────────────────────────────
    static void printCyan   (const std::wstring& text);
    static void printGreen  (const std::wstring& text);
    static void printYellow (const std::wstring& text);
    static void printRed    (const std::wstring& text);
    static void printMagenta(const std::wstring& text);
    static void printWhite  (const std::wstring& text);

    // ── 구분선 ───────────────────────────────────────────────
    static void printDoubleDivider(); // ══════ 60자
    static void printDivider();       // ────── 60자

    // ── 상태 배지 ────────────────────────────────────────────
    // 예: printBadge(L"CONFIRMED", Green)
    static void printBadge(const std::wstring& text, WORD color);

    // ── 정렬 유틸 ────────────────────────────────────────────
    // 한글/CJK = 2칸, 그 외 = 1칸으로 계산한 표시 너비
    static int displayWidth(const std::wstring& s);
    // 표시 너비 기준으로 오른쪽을 공백으로 채워 width 칸으로 맞춤
    static std::wstring padRight(const std::wstring& s, int width);

private:
    static void setColor(WORD color);
    static void resetColor();
};