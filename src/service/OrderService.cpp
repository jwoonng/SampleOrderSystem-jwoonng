// encoding: UTF-8 with BOM
#include "service/OrderService.h"
#include "model/ProductionJob.h"
#include "model/Sample.h"
#include <stdexcept>
#include <cmath>

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
        double yield09 = sample.getYield() * 0.9;
        int actualQty  = static_cast<int>(std::ceil(static_cast<double>(shortage) / yield09));
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

std::vector<Order> OrderService::getReservedOrders() const
{
    return orderRepo_.findByStatus(OrderStatus::Reserved);
}

std::vector<Order> OrderService::getConfirmedOrders() const
{
    return orderRepo_.findByStatus(OrderStatus::Confirmed);
}

std::optional<Order> OrderService::findOrderById(const std::wstring& orderId) const
{
    return orderRepo_.findById(orderId);
}
