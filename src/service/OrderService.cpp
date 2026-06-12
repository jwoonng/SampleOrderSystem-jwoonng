// encoding: UTF-8 with BOM
#include "service/OrderService.h"
#include "model/ProductionJob.h"
#include "model/Sample.h"
#include <stdexcept>
#include <cmath>
#include <algorithm>

// 실 생산량 계산 시 수율에 적용하는 오차 비율 (10% 감소 여유)
static constexpr double PRODUCTION_ERROR_RATE = 0.9;

OrderService::OrderService(IOrderRepository& orderRepo,
                           ISampleRepository& sampleRepo,
                           IProductionRepository& productionRepo)
    : orderRepo_(orderRepo)
    , sampleRepo_(sampleRepo)
    , productionRepo_(productionRepo)
{
}

Order OrderService::placeOrder(const std::wstring& sampleId,
                               const std::wstring& customerName,
                               int quantity,
                               const std::wstring& orderId,
                               const std::wstring& createdAt)
{
    if (quantity < 1)
        throw std::invalid_argument("주문 수량은 1 이상이어야 합니다.");

    auto sampleOpt = sampleRepo_.findById(sampleId);
    if (!sampleOpt.has_value())
        throw std::invalid_argument("등록되지 않은 시료 ID입니다.");

    Order order(orderId, sampleId, customerName, quantity, createdAt);
    orderRepo_.add(order);
    return order;
}

void OrderService::approveOrder(const std::wstring& orderId,
                                const std::wstring& jobId,
                                const std::wstring& startTime)
{
    auto orderOpt = orderRepo_.findById(orderId);
    if (!orderOpt.has_value())
        throw std::invalid_argument("존재하지 않는 주문 ID입니다.");

    Order order = orderOpt.value();
    if (order.getStatus() != OrderStatus::Reserved)
        throw std::invalid_argument("승인 가능한 상태가 아닙니다.");

    auto sampleOpt = sampleRepo_.findById(order.getSampleId());
    Sample sample = sampleOpt.value();

    int stock    = sample.getStock();
    int quantity = order.getQuantity();

    if (stock >= quantity)
    {
        Sample updatedSample(sample.getSampleId(), sample.getName(),
                             sample.getAvgProdTime(), sample.getYield(),
                             sample.getStock() - quantity);
        sampleRepo_.update(updatedSample);

        order.setStatus(OrderStatus::Confirmed);
        orderRepo_.update(order);
    }
    else
    {
        int shortage   = quantity - stock;
        double adjustedYield = sample.getYield() * PRODUCTION_ERROR_RATE;
        int actualQty  = static_cast<int>(std::ceil(static_cast<double>(shortage) / adjustedYield));
        double totalTime = sample.getAvgProdTime() * actualQty;

        ProductionJob job(jobId, orderId, order.getSampleId(),
                          shortage, actualQty, totalTime, startTime);
        productionRepo_.enqueue(job);

        order.setStatus(OrderStatus::Producing);
        orderRepo_.update(order);
    }
}

void OrderService::rejectOrder(const std::wstring& orderId)
{
    auto orderOpt = orderRepo_.findById(orderId);
    if (!orderOpt.has_value())
        throw std::invalid_argument("존재하지 않는 주문 ID입니다.");

    Order order = orderOpt.value();
    if (order.getStatus() != OrderStatus::Reserved)
        throw std::invalid_argument("거절 가능한 상태가 아닙니다.");

    order.setStatus(OrderStatus::Rejected);
    orderRepo_.update(order);
}

void OrderService::releaseOrder(const std::wstring& orderId)
{
    auto orderOpt = orderRepo_.findById(orderId);
    if (!orderOpt.has_value())
        throw std::invalid_argument("존재하지 않는 주문 ID입니다.");

    Order order = orderOpt.value();
    if (order.getStatus() != OrderStatus::Confirmed)
        throw std::invalid_argument("출고 가능한 상태가 아닙니다.");

    order.setStatus(OrderStatus::Release);
    orderRepo_.update(order);
}

std::vector<Order> OrderService::getAllOrders() const
{
    return orderRepo_.findAll();
}

std::vector<Order> OrderService::getReservedOrders() const
{
    return orderRepo_.findByStatus(OrderStatus::Reserved);
}

std::vector<Order> OrderService::getConfirmedOrders() const
{
    return orderRepo_.findByStatus(OrderStatus::Confirmed);
}

std::vector<Order> OrderService::getReleasableOrders() const
{
    auto confirmed = orderRepo_.findByStatus(OrderStatus::Confirmed);

    // createdAt 오름차순 정렬 (먼저 들어온 주문이 앞)
    std::sort(confirmed.begin(), confirmed.end(),
              [](const Order& a, const Order& b) {
                  return a.getCreatedAt() < b.getCreatedAt();
              });

    // 동일 시료에 대해서는 가장 먼저 들어온 주문만 출고 가능 (FIFO)
    std::vector<Order> releasable;
    for (const auto& order : confirmed)
    {
        bool alreadyAdded = false;
        for (const auto& r : releasable)
        {
            if (r.getSampleId() == order.getSampleId())
            {
                alreadyAdded = true;
                break;
            }
        }
        if (!alreadyAdded)
            releasable.push_back(order);
    }
    return releasable;
}

std::optional<Order> OrderService::findOrderById(const std::wstring& orderId) const
{
    return orderRepo_.findById(orderId);
}
