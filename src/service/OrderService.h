#pragma once
// encoding: UTF-8 with BOM
#include <optional>
#include <string>
#include <vector>
#include "repository/IOrderRepository.h"
#include "repository/ISampleRepository.h"
#include "repository/IProductionRepository.h"
#include "model/Order.h"

class OrderService
{
public:
    OrderService(IOrderRepository& orderRepo,
                 ISampleRepository& sampleRepo,
                 IProductionRepository& productionRepo);

    Order placeOrder(const std::wstring& sampleId,
                     const std::wstring& customerName,
                     int quantity,
                     const std::wstring& orderId,
                     const std::wstring& createdAt);

    void approveOrder(const std::wstring& orderId,
                      const std::wstring& jobId,
                      const std::wstring& startTime);

    void rejectOrder(const std::wstring& orderId);
    void releaseOrder(const std::wstring& orderId);

    std::vector<Order> getAllOrders() const;
    std::vector<Order> getReservedOrders() const;
    std::vector<Order> getConfirmedOrders() const;
    // 동일 시료 FIFO 적용: 시료별로 가장 먼저 들어온 Confirmed 주문만 반환
    std::vector<Order> getReleasableOrders() const;
    std::optional<Order> findOrderById(const std::wstring& orderId) const;

private:
    IOrderRepository&      orderRepo_;
    ISampleRepository&     sampleRepo_;
    IProductionRepository& productionRepo_;
};
