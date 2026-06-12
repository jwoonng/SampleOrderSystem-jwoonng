// encoding: UTF-8 with BOM
#include "view/OrderView.h"
#include "util/ConsoleHelper.h"
#include "util/InputHelper.h"
#include <iostream>

void OrderView::showMenu()
{
    ConsoleHelper::printDoubleDivider();
    ConsoleHelper::printCyan(L"  [2] 시료 주문\n");
    ConsoleHelper::printDivider();
}

void OrderView::showOrderConfirm(const Sample& sample,
                                  const std::wstring& customerName,
                                  int quantity)
{
    ConsoleHelper::printDivider();
    std::wcout << L"  시료   " << sample.getName()
               << L"  (" << sample.getSampleId() << L")\n";
    std::wcout << L"  고객   " << customerName << L"\n";
    std::wcout << L"  수량   " << quantity << L" ea\n";
    ConsoleHelper::printDivider();
    std::wcout << L"  [Y] 예약 접수   [N] 취소\n";
}

void OrderView::showOrderSuccess(const Order& order)
{
    ConsoleHelper::printGreen(L"예약 접수 완료.\n");
    std::wcout << L"  주문번호  " << order.getOrderId() << L"\n";
    std::wcout << L"  현재 상태 ";
    ConsoleHelper::printBadge(L"RESERVED",
        FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    std::wcout << L"\n";
    ConsoleHelper::printYellow(L"  ※ 재고 확인은 [3] 승인 메뉴에서 직접 진행하세요.\n");
}

int OrderView::promptChoice()
{
    return InputHelper::promptInt(L"선택");
}