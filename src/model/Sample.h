#pragma once
// encoding: UTF-8 with BOM
#include <string>

class Sample
{
public:
    Sample(std::wstring sampleId, std::wstring name,
           double avgProdTime, double yield, int stock);

    std::wstring getSampleId()    const;
    std::wstring getName()        const;
    double       getAvgProdTime() const;
    double       getYield()       const;
    int          getStock()       const;

    std::wstring toString()       const;

private:
    std::wstring sampleId;
    std::wstring name;
    double       avgProdTime;
    double       yield;
    int          stock;
};