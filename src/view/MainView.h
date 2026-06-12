#pragma once
// encoding: UTF-8 with BOM
#include <string>

class MainView
{
public:
    void showHeader(const std::wstring& currentTime);
    void showSummary(int sampleCount, int totalStock,
                     int orderCount, int queueCount);
    void showMenu();
    int  promptChoice();
};