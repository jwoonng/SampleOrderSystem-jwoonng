// encoding: UTF-8 with BOM
#include "controller/MonitoringController.h"
#include "view/MonitoringView.h"
#include "util/ConsoleHelper.h"
#include <algorithm>

void MonitoringController::run(SampleService& sampleService,
                               OrderService& orderService,
                               ProductionService& productionService)
{
    MonitoringView view;
    while (true)
    {
        std::wstring currentTime = ConsoleHelper::getCurrentTimeString();
        // 모니터링 화면 진입마다 생산 완료 여부를 확인하여 재고를 최신 상태로 유지
        while (productionService.checkCompletion(currentTime)) {}

        view.showMenu(currentTime);
        int choice = view.promptChoice();

        if (choice == 1)      handleOrderStats(orderService);
        else if (choice == 2) handleStockStatus(sampleService, orderService, productionService);
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
                                              OrderService& orderService,
                                              ProductionService& productionService)
{
    auto samples   = sampleService.getAllSamples();
    auto allOrders = orderService.getAllOrders();

    // 현재 생산 중인 작업이 있고 해당 주문이 아직 PRODUCING 상태일 때만
    // 경과 생산량을 표시용 재고에 반영 (Phase 1 이후 Confirmed이면 이미 정산 대기 중)
    auto jobOpt = productionService.getCurrentJob();
    if (jobOpt.has_value())
    {
        auto orderOpt = orderService.findOrderById(jobOpt->getOrderId());
        if (orderOpt.has_value() && orderOpt->getStatus() == OrderStatus::Producing)
        {
            std::wstring currentTime = ConsoleHelper::getCurrentTimeString();
            int elapsedQty = productionService.getElapsedQty(jobOpt.value(), currentTime);
            if (elapsedQty > 0)
            {
                for (auto& s : samples)
                {
                    if (s.getSampleId() == jobOpt->getSampleId())
                    {
                        s = Sample(s.getSampleId(), s.getName(),
                                   s.getAvgProdTime(), s.getYield(),
                                   s.getStock() + elapsedQty);
                        break;
                    }
                }
            }
        }
    }

    MonitoringView view;
    view.showStockStatus(samples, allOrders);
}