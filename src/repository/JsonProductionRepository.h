// encoding: UTF-8 with BOM
#pragma once
#include "IProductionRepository.h"
#include <vector>
#include <string>

class JsonProductionRepository : public IProductionRepository
{
public:
    explicit JsonProductionRepository(std::wstring filePath);

    void enqueue(const ProductionJob& job) override;
    std::optional<ProductionJob> front() const override;
    void dequeue() override;
    std::vector<ProductionJob> findAll() const override;
    bool isEmpty() const override;

private:
    std::wstring filePath_;
    std::vector<ProductionJob> jobs_;

    void loadFromFile();
    void saveToFile() const;
    static std::wstring jobToJson(const ProductionJob& job);
    static ProductionJob parseJobFromJson(const std::wstring& json);
    static std::wstring extractValue(const std::wstring& json, const std::wstring& key);
};