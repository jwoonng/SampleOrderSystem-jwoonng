#pragma once
// encoding: UTF-8 with BOM
#include <string>
#include "model/Sample.h"
#include "model/Order.h"

class OrderView
{
public:
    void showMenu();
    void showOrderConfirm(const Sample& sample,
                          const std::wstring& customerName,
                          int quantity);
    void showOrderSuccess(const Order& order);
    int  promptChoice();
};