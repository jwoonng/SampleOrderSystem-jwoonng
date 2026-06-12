// encoding: UTF-8 with BOM
#pragma once
#include "IOrderRepository.h"

class JsonOrderRepository : public IOrderRepository
{
public:
    explicit JsonOrderRepository(std::wstring filePath);

    void add(const Order& order) override;
    std::optional<Order> findById(const std::wstring& id) const override;
    std::vector<Order> findAll() const override;
    std::vector<Order> findByStatus(OrderStatus status) const override;
    void update(const Order& order) override;
    void remove(const std::wstring& id) override;

private:
    std::wstring filePath_;
    std::vector<Order> orders_;

    void loadFromFile();
    void saveToFile() const;
    static std::wstring orderToJson(const Order& order);
    static Order parseOrderFromJson(const std::wstring& json);
    static std::wstring extractValue(const std::wstring& json, const std::wstring& key);
};