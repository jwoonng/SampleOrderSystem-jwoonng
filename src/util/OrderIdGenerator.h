#pragma once
// encoding: UTF-8 with BOM
#include <string>
#include <vector>
#include "model/Order.h"

class OrderIdGenerator
{
public:
    // 형식: "ORD-YYYYMMDD-XXXX"
    // XXXX = allOrders.size() + 1, 4자리 zero-padding
    static std::wstring generate(const std::vector<Order>& allOrders,
                                  const std::wstring& dateStr);
};