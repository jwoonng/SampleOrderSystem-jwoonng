// encoding: UTF-8 with BOM
#include "controller/OrderController.h"
#include "view/OrderView.h"
#include "util/ConsoleHelper.h"
#include "util/InputHelper.h"
#include <chrono>
#include <ctime>
#include "util/OrderIdGenerator.h"
#include <iostream>
#include <stdexcept>

void OrderController::run(OrderService& orderService, SampleService& sampleService)
{
    OrderView view;
    view.showMenu();
    handleOrder(orderService, sampleService);
}

void OrderController::handleOrder(OrderService& orderService,
                                   SampleService& sampleService)
{
    std::wstring sampleId = InputHelper::promptWString(L"시료 ID");
    auto sampleOpt = sampleService.findSampleById(sampleId);
    if (!sampleOpt.has_value())
    {
        ConsoleHelper::printRed(L"등록되지 않은 시료 ID입니다.\n");
        return;
    }

    std::wstring customerName = InputHelper::promptWString(L"고객명");
    int quantity              = InputHelper::promptInt(L"주문 수량");

    OrderView view;
    view.showOrderConfirm(sampleOpt.value(), customerName, quantity);

    bool confirmed = InputHelper::promptYN(L"접수 여부");
    if (!confirmed) return;

    try
    {
        auto allOrders = orderService.getAllOrders();
        std::wstring dateStr  = []() -> std::wstring {
            // ConsoleHelper 이용해도 되지만 순환참조 피하기 위해 직접 구성
            auto now = std::chrono::system_clock::now();
            auto t   = std::chrono::system_clock::to_time_t(now);
            struct tm tm_buf{};
            localtime_s(&tm_buf, &t);
            wchar_t buf[16];
            wcsftime(buf, 16, L"%Y%m%d", &tm_buf);
            return buf;
        }();
        std::wstring orderId  = OrderIdGenerator::generate(allOrders, dateStr);
        std::wstring createdAt = []() -> std::wstring {
            auto now = std::chrono::system_clock::now();
            auto t   = std::chrono::system_clock::to_time_t(now);
            struct tm tm_buf{};
            localtime_s(&tm_buf, &t);
            wchar_t buf[32];
            wcsftime(buf, 32, L"%Y-%m-%d %H:%M:%S", &tm_buf);
            return buf;
        }();

        Order order = orderService.placeOrder(sampleId, customerName,
                                               quantity, orderId, createdAt);
        view.showOrderSuccess(order);
    }
    catch (const std::invalid_argument& e)
    {
        ConsoleHelper::printRed(
            std::wstring(e.what(), e.what() + strlen(e.what())) + L"\n");
    }
}