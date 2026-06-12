// encoding: UTF-8 with BOM
#pragma once
#include <optional>
#include <vector>
#include <string>
#include "model/Order.h"

class IOrderRepository
{
public:
    virtual ~IOrderRepository() = default;
    virtual void add(const Order& order) = 0;
    virtual std::optional<Order> findById(const std::wstring& id) const = 0;
    virtual std::vector<Order> findAll() const = 0;
    virtual std::vector<Order> findByStatus(OrderStatus status) const = 0;
    virtual void update(const Order& order) = 0;
    virtual void remove(const std::wstring& id) = 0;
};