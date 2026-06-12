#pragma once
// encoding: UTF-8 with BOM
#include <string>
#include <vector>
#include "model/ProductionJob.h"
#include "model/Sample.h"

class ProductionView
{
public:
    void showHeader();
    void showCurrentJob(const ProductionJob& job,
                        const Sample& sample,
                        int elapsedQty,
                        const std::wstring& currentTime);
    void showIdleMessage();
    void showQueue(const std::vector<ProductionJob>& jobs,
                   const std::vector<Sample>& allSamples,
                   const std::wstring& currentTime);
    int  promptChoice();
};