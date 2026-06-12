#pragma once
// encoding: UTF-8 with BOM
#include <string>

class ProductionJob
{
public:
    ProductionJob(std::wstring jobId, std::wstring orderId, std::wstring sampleId,
                  int shortage, int actualQty, double totalTime, std::wstring startTime);

    std::wstring getJobId()     const;
    std::wstring getOrderId()   const;
    std::wstring getSampleId()  const;
    int          getShortage()  const;
    int          getActualQty() const;
    double       getTotalTime() const;
    std::wstring getStartTime() const;

    std::wstring toString() const;

private:
    std::wstring jobId;
    std::wstring orderId;
    std::wstring sampleId;
    int          shortage;
    int          actualQty;
    double       totalTime;
    std::wstring startTime;
};