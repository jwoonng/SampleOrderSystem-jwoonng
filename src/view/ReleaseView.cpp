// encoding: UTF-8 with BOM
#include "view/ReleaseView.h"
#include "util/ConsoleHelper.h"
#include "util/InputHelper.h"
#include <iostream>
#include <iomanip>

void ReleaseView::showConfirmedList(const std::vector<Order>& orders,
                                     const std::vector<Sample>& allSamples)
{
    ConsoleHelper::printDoubleDivider();
    ConsoleHelper::printCyan(L"  [6] 출고 처리\n");
    ConsoleHelper::printDivider();
    std::wcout << std::left
               << std::setw(5)  << L"번호"
               << std::setw(22) << L"주문번호"
               << std::setw(16) << L"고객"
               << std::setw(16) << L"시료"
               << L"수량\n";
    ConsoleHelper::printDivider();

    int idx = 1;
    for (const auto& o : orders)
    {
        std::wstring sampleName = o.getSampleId();
        for (const auto& s : allSamples)
        {
            if (s.getSampleId() == o.getSampleId())
            {
                sampleName = s.getName();
                break;
            }
        }
        std::wcout << L"  " << std::setw(3) << idx++
                   << std::setw(22) << o.getOrderId()
                   << std::setw(16) << o.getCustomerName()
                   << std::setw(16) << sampleName
                   << o.getQuantity() << L" ea\n";
    }
    ConsoleHelper::printDivider();
}

void ReleaseView::showReleaseResult(const Order& order,
                                     const std::wstring& releasedAt)
{
    ConsoleHelper::printGreen(L"출고 처리 완료.\n");
    std::wcout << L"  주문번호  " << order.getOrderId() << L"\n";
    std::wcout << L"  출고수량  " << order.getQuantity() << L" ea\n";
    std::wcout << L"  처리일시  " << releasedAt << L"\n";
    std::wcout << L"  상태      CONFIRMED → ";
    ConsoleHelper::printBadge(L"RELEASE",
        FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    std::wcout << L"\n";
}

int ReleaseView::promptChoice()
{
    return InputHelper::promptInt(L"출고할 번호");
}