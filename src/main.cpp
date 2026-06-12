// encoding: UTF-8 with BOM
#include "util/ConsoleUtil.h"
#include "repository/JsonSampleRepository.h"
#include "repository/JsonOrderRepository.h"
#include "repository/JsonProductionRepository.h"
#include "service/SampleService.h"
#include "service/OrderService.h"
#include "service/ProductionService.h"
#include "controller/MainController.h"
#include <windows.h>

int main()
{
    ConsoleUtil::initEncoding();
    CreateDirectoryW(L"data", nullptr);

    JsonSampleRepository     sampleRepo    (L"data/samples.json");
    JsonOrderRepository      orderRepo     (L"data/orders.json");
    JsonProductionRepository productionRepo(L"data/production.json");

    SampleService     sampleService    (sampleRepo);
    OrderService      orderService     (orderRepo, sampleRepo, productionRepo);
    ProductionService productionService(productionRepo, sampleRepo, orderRepo);

    MainController controller(sampleService, orderService, productionService);
    controller.run();

    return 0;
}