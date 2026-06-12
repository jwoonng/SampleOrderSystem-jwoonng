#pragma once
// encoding: UTF-8 with BOM
#include <optional>
#include <vector>
#include <string>
#include "repository/ISampleRepository.h"
#include "model/Sample.h"

class SampleService
{
public:
    explicit SampleService(ISampleRepository& repo);

    void registerSample(const Sample& sample);
    std::vector<Sample> getAllSamples() const;
    std::optional<Sample> findSampleById(const std::wstring& id) const;
    std::vector<Sample> searchSamples(const std::wstring& keyword) const;

private:
    ISampleRepository& repo_;

    static void validateSample(const Sample& sample);
    static bool isValidSampleId(const std::wstring& id);
};