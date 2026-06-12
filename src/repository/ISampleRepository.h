// encoding: UTF-8 with BOM
#pragma once
#include <optional>
#include <vector>
#include <string>
#include "model/Sample.h"

class ISampleRepository
{
public:
    virtual ~ISampleRepository() = default;
    virtual void add(const Sample& sample) = 0;
    virtual std::optional<Sample> findById(const std::wstring& id) const = 0;
    virtual std::vector<Sample> findAll() const = 0;
    virtual void update(const Sample& sample) = 0;
    virtual void remove(const std::wstring& id) = 0;
};