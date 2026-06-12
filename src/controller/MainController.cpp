// encoding: UTF-8 with BOM
#include "controller/MainController.h"
#include "controller/SampleController.h"
#include "controller/OrderController.h"
#include "controller/ApprovalController.h"
#include "controller/MonitoringController.h"
#include "controller/ProductionController.h"
#include "controller/ReleaseController.h"
#include "view/MainView.h"
#include "util/ConsoleHelper.h"

MainController::MainController(SampleService& sampleService,
                                OrderService& orderService,
                                ProductionService& productionService)
    : sampleService_(sampleService)
    , orderService_(orderService)
    , productionService_(productionService)
{
}

void MainController::run()
{
    // 앱 시작 시: 경과 시간 기반 완료된 생산 작업을 모두 처리
    {
        std::wstring startupTime = ConsoleHelper::getCurrentTimeString();
        while (productionService_.checkCompletion(startupTime)) {}
    }

    MainView view;
    while (true)
    {
        // 메인 메뉴 진입마다 생산 완료 여부 재확인
        std::wstring currentTime = ConsoleHelper::getCurrentTimeString();
        while (productionService_.checkCompletion(currentTime)) {}
        view.showHeader(currentTime);
        view.showSummary(
            static_cast<int>(sampleService_.getAllSamples().size()),
            calcTotalStock(),
            calcTotalOrderCount(),
            static_cast<int>(productionService_.getQueue().size()));
        view.showMenu();

        int choice = view.promptChoice();
        if (choice == 0) break;
        routeMenu(choice);
    }
}

int MainController::calcTotalStock() const
{
    int total = 0;
    for (const auto& s : sampleService_.getAllSamples())
        total += s.getStock();
    return total;
}

int MainController::calcTotalOrderCount() const
{
    return static_cast<int>(orderService_.getAllOrders().size());
}

void MainController::routeMenu(int choice)
{
    switch (choice)
    {
    case 1:
    {
        SampleController ctrl;
        ctrl.run(sampleService_);
        break;
    }
    case 2:
    {
        OrderController ctrl;
        ctrl.run(orderService_, sampleService_);
        break;
    }
    case 3:
    {
        ApprovalController ctrl;
        ctrl.run(orderService_, sampleService_, productionService_);
        break;
    }
    case 4:
    {
        MonitoringController ctrl;
        ctrl.run(sampleService_, orderService_);
        break;
    }
    case 5:
    {
        ProductionController ctrl;
        ctrl.run(productionService_, sampleService_, orderService_);
        break;
    }
    case 6:
    {
        ReleaseController ctrl;
        ctrl.run(orderService_, sampleService_);
        break;
    }
    default:
        ConsoleHelper::printRed(L"잘못된 선택입니다.\n");
        break;
    }
}