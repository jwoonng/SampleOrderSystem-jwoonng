// encoding: UTF-8 with BOM
#include "view/MainView.h"
#include "util/ConsoleHelper.h"
#include "util/InputHelper.h"
#include <iostream>
#include <iomanip>

void MainView::showHeader(const std::wstring& currentTime)
{
    ConsoleHelper::printDoubleDivider();
    ConsoleHelper::printCyan(L"  시스템 현황   " + currentTime + L"\n");
    ConsoleHelper::printDivider();
}

void MainView::showSummary(int sampleCount, int totalStock,
                            int orderCount, int queueCount)
{
    std::wcout << L"  등록 시료 " << sampleCount << L"종  총 재고  ";
    ConsoleHelper::printYellow(std::to_wstring(totalStock));
    std::wcout << L" ea\n";
    std::wcout << L"  전체 주문 " << orderCount << L"건  생산라인 ";
    ConsoleHelper::printYellow(std::to_wstring(queueCount));
    std::wcout << L"건 대기\n";
    ConsoleHelper::printDivider();
}

void MainView::showMenu()
{
    std::wcout << L"  [1] 시료 관리          [2] 시료 주문\n";
    std::wcout << L"  [3] 주문 승인/거절     [4] 모니터링\n";
    std::wcout << L"  [5] 생산라인 조회      [6] 출고 처리\n";
    std::wcout << L"  [0] 종료\n";
    ConsoleHelper::printDivider();
}

int MainView::promptChoice()
{
    int choice = InputHelper::promptInt(L"선택");
    std::wcout << L"\n";
    return choice;
}