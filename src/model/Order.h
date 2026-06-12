#pragma once
// encoding: UTF-8 with BOM
#include <string>

enum class OrderStatus
{
    Reserved,
    Rejected,
    Producing,
    Confirmed,
    Release
};

class Order
{
public:
    Order(std::wstring orderId, std::wstring sampleId,
          std::wstring customerName, int quantity, std::wstring createdAt);

    std::wstring getOrderId()      const;
    std::wstring getSampleId()     const;
    std::wstring getCustomerName() const;
    int          getQuantity()     const;
    OrderStatus  getStatus()       const;
    std::wstring getCreatedAt()    const;

    void setStatus(OrderStatus status);

    std::wstring toString() const;

private:
    std::wstring orderId;
    std::wstring sampleId;
    std::wstring customerName;
    int          quantity;
    OrderStatus  status;
    std::wstring createdAt;
};