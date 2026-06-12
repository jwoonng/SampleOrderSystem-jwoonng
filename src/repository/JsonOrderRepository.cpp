// encoding: UTF-8 with BOM
#include "repository/JsonOrderRepository.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <windows.h>

// ─────────────────────────────────────────
//  파일 읽기: UTF-8 -> wstring
// ─────────────────────────────────────────
static std::wstring readFileAsWString(const std::wstring& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
        return L"";

    std::string bytes((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
    file.close();

    if (bytes.empty())
        return L"";

    if (bytes.size() >= 3 &&
        static_cast<unsigned char>(bytes[0]) == 0xEF &&
        static_cast<unsigned char>(bytes[1]) == 0xBB &&
        static_cast<unsigned char>(bytes[2]) == 0xBF)
    {
        bytes = bytes.substr(3);
    }

    int wlen = MultiByteToWideChar(CP_UTF8, 0, bytes.c_str(), static_cast<int>(bytes.size()), nullptr, 0);
    if (wlen <= 0) return L"";
    std::wstring result(wlen, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, bytes.c_str(), static_cast<int>(bytes.size()), &result[0], wlen);
    return result;
}

// ─────────────────────────────────────────
//  파일 쓰기: wstring -> UTF-8
// ─────────────────────────────────────────
static bool writeWStringToFile(const std::wstring& path, const std::wstring& content)
{
    int mlen = WideCharToMultiByte(CP_UTF8, 0, content.c_str(), static_cast<int>(content.size()), nullptr, 0, nullptr, nullptr);
    if (mlen <= 0) return false;
    std::string bytes(mlen, '\0');
    WideCharToMultiByte(CP_UTF8, 0, content.c_str(), static_cast<int>(content.size()), &bytes[0], mlen, nullptr, nullptr);

    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) return false;
    file.write(bytes.c_str(), bytes.size());
    return true;
}

// ─────────────────────────────────────────
//  헬퍼: JSON 문자열에서 key에 해당하는 값 추출
// ─────────────────────────────────────────
std::wstring JsonOrderRepository::extractValue(const std::wstring& json, const std::wstring& key)
{
    std::wstring searchKey = L"\"" + key + L"\":";
    auto pos = json.find(searchKey);
    if (pos == std::wstring::npos)
        return L"";

    pos += searchKey.size();
    while (pos < json.size() && json[pos] == L' ') ++pos;

    if (json[pos] == L'"')
    {
        ++pos;
        std::wstring result;
        while (pos < json.size() && json[pos] != L'"')
        {
            if (json[pos] == L'\\' && pos + 1 < json.size())
            {
                ++pos;
                if (json[pos] == L'"') result += L'"';
                else if (json[pos] == L'\\') result += L'\\';
                else if (json[pos] == L'n') result += L'\n';
                else result += json[pos];
            }
            else
            {
                result += json[pos];
            }
            ++pos;
        }
        return result;
    }
    else
    {
        std::wstring result;
        while (pos < json.size() && json[pos] != L',' && json[pos] != L'}')
            result += json[pos++];
        return result;
    }
}

// ─────────────────────────────────────────
//  Order -> JSON 직렬화
// ─────────────────────────────────────────
std::wstring JsonOrderRepository::orderToJson(const Order& order)
{
    std::wostringstream oss;
    oss << L"{";
    oss << L"\"orderId\":\"" << order.getOrderId() << L"\",";
    oss << L"\"sampleId\":\"" << order.getSampleId() << L"\",";
    oss << L"\"customerName\":\"" << order.getCustomerName() << L"\",";
    oss << L"\"quantity\":" << order.getQuantity() << L",";
    oss << L"\"status\":" << static_cast<int>(order.getStatus()) << L",";
    oss << L"\"createdAt\":\"" << order.getCreatedAt() << L"\"";
    oss << L"}";
    return oss.str();
}

// ─────────────────────────────────────────
//  JSON -> Order 역직렬화
// ─────────────────────────────────────────
Order JsonOrderRepository::parseOrderFromJson(const std::wstring& json)
{
    std::wstring orderId      = extractValue(json, L"orderId");
    std::wstring sampleId     = extractValue(json, L"sampleId");
    std::wstring customerName = extractValue(json, L"customerName");
    int quantity              = std::stoi(extractValue(json, L"quantity"));
    int statusInt             = std::stoi(extractValue(json, L"status"));
    std::wstring createdAt    = extractValue(json, L"createdAt");

    Order order(orderId, sampleId, customerName, quantity, createdAt);
    order.setStatus(static_cast<OrderStatus>(statusInt));
    return order;
}

// ─────────────────────────────────────────
//  파일 로드
// ─────────────────────────────────────────
void JsonOrderRepository::loadFromFile()
{
    orders_.clear();
    std::wstring content = readFileAsWString(filePath_);
    if (content.empty()) return;

    auto start = content.find(L'{');
    while (start != std::wstring::npos)
    {
        auto end = content.find(L'}', start);
        if (end == std::wstring::npos) break;
        std::wstring objJson = content.substr(start, end - start + 1);
        orders_.push_back(parseOrderFromJson(objJson));
        start = content.find(L'{', end + 1);
    }
}

// ─────────────────────────────────────────
//  파일 저장 (원자적)
// ─────────────────────────────────────────
void JsonOrderRepository::saveToFile() const
{
    std::wstring tmpPath = filePath_ + L".tmp";

    std::wostringstream oss;
    oss << L"[\n";
    for (size_t i = 0; i < orders_.size(); ++i)
    {
        oss << L"  " << orderToJson(orders_[i]);
        if (i + 1 < orders_.size()) oss << L",";
        oss << L"\n";
    }
    oss << L"]";

    writeWStringToFile(tmpPath, oss.str());
    _wrename(tmpPath.c_str(), filePath_.c_str());
}

// ─────────────────────────────────────────
//  생성자
// ─────────────────────────────────────────
JsonOrderRepository::JsonOrderRepository(std::wstring filePath)
    : filePath_(std::move(filePath))
{
    loadFromFile();
}

// ─────────────────────────────────────────
//  CRUD 구현
// ─────────────────────────────────────────
void JsonOrderRepository::add(const Order& order)
{
    orders_.push_back(order);
    saveToFile();
}

std::optional<Order> JsonOrderRepository::findById(const std::wstring& id) const
{
    for (const auto& o : orders_)
    {
        if (o.getOrderId() == id)
            return o;
    }
    return std::nullopt;
}

std::vector<Order> JsonOrderRepository::findAll() const
{
    return orders_;
}

std::vector<Order> JsonOrderRepository::findByStatus(OrderStatus status) const
{
    std::vector<Order> result;
    for (const auto& o : orders_)
    {
        if (o.getStatus() == status)
            result.push_back(o);
    }
    return result;
}

void JsonOrderRepository::update(const Order& order)
{
    for (auto& o : orders_)
    {
        if (o.getOrderId() == order.getOrderId())
        {
            o = order;
            break;
        }
    }
    saveToFile();
}

void JsonOrderRepository::remove(const std::wstring& id)
{
    auto it = std::remove_if(orders_.begin(), orders_.end(),
        [&id](const Order& o) { return o.getOrderId() == id; });
    orders_.erase(it, orders_.end());
    saveToFile();
}