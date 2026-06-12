// encoding: UTF-8 with BOM
#include "controller/MonitoringController.h"
#include "view/MonitoringView.h"
#include "util/ConsoleHelper.h"
#include <algorithm>

void MonitoringController::run(SampleService& sampleService, OrderService& orderService)
{
    MonitoringView view;
    while (true)
    {
        std::wstring currentTime = ConsoleHelper::getCurrentTimeString();
        view.showMenu(currentTime);
        int choice = view.promptChoice();

        if (choice == 1)      handleOrderStats(orderService);
        else if (choice == 2) handleStockStatus(sampleService, orderService);
        else if (choice == 0) break;
        else ConsoleHelper::printRed(L"잘못된 선택입니다.\n");
    }
}

void MonitoringController::handleOrderStats(OrderService& orderService)
{
    auto allOrders = orderService.getAllOrders();

    int reserved  = 0;
    int confirmed = 0;
    int producing = 0;
    int released  = 0;

    for (const auto& o : allOrders)
    {
        switch (o.getStatus())
        {
        case OrderStatus::Reserved:  ++reserved;  break;
        case OrderStatus::Confirmed: ++confirmed; break;
        case OrderStatus::Producing: ++producing; break;
        case OrderStatus::Release:   ++released;  break;
        default: break;
        }
    }

    MonitoringView view;
    view.showOrderStats(reserved, confirmed, producing, released);
}

void MonitoringController::handleStockStatus(SampleService& sampleService,
                                              OrderService& orderService)
{
    auto samples   = sampleService.getAllSamples();
    auto allOrders = orderService.getAllOrders();
    MonitoringView view;
    view.showStockStatus(samples, allOrders);
}