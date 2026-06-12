#pragma once
// encoding: UTF-8 with BOM
#include <string>
#include <vector>
#include "model/Order.h"
#include "model/Sample.h"

class ReleaseView
{
public:
    void showConfirmedList(const std::vector<Order>& orders,
                           const std::vector<Sample>& allSamples);
    void showReleaseResult(const Order& order,
                           const std::wstring& releasedAt);
    int  promptChoice();
};