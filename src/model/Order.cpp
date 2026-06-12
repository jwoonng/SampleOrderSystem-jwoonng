// encoding: UTF-8 with BOM
#include "Order.h"
#include <sstream>

Order::Order(std::wstring orderId, std::wstring sampleId,
             std::wstring customerName, int quantity, std::wstring createdAt)
    : orderId(std::move(orderId))
    , sampleId(std::move(sampleId))
    , customerName(std::move(customerName))
    , quantity(quantity)
    , status(OrderStatus::Reserved)
    , createdAt(std::move(createdAt))
{
}

std::wstring Order::getOrderId() const
{
    return orderId;
}

std::wstring Order::getSampleId() const
{
    return sampleId;
}

std::wstring Order::getCustomerName() const
{
    return customerName;
}

int Order::getQuantity() const
{
    return quantity;
}

OrderStatus Order::getStatus() const
{
    return status;
}

std::wstring Order::getCreatedAt() const
{
    return createdAt;
}

void Order::setStatus(OrderStatus newStatus)
{
    status = newStatus;
}

std::wstring Order::toString() const
{
    std::wostringstream oss;
    oss << L"Order[" << orderId << L"] sample=" << sampleId
        << L" customer=" << customerName
        << L" qty=" << quantity
        << L" createdAt=" << createdAt;
    return oss.str();
}