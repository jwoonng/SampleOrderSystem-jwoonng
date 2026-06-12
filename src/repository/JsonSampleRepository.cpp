// encoding: UTF-8 with BOM
#include "repository/JsonSampleRepository.h"
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

    // BOM 제거 (EF BB BF)
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
std::wstring JsonSampleRepository::extractValue(const std::wstring& json, const std::wstring& key)
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
    std::wstring content = readFileAsWString(filePath_);
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

    writeWStringToFile(tmpPath, oss.str());
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