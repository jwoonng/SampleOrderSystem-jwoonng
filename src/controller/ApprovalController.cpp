// encoding: UTF-8 with BOM
#include "controller/ApprovalController.h"
#include "view/ApprovalView.h"
#include "util/ConsoleHelper.h"
#include "util/InputHelper.h"
#include <iostream>
#include <stdexcept>
#include <chrono>
#include <ctime>

void ApprovalController::run(OrderService& orderService,
                              SampleService& sampleService,
                              ProductionService& productionService)
{
    ApprovalView view;
    auto reserved    = orderService.getReservedOrders();
    auto allSamples  = sampleService.getAllSamples();

    if (reserved.empty())
    {
        ConsoleHelper::printYellow(L"승인 대기 중인 주문이 없습니다.\n");
        return;
    }

    view.showReservedList(reserved, allSamples);
    handleApproval(orderService, sampleService, productionService);
}

void ApprovalController::handleApproval(OrderService& orderService,
                                         SampleService& sampleService,
                                         ProductionService& productionService)
{
    ApprovalView view;
    int idx = view.promptChoice();
    auto reserved = orderService.getReservedOrders();

    if (idx < 1 || idx > static_cast<int>(reserved.size()))
    {
        ConsoleHelper::printRed(L"잘못된 번호입니다.\n");
        return;
    }

    const Order& selectedOrder = reserved[static_cast<std::size_t>(idx - 1)];
    auto sampleOpt = sampleService.findSampleById(selectedOrder.getSampleId());
    if (!sampleOpt.has_value())
    {
        ConsoleHelper::printRed(L"시료 정보를 찾을 수 없습니다.\n");
        return;
    }

    view.showStockCheck(sampleOpt.value(), selectedOrder);
    bool approve = view.promptYNChoice();

    try
    {
        if (approve)
        {
            auto now = std::chrono::system_clock::now();
            auto t   = std::chrono::system_clock::to_time_t(now);
            struct tm tm_buf{};
            localtime_s(&tm_buf, &t);
            wchar_t buf[32];
            wcsftime(buf, 32, L"%Y-%m-%d %H:%M", &tm_buf);
            std::wstring startTime = buf;
            std::wstring jobId = L"JOB-" + selectedOrder.getOrderId();

            orderService.approveOrder(selectedOrder.getOrderId(), jobId, startTime);

            auto updatedOpt = orderService.findOrderById(selectedOrder.getOrderId());
            if (updatedOpt.has_value())
                view.showApprovalResult(updatedOpt.value());
        }
        else
        {
            orderService.rejectOrder(selectedOrder.getOrderId());
            auto updatedOpt = orderService.findOrderById(selectedOrder.getOrderId());
            if (updatedOpt.has_value())
                view.showRejectionResult(updatedOpt.value());
        }
    }
    catch (const std::invalid_argument& e)
    {
        ConsoleHelper::printRed(
            std::wstring(e.what(), e.what() + strlen(e.what())) + L"\n");
    }
}