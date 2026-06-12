#pragma once
// encoding: UTF-8 with BOM
#include "service/SampleService.h"
#include "service/OrderService.h"
#include "service/ProductionService.h"

class MainController
{
public:
    MainController(SampleService& sampleService,
                   OrderService& orderService,
                   ProductionService& productionService);
    void run();

private:
    SampleService&     sampleService_;
    OrderService&      orderService_;
    ProductionService& productionService_;

    int  calcTotalStock() const;
    int  calcTotalOrderCount() const;
    void routeMenu(int choice);
};