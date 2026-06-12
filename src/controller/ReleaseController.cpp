// encoding: UTF-8 with BOM
#include "controller/ReleaseController.h"
#include "view/ReleaseView.h"
#include "util/ConsoleHelper.h"
#include <stdexcept>

void ReleaseController::run(OrderService& orderService, SampleService& sampleService)
{
    auto confirmed  = orderService.getReleasableOrders();
    auto allSamples = sampleService.getAllSamples();

    if (confirmed.empty())
    {
        ConsoleHelper::printYellow(L"출고 가능한 주문이 없습니다.\n");
        return;
    }

    ReleaseView view;
    view.showConfirmedList(confirmed, allSamples);
    handleRelease(orderService, sampleService);
}

void ReleaseController::handleRelease(OrderService& orderService,
                                       SampleService& sampleService)
{
    ReleaseView view;
    int idx = view.promptChoice();
    auto confirmed = orderService.getReleasableOrders();

    if (idx < 1 || idx > static_cast<int>(confirmed.size()))
    {
        ConsoleHelper::printRed(L"잘못된 번호입니다.\n");
        return;
    }

    const Order& selected = confirmed[static_cast<std::size_t>(idx - 1)];
    try
    {
        orderService.releaseOrder(selected.getOrderId());
        std::wstring releasedAt = ConsoleHelper::getCurrentTimeString();
        auto updatedOpt = orderService.findOrderById(selected.getOrderId());
        if (updatedOpt.has_value())
            view.showReleaseResult(updatedOpt.value(), releasedAt);
    }
    catch (const std::invalid_argument& e)
    {
        ConsoleHelper::printRed(
            std::wstring(e.what(), e.what() + strlen(e.what())) + L"\n");
    }
}