#pragma once
// encoding: UTF-8 with BOM
#include <string>
#include <vector>
#include "model/Order.h"
#include "model/Sample.h"

class MonitoringView
{
public:
    void showMenu(const std::wstring& currentTime);
    void showOrderStats(int reserved, int confirmed,
                        int producing, int released);
    void showStockStatus(const std::vector<Sample>& samples,
                         const std::vector<Order>& allOrders);
    int  promptChoice();
};