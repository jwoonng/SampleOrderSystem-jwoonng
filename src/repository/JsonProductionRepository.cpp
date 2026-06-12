// encoding: UTF-8 with BOM
#include "repository/JsonProductionRepository.h"
#include "util/FileUtil.h"
#include <sstream>
#include <algorithm>
#include <windows.h>

// ─────────────────────────────────────────
//  헬퍼: JSON 문자열에서 key에 해당하는 값 추출
// ─────────────────────────────────────────
std::wstring JsonProductionRepository::extractValue(const std::wstring& json, const std::wstring& key)
{
    std::wstring searchKey = L"\"" + key + L"\":";
    auto pos = json.find(searchKey);
    if (pos == std::wstring::npos)
        return L"";

    pos += searchKey.size();
    while (pos < json.size() && json[pos] == L' ') ++pos;

    // 숫자 값: 쉼표 또는 닫는 중괄호 전까지 추출
    if (json[pos] != L'"')
    {
        std::wstring numericValue;
        while (pos < json.size() && json[pos] != L',' && json[pos] != L'}')
            numericValue += json[pos++];
        return numericValue;
    }

    // 문자열 값: 이스케이프 처리 포함
    ++pos;
    std::wstring stringValue;
    while (pos < json.size() && json[pos] != L'"')
    {
        if (json[pos] == L'\\' && pos + 1 < json.size())
        {
            ++pos;
            if (json[pos] == L'"')       stringValue += L'"';
            else if (json[pos] == L'\\') stringValue += L'\\';
            else if (json[pos] == L'n')  stringValue += L'\n';
            else                         stringValue += json[pos];
        }
        else
        {
            stringValue += json[pos];
        }
        ++pos;
    }
    return stringValue;
}

// ─────────────────────────────────────────
//  ProductionJob -> JSON 직렬화
// ─────────────────────────────────────────
std::wstring JsonProductionRepository::jobToJson(const ProductionJob& job)
{
    std::wostringstream oss;
    oss << L"{";
    oss << L"\"jobId\":\"" << job.getJobId() << L"\",";
    oss << L"\"orderId\":\"" << job.getOrderId() << L"\",";
    oss << L"\"sampleId\":\"" << job.getSampleId() << L"\",";
    oss << L"\"shortage\":" << job.getShortage() << L",";
    oss << L"\"actualQty\":" << job.getActualQty() << L",";
    oss << L"\"totalTime\":" << job.getTotalTime() << L",";
    oss << L"\"startTime\":\"" << job.getStartTime() << L"\"";
    oss << L"}";
    return oss.str();
}

// ─────────────────────────────────────────
//  JSON -> ProductionJob 역직렬화
// ─────────────────────────────────────────
ProductionJob JsonProductionRepository::parseJobFromJson(const std::wstring& json)
{
    std::wstring jobId     = extractValue(json, L"jobId");
    std::wstring orderId   = extractValue(json, L"orderId");
    std::wstring sampleId  = extractValue(json, L"sampleId");
    int shortage           = std::stoi(extractValue(json, L"shortage"));
    int actualQty          = std::stoi(extractValue(json, L"actualQty"));
    double totalTime       = std::stod(extractValue(json, L"totalTime"));
    std::wstring startTime = extractValue(json, L"startTime");

    return ProductionJob(jobId, orderId, sampleId, shortage, actualQty, totalTime, startTime);
}

// ─────────────────────────────────────────
//  파일 로드
// ─────────────────────────────────────────
void JsonProductionRepository::loadFromFile()
{
    jobs_.clear();
    std::wstring content = FileUtil::readUtf8FileAsWString(filePath_);
    if (content.empty()) return;

    auto start = content.find(L'{');
    while (start != std::wstring::npos)
    {
        auto end = content.find(L'}', start);
        if (end == std::wstring::npos) break;
        std::wstring objJson = content.substr(start, end - start + 1);
        jobs_.push_back(parseJobFromJson(objJson));
        start = content.find(L'{', end + 1);
    }
}

// ─────────────────────────────────────────
//  파일 저장 (원자적)
// ─────────────────────────────────────────
void JsonProductionRepository::saveToFile() const
{
    std::wstring tmpPath = filePath_ + L".tmp";

    std::wostringstream oss;
    oss << L"[\n";
    for (size_t i = 0; i < jobs_.size(); ++i)
    {
        oss << L"  " << jobToJson(jobs_[i]);
        if (i + 1 < jobs_.size()) oss << L",";
        oss << L"\n";
    }
    oss << L"]";

    FileUtil::writeWStringToUtf8File(tmpPath, oss.str());
    _wrename(tmpPath.c_str(), filePath_.c_str());
}

// ─────────────────────────────────────────
//  생성자
// ─────────────────────────────────────────
JsonProductionRepository::JsonProductionRepository(std::wstring filePath)
    : filePath_(std::move(filePath))
{
    loadFromFile();
}

// ─────────────────────────────────────────
//  FIFO 큐 구현
// ─────────────────────────────────────────
void JsonProductionRepository::enqueue(const ProductionJob& job)
{
    jobs_.push_back(job);
    saveToFile();
}

std::optional<ProductionJob> JsonProductionRepository::front() const
{
    if (jobs_.empty())
        return std::nullopt;
    return jobs_.front();
}

void JsonProductionRepository::dequeue()
{
    if (!jobs_.empty())
    {
        jobs_.erase(jobs_.begin());
        saveToFile();
    }
}

std::vector<ProductionJob> JsonProductionRepository::findAll() const
{
    return jobs_;
}

bool JsonProductionRepository::isEmpty() const
{
    return jobs_.empty();
}