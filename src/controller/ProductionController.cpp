// encoding: UTF-8 with BOM
#include "controller/ProductionController.h"
#include "view/ProductionView.h"
#include "util/ConsoleHelper.h"
#include "util/InputHelper.h"

void ProductionController::run(ProductionService& productionService,
                                SampleService& sampleService,
                                OrderService& orderService)
{
    showProductionStatus(productionService, sampleService, orderService);
    // 단순 조회 화면 — promptChoice로 뒤로가기 처리
    InputHelper::promptInt(L"[0] 위로");
}

void ProductionController::showProductionStatus(ProductionService& productionService,
                                                  SampleService& sampleService,
                                                  OrderService& orderService)
{
    std::wstring currentTime = ConsoleHelper::getCurrentTimeString();
    productionService.checkCompletion(currentTime);

    auto samples = sampleService.getAllSamples();
    auto queue   = productionService.getQueue();

    ProductionView view;
    view.showHeader();

    auto jobOpt = productionService.getCurrentJob();
    if (jobOpt.has_value())
    {
        const auto& job = jobOpt.value();
        std::wstring sampleName;
        Sample foundSample(L"", L"", 0.0, 1.0, 0);
        for (const auto& s : samples)
        {
            if (s.getSampleId() == job.getSampleId())
            {
                foundSample = s;
                break;
            }
        }
        int elapsedQty = productionService.getElapsedQty(job, currentTime);
        view.showCurrentJob(job, foundSample, elapsedQty, currentTime);
    }
    else
    {
        view.showIdleMessage();
    }

    view.showQueue(queue, samples, currentTime);
}