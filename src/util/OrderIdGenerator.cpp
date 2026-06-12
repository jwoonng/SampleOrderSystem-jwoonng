// encoding: UTF-8 with BOM
#include "util/OrderIdGenerator.h"
#include <cstdio>

std::wstring OrderIdGenerator::generate(const std::vector<Order>& allOrders,
                                         const std::wstring& dateStr)
{
    int seq = static_cast<int>(allOrders.size()) + 1;
    wchar_t buf[32];
    swprintf_s(buf, 32, L"ORD-%s-%04d", dateStr.c_str(), seq);
    return buf;
}