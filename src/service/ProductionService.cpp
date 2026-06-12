// encoding: UTF-8 with BOM
#include "service/ProductionService.h"
#include "model/Sample.h"
#include "model/Order.h"
#include <cmath>
#include <algorithm>
#include <stdexcept>

ProductionService::ProductionService(IProductionRepository& productionRepo,
                                     ISampleRepository&     sampleRepo,
                                     IOrderRepository&      orderRepo)
    : productionRepo_(productionRepo)
    , sampleRepo_(sampleRepo)
    , orderRepo_(orderRepo)
{
}

void ProductionService::enqueue(const ProductionJob& job)
{
    productionRepo_.enqueue(job);
}

std::vector<ProductionJob> ProductionService::getQueue() const
{
    return productionRepo_.findAll();
}

std::optional<ProductionJob> ProductionService::getCurrentJob() const
{
    return productionRepo_.front();
}

int ProductionService::getElapsedQty(const ProductionJob& job,
                                      const std::wstring& currentTime) const
{
    auto sampleOpt = sampleRepo_.findById(job.getSampleId());
    if (!sampleOpt.has_value())
        return 0;

    double avgProdTime = sampleOpt->getAvgProdTime();

    long long elapsed = parseToMinutes(currentTime) - parseToMinutes(job.getStartTime());
    if (elapsed <= 0)
        return 0;

    int qty = static_cast<int>(std::floor(static_cast<double>(elapsed) / avgProdTime));
    return std::min(qty, job.getActualQty());
}

bool ProductionService::checkCompletion(const std::wstring& currentTime)
{
    auto jobOpt = productionRepo_.front();
    if (!jobOpt.has_value())
        return false;

    const ProductionJob& job = jobOpt.value();
    int elapsed = getElapsedQty(job, currentTime);

    if (elapsed < job.getActualQty())
        return false;

    auto sampleOpt = sampleRepo_.findById(job.getSampleId());
    auto orderOpt  = orderRepo_.findById(job.getOrderId());

    Sample sample = sampleOpt.value();
    Order  order  = orderOpt.value();

    int newStock = sample.getStock() + job.getActualQty() - order.getQuantity();
    Sample updatedSample(sample.getSampleId(), sample.getName(),
                         sample.getAvgProdTime(), sample.getYield(), newStock);
    sampleRepo_.update(updatedSample);

    order.setStatus(OrderStatus::Confirmed);
    orderRepo_.update(order);

    productionRepo_.dequeue();

    return true;
}

long long ProductionService::parseToMinutes(const std::wstring& dateTime)
{
    int year   = std::stoi(dateTime.substr(0, 4));
    int month  = std::stoi(dateTime.substr(5, 2));
    int day    = std::stoi(dateTime.substr(8, 2));
    int hour   = std::stoi(dateTime.substr(11, 2));
    int minute = std::stoi(dateTime.substr(14, 2));

    long long totalMinutes = static_cast<long long>(year)  * 365LL * 24LL * 60LL
                           + static_cast<long long>(month) * 30LL  * 24LL * 60LL
                           + static_cast<long long>(day)   * 24LL  * 60LL
                           + static_cast<long long>(hour)  * 60LL
                           + static_cast<long long>(minute);
    return totalMinutes;
}
