// encoding: UTF-8 with BOM
#include "view/ApprovalView.h"
#include "util/ConsoleHelper.h"
#include "util/InputHelper.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

// 실 생산량 계산 시 수율에 적용하는 오차 비율 (OrderService와 동일한 값 유지)
static constexpr double PRODUCTION_ERROR_RATE = 0.9;

void ApprovalView::showReservedList(const std::vector<Order>& orders,
                                     const std::vector<Sample>& allSamples)
{
    ConsoleHelper::printDoubleDivider();
    ConsoleHelper::printCyan(L"  [3] 주문 승인/거절\n");
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
        double adjustedYield = sample.getYield() * PRODUCTION_ERROR_RATE;
        int actualQty = static_cast<int>(
            std::ceil(static_cast<double>(shortage) / adjustedYield));
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
    int choice = InputHelper::promptInt(L"승인할 번호");
    std::wcout << L"\n";
    return choice;
}

bool ApprovalView::promptYNChoice()
{
    bool result = InputHelper::promptYN(L"[Y] 승인 / [N] 주문 거절");
    std::wcout << L"\n";
    return result;
}