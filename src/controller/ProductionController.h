#pragma once
// encoding: UTF-8 with BOM
#include "service/ProductionService.h"
#include "service/SampleService.h"
#include "service/OrderService.h"

class ProductionController
{
public:
    void run(ProductionService& productionService,
             SampleService& sampleService,
             OrderService& orderService);

private:
    void showProductionStatus(ProductionService& productionService,
                               SampleService& sampleService,
                               OrderService& orderService);
};