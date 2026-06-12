#pragma once
// encoding: UTF-8 with BOM
#include "service/OrderService.h"
#include "service/SampleService.h"

class ReleaseController
{
public:
    void run(OrderService& orderService, SampleService& sampleService);

private:
    void handleRelease(OrderService& orderService, SampleService& sampleService);
};