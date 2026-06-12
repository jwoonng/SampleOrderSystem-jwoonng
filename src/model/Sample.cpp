// encoding: UTF-8 with BOM
#include "Sample.h"
#include <sstream>

Sample::Sample(std::wstring sampleId, std::wstring name,
               double avgProdTime, double yield, int stock)
    : sampleId(std::move(sampleId))
    , name(std::move(name))
    , avgProdTime(avgProdTime)
    , yield(yield)
    , stock(stock)
{
}

std::wstring Sample::getSampleId() const
{
    return sampleId;
}

std::wstring Sample::getName() const
{
    return name;
}

double Sample::getAvgProdTime() const
{
    return avgProdTime;
}

double Sample::getYield() const
{
    return yield;
}

int Sample::getStock() const
{
    return stock;
}

std::wstring Sample::toString() const
{
    std::wostringstream oss;
    oss << L"Sample[" << sampleId << L"] " << name
        << L" avgProdTime=" << avgProdTime
        << L" yield=" << yield
        << L" stock=" << stock;
    return oss.str();
}