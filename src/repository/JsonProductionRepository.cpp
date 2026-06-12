// encoding: UTF-8 with BOM
#include "repository/JsonProductionRepository.h"
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
std::wstring JsonProductionRepository::extractValue(const std::wstring& json, const std::wstring& key)
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
    std::wstring content = readFileAsWString(filePath_);
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

    writeWStringToFile(tmpPath, oss.str());
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