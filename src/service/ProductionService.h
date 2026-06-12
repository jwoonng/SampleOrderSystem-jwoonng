// encoding: UTF-8 with BOM
#pragma once
#include <optional>
#include <string>
#include <vector>
#include "repository/IProductionRepository.h"
#include "repository/ISampleRepository.h"
#include "repository/IOrderRepository.h"
#include "model/ProductionJob.h"

class ProductionService
{
public:
    ProductionService(IProductionRepository& productionRepo,
                      ISampleRepository&     sampleRepo,
                      IOrderRepository&      orderRepo);

    void enqueue(const ProductionJob& job);
    std::vector<ProductionJob> getQueue() const;
    std::optional<ProductionJob> getCurrentJob() const;
    int getElapsedQty(const ProductionJob& job, const std::wstring& currentTime) const;
    bool checkCompletion(const std::wstring& currentTime);

private:
    IProductionRepository& productionRepo_;
    ISampleRepository&     sampleRepo_;
    IOrderRepository&      orderRepo_;

    static long long parseToMinutes(const std::wstring& dateTime);
};
