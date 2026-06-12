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

    std::vector<Order> getReservedOrders() const;
    std::vector<Order> getConfirmedOrders() const;
    std::optional<Order> findOrderById(const std::wstring& orderId) const;

private:
    IOrderRepository&      orderRepo_;
    ISampleRepository&     sampleRepo_;
    IProductionRepository& productionRepo_;
};
