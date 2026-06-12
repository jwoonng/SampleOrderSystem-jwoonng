// encoding: UTF-8 with BOM
#include "view/ApprovalView.h"
#include "util/ConsoleHelper.h"
#include "util/InputHelper.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

void ApprovalView::showReservedList(const std::vector<Order>& orders,
                                     const std::vector<Sample>& allSamples)
{
    ConsoleHelper::printDoubleDivider();
    ConsoleHelper::printCyan(L"  [3] 주문 승인/거절\n");
    ConsoleHelper::printDivider();
    std::wcout << std::left
               << std::setw(5)  << L"번호"
               << std::setw(22) << L"주문번호"
               << std::setw(16) << L"고객"
               << std::setw(16) << L"시료"
               << std::setw(8)  << L"수량"
               << L"상태\n";
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
                   << std::setw(8)  << o.getQuantity()
                   << L"RESERVED\n";
    }
    ConsoleHelper::printDivider();
}

void ApprovalView::showStockCheck(const Sample& sample, const Order& order)
{
    ConsoleHelper::printDivider();
    std::wcout << L"  시료    " << sample.getName() << L"\n";
    std::wcout << L"  현재재고 " << sample.getStock() << L" ea\n";
    std::wcout << L"  주문수량 " << order.getQuantity() << L" ea\n";

    if (sample.getStock() >= order.getQuantity())
    {
        ConsoleHelper::printGreen(L"  재고 충분. 바로 확정하시겠습니까?\n");
    }
    else
    {
        int shortage = order.getQuantity() - sample.getStock();
        double yield09 = sample.getYield() * 0.9;
        int actualQty = static_cast<int>(
            std::ceil(static_cast<double>(shortage) / yield09));
        double totalTime = sample.getAvgProdTime() * actualQty;

        std::wcout << L"  재고 부족.  부족분 ";
        ConsoleHelper::printRed(std::to_wstring(shortage) + L" ea");
        std::wcout << L" 승인하시겠습니까?";
        ConsoleHelper::printYellow(L" (실생산량 " + std::to_wstring(actualQty)
            + L" ea / " + std::to_wstring(static_cast<int>(totalTime)) + L" min)\n");
    }
    std::wcout << L"  [Y] 승인   [N] 주문 거절\n";
}

void ApprovalView::showApprovalResult(const Order& updatedOrder)
{
    ConsoleHelper::printGreen(L"승인 완료.\n");
    std::wcout << L"  상태 변경  RESERVED → ";

    if (updatedOrder.getStatus() == OrderStatus::Confirmed)
        ConsoleHelper::printBadge(L"CONFIRMED",
            FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    else
        ConsoleHelper::printBadge(L"PRODUCING",
            FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

    std::wcout << L"\n  주문번호   " << updatedOrder.getOrderId() << L"\n";
}

void ApprovalView::showRejectionResult(const Order& updatedOrder)
{
    ConsoleHelper::printRed(L"주문이 거절되었습니다.\n");
    std::wcout << L"  상태 변경  RESERVED → ";
    ConsoleHelper::printBadge(L"REJECTED", FOREGROUND_RED | FOREGROUND_INTENSITY);
    std::wcout << L"\n";
}

int ApprovalView::promptChoice()
{
    return InputHelper::promptInt(L"승인할 번호");
}

bool ApprovalView::promptYNChoice()
{
    return InputHelper::promptYN(L"[Y] 승인 / [N] 주문 거절");
}