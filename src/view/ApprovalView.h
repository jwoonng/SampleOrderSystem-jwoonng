#pragma once
// encoding: UTF-8 with BOM
#include <string>
#include <vector>
#include "model/Order.h"
#include "model/Sample.h"

class ApprovalView
{
public:
    void showReservedList(const std::vector<Order>& orders,
                          const std::vector<Sample>& allSamples);
    void showStockCheck(const Sample& sample, const Order& order);
    void showApprovalResult(const Order& updatedOrder);
    void showRejectionResult(const Order& updatedOrder);
    int  promptChoice();
    bool promptYNChoice();
};