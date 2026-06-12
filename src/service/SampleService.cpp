// encoding: UTF-8 with BOM
#include "service/SampleService.h"
#include <stdexcept>
#include <cwctype>

// S- 로 시작하고 뒤에 숫자가 1개 이상인지 수동 파싱으로 검사
bool SampleService::isValidSampleId(const std::wstring& id)
{
    if (id.size() < 3) return false;
    if (id[0] != L'S' || id[1] != L'-') return false;
    for (size_t i = 2; i < id.size(); ++i)
    {
        if (!iswdigit(id[i])) return false;
    }
    return true;
}

void SampleService::validateSample(const Sample& sample)
{
    if (!isValidSampleId(sample.getSampleId()))
        throw std::invalid_argument("시료 ID 형식이 올바르지 않습니다. (예: S-001)");

    if (sample.getYield() <= 0.0 || sample.getYield() > 1.0)
        throw std::invalid_argument("수율은 0.0 초과 1.0 이하여야 합니다.");

    if (sample.getAvgProdTime() <= 0.0)
        throw std::invalid_argument("평균 생산시간은 0보다 커야 합니다.");
}

SampleService::SampleService(ISampleRepository& repo)
    : repo_(repo)
{
}

void SampleService::registerSample(const Sample& sample)
{
    validateSample(sample);

    auto existing = repo_.findById(sample.getSampleId());
    if (existing.has_value())
        throw std::invalid_argument("이미 존재하는 시료 ID입니다.");

    repo_.add(sample);
}

std::vector<Sample> SampleService::getAllSamples() const
{
    return repo_.findAll();
}

std::optional<Sample> SampleService::findSampleById(const std::wstring& id) const
{
    return repo_.findById(id);
}

std::vector<Sample> SampleService::searchSamples(const std::wstring& keyword) const
{
    auto all = repo_.findAll();
    std::vector<Sample> result;

    for (const auto& sample : all)
    {
        bool matchId   = sample.getSampleId().find(keyword) != std::wstring::npos;
        bool matchName = sample.getName().find(keyword)     != std::wstring::npos;
        if (matchId || matchName)
            result.push_back(sample);
    }

    return result;
}