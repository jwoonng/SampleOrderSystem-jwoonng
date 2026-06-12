// encoding: UTF-8 with BOM
#pragma once
#include <optional>
#include <vector>
#include "model/ProductionJob.h"

class IProductionRepository
{
public:
    virtual ~IProductionRepository() = default;
    virtual void enqueue(const ProductionJob& job) = 0;
    virtual std::optional<ProductionJob> front() const = 0;
    virtual void dequeue() = 0;
    virtual std::vector<ProductionJob> findAll() const = 0;
    virtual bool isEmpty() const = 0;
};