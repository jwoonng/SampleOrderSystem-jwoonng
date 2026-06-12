#pragma once
// encoding: UTF-8 with BOM
#include "service/OrderService.h"
#include "service/SampleService.h"

class OrderController
{
public:
    void run(OrderService& orderService, SampleService& sampleService);

private:
    void handleOrder(OrderService& orderService, SampleService& sampleService);
};