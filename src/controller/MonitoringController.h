#pragma once
// encoding: UTF-8 with BOM
#include "service/SampleService.h"
#include "service/OrderService.h"
#include "service/ProductionService.h"

class MonitoringController
{
public:
    void run(SampleService& sampleService,
             OrderService& orderService,
             ProductionService& productionService);

private:
    void handleOrderStats (OrderService& orderService);
    void handleStockStatus(SampleService& sampleService,
                           OrderService& orderService,
                           ProductionService& productionService);
};