#pragma once
// encoding: UTF-8 with BOM
#include "service/OrderService.h"
#include "service/SampleService.h"
#include "service/ProductionService.h"

class ApprovalController
{
public:
    void run(OrderService& orderService,
             SampleService& sampleService,
             ProductionService& productionService);

private:
    void handleApproval(OrderService& orderService,
                        SampleService& sampleService,
                        ProductionService& productionService);
};