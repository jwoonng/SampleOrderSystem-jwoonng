// encoding: UTF-8 with BOM
#include "view/MonitoringView.h"
#include "util/ConsoleHelper.h"
#include "util/InputHelper.h"
#include <iostream>
#include <iomanip>

void MonitoringView::showMenu(const std::wstring& currentTime)
{
    ConsoleHelper::printDoubleDivider();
    ConsoleHelper::printCyan(L"  [4] 모니터링   " + currentTime + L"\n");
    ConsoleHelper::printDivider();
    std::wcout << L"  [1] 주문량 확인   [2] 재고량 확인   [0] 위로\n";
    ConsoleHelper::printDivider();
}

void MonitoringView::showOrderStats(int reserved, int confirmed,
                                     int producing, int released)
{
    ConsoleHelper::printDivider();
    std::wcout << L"  상태별 주문 현황\n";
    ConsoleHelper::printDivider();

    std::wcout << L"  ";
    ConsoleHelper::printBadge(L"RESERVED",
        FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    std::wcout << L"   " << reserved << L"건\n";

    std::wcout << L"  ";
    ConsoleHelper::printBadge(L"CONFIRMED",
        FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::wcout << L"  " << confirmed << L"건\n";

    std::wcout << L"  ";
    ConsoleHelper::printBadge(L"PRODUCING",
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::wcout << L"  " << producing << L"건 ← 생산라인 대기\n";

    std::wcout << L"  ";
    ConsoleHelper::printBadge(L"RELEASE",
        FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    std::wcout << L"    " << released << L"건\n";
    ConsoleHelper::printDivider();
}

void MonitoringView::showStockStatus(const std::vector<Sample>& samples,
                                      const std::vector<Order>& allOrders)
{
    ConsoleHelper::printDivider();
    // 컬럼 표시 너비: 시료명=22, 재고=8, 상태=[배지]+2공백=8, 잔여율=나머지
    std::wcout << ConsoleHelper::padRight(L"시료명", 22)
               << ConsoleHelper::padRight(L"재고", 8)
               << ConsoleHelper::padRight(L"상태", 8)
               << L"잔여율\n";
    ConsoleHelper::printDivider();

    for (const auto& s : samples)
    {
        int pendingQty = 0;
        for (const auto& o : allOrders)
        {
            if (o.getSampleId() == s.getSampleId())
            {
                if (o.getStatus() == OrderStatus::Reserved ||
                    o.getStatus() == OrderStatus::Producing)
                    pendingQty += o.getQuantity();
            }
        }

        int total = s.getStock() + pendingQty;
        int percent = (total > 0) ? (s.getStock() * 100 / total) : 0;

        std::wstring statusStr = ConsoleHelper::getStockStatus(s.getStock(), pendingQty);
        WORD statusColor = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        if (statusStr == L"부족")
            statusColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        else if (statusStr == L"고갈")
            statusColor = FOREGROUND_RED | FOREGROUND_INTENSITY;

        std::wcout << ConsoleHelper::padRight(s.getName(), 22)
                   << std::left << std::setw(8) << s.getStock();
        ConsoleHelper::printBadge(statusStr, statusColor);
        std::wcout << L"  " << ConsoleHelper::buildProgressBar(percent, 10)
                   << L" " << percent << L"%\n";
    }
    ConsoleHelper::printDivider();
}

int MonitoringView::promptChoice()
{
    return InputHelper::promptInt(L"선택");
}