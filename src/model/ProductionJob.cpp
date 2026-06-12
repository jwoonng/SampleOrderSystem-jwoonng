// encoding: UTF-8 with BOM
#include "ProductionJob.h"
#include <sstream>

ProductionJob::ProductionJob(std::wstring jobId, std::wstring orderId, std::wstring sampleId,
                             int shortage, int actualQty, double totalTime, std::wstring startTime)
    : jobId(std::move(jobId))
    , orderId(std::move(orderId))
    , sampleId(std::move(sampleId))
    , shortage(shortage)
    , actualQty(actualQty)
    , totalTime(totalTime)
    , startTime(std::move(startTime))
{
}

std::wstring ProductionJob::getJobId() const
{
    return jobId;
}

std::wstring ProductionJob::getOrderId() const
{
    return orderId;
}

std::wstring ProductionJob::getSampleId() const
{
    return sampleId;
}

int ProductionJob::getShortage() const
{
    return shortage;
}

int ProductionJob::getActualQty() const
{
    return actualQty;
}

double ProductionJob::getTotalTime() const
{
    return totalTime;
}

std::wstring ProductionJob::getStartTime() const
{
    return startTime;
}

std::wstring ProductionJob::toString() const
{
    std::wostringstream oss;
    oss << L"ProductionJob[" << jobId << L"] order=" << orderId
        << L" sample=" << sampleId
        << L" shortage=" << shortage
        << L" actualQty=" << actualQty
        << L" totalTime=" << totalTime
        << L" startTime=" << startTime;
    return oss.str();
}