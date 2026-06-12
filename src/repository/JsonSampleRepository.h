// encoding: UTF-8 with BOM
#pragma once
#include "ISampleRepository.h"
#include <vector>
#include <string>

class JsonSampleRepository : public ISampleRepository
{
public:
    explicit JsonSampleRepository(std::wstring filePath);

    void add(const Sample& sample) override;
    std::optional<Sample> findById(const std::wstring& id) const override;
    std::vector<Sample> findAll() const override;
    void update(const Sample& sample) override;
    void remove(const std::wstring& id) override;

private:
    std::wstring filePath_;
    std::vector<Sample> samples_;

    void loadFromFile();
    void saveToFile() const;
    static std::wstring sampleToJson(const Sample& sample);
    static Sample parseSampleFromJson(const std::wstring& json);
    static std::wstring extractValue(const std::wstring& json, const std::wstring& key);
};