// encoding: UTF-8 with BOM
#include "repository/JsonSampleRepository.h"
#include "util/FileUtil.h"
#include <sstream>
#include <algorithm>
#include <windows.h>

// ─────────────────────────────────────────
//  헬퍼: JSON 문자열에서 key에 해당하는 값 추출
// ─────────────────────────────────────────
std::wstring JsonSampleRepository::extractValue(const std::wstring& json, const std::wstring& key)
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
//  Sample -> JSON 직렬화
// ─────────────────────────────────────────
std::wstring JsonSampleRepository::sampleToJson(const Sample& sample)
{
    std::wostringstream oss;
    oss << L"{";
    oss << L"\"sampleId\":\"" << sample.getSampleId() << L"\",";
    oss << L"\"name\":\"" << sample.getName() << L"\",";
    oss << L"\"avgProdTime\":" << sample.getAvgProdTime() << L",";
    oss << L"\"yield\":" << sample.getYield() << L",";
    oss << L"\"stock\":" << sample.getStock();
    oss << L"}";
    return oss.str();
}

// ─────────────────────────────────────────
//  JSON -> Sample 역직렬화
// ─────────────────────────────────────────
Sample JsonSampleRepository::parseSampleFromJson(const std::wstring& json)
{
    std::wstring sampleId  = extractValue(json, L"sampleId");
    std::wstring name      = extractValue(json, L"name");
    double avgProdTime     = std::stod(extractValue(json, L"avgProdTime"));
    double yieldVal        = std::stod(extractValue(json, L"yield"));
    int stock              = std::stoi(extractValue(json, L"stock"));
    return Sample(sampleId, name, avgProdTime, yieldVal, stock);
}

// ─────────────────────────────────────────
//  파일 로드
// ─────────────────────────────────────────
void JsonSampleRepository::loadFromFile()
{
    samples_.clear();
    std::wstring content = FileUtil::readUtf8FileAsWString(filePath_);
    if (content.empty()) return;

    auto start = content.find(L'{');
    while (start != std::wstring::npos)
    {
        auto end = content.find(L'}', start);
        if (end == std::wstring::npos) break;
        std::wstring objJson = content.substr(start, end - start + 1);
        samples_.push_back(parseSampleFromJson(objJson));
        start = content.find(L'{', end + 1);
    }
}

// ─────────────────────────────────────────
//  파일 저장 (원자적: .tmp -> rename)
// ─────────────────────────────────────────
void JsonSampleRepository::saveToFile() const
{
    std::wstring tmpPath = filePath_ + L".tmp";

    std::wostringstream oss;
    oss << L"[\n";
    for (size_t i = 0; i < samples_.size(); ++i)
    {
        oss << L"  " << sampleToJson(samples_[i]);
        if (i + 1 < samples_.size()) oss << L",";
        oss << L"\n";
    }
    oss << L"]";

    FileUtil::writeWStringToUtf8File(tmpPath, oss.str());
    _wrename(tmpPath.c_str(), filePath_.c_str());
}

// ─────────────────────────────────────────
//  생성자
// ─────────────────────────────────────────
JsonSampleRepository::JsonSampleRepository(std::wstring filePath)
    : filePath_(std::move(filePath))
{
    loadFromFile();
}

// ─────────────────────────────────────────
//  CRUD 구현
// ─────────────────────────────────────────
void JsonSampleRepository::add(const Sample& sample)
{
    samples_.push_back(sample);
    saveToFile();
}

std::optional<Sample> JsonSampleRepository::findById(const std::wstring& id) const
{
    for (const auto& s : samples_)
    {
        if (s.getSampleId() == id)
            return s;
    }
    return std::nullopt;
}

std::vector<Sample> JsonSampleRepository::findAll() const
{
    return samples_;
}

void JsonSampleRepository::update(const Sample& sample)
{
    for (auto& s : samples_)
    {
        if (s.getSampleId() == sample.getSampleId())
        {
            s = sample;
            break;
        }
    }
    saveToFile();
}

void JsonSampleRepository::remove(const std::wstring& id)
{
    auto it = std::remove_if(samples_.begin(), samples_.end(),
        [&id](const Sample& s) { return s.getSampleId() == id; });
    samples_.erase(it, samples_.end());
    saveToFile();
}