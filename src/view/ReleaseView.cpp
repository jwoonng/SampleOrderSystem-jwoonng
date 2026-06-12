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
    // 컬럼 표시 너비(display columns): 번호=5, 주문번호=20, 고객=12, 시료=12, 수량=나머지
    std::wcout << ConsoleHelper::padRight(L"번호", 5)
               << ConsoleHelper::padRight(L"주문번호", 20)
               << ConsoleHelper::padRight(L"고객", 12)
               << ConsoleHelper::padRight(L"시료", 12)
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
        std::wcout << L"  " << std::left << std::setw(3) << idx++
                   << ConsoleHelper::padRight(o.getOrderId(), 20)
                   << ConsoleHelper::padRight(o.getCustomerName(), 12)
                   << ConsoleHelper::padRight(sampleName, 12)
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
    int choice = InputHelper::promptInt(L"출고할 번호");
    std::wcout << L"\n";
    return choice;
}