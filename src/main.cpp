// [임시 UI - Phase 7에서 MVC로 교체]
// encoding: UTF-8 with BOM
#include <iostream>
#include <vector>
#include <windows.h>
#include "util/ConsoleUtil.h"
#include "repository/JsonSampleRepository.h"
#include "service/SampleService.h"

static void printSampleTable(const std::vector<Sample>& samples)
{
    std::wcout << L"------------------------------------------------------------\n";
    std::wcout << L" ID       | 이름             | 생산시간 | 수율   | 재고\n";
    std::wcout << L"------------------------------------------------------------\n";
    for (const auto& s : samples)
    {
        std::wcout << L" " << s.getSampleId()
                   << L" | " << s.getName()
                   << L" | " << s.getAvgProdTime()
                   << L" | " << s.getYield()
                   << L" | " << s.getStock()
                   << L"\n";
    }
    std::wcout << L"------------------------------------------------------------\n";
}

int main()
{
    ConsoleUtil::initEncoding();

    // data/ 디렉터리가 없으면 생성
    CreateDirectoryW(L"data", nullptr);

    JsonSampleRepository repo(L"data/samples.json");
    SampleService service(repo);

    int choice = 0;
    while (true)
    {
        std::wcout << L"\n=== 시료 관리 (임시 UI) ===\n";
        std::wcout << L"1. 시료 등록\n";
        std::wcout << L"2. 시료 전체 조회\n";
        std::wcout << L"3. 시료 ID/이름 검색\n";
        std::wcout << L"4. 종료\n";
        std::wcout << L"선택 > ";
        std::wcin >> choice;

        if (choice == 1)
        {
            std::wstring id, name;
            double avgProdTime = 0.0, yield = 0.0;
            int stock = 0;

            std::wcout << L"시료 ID (예: S-001): ";
            std::wcin >> id;
            std::wcout << L"시료 이름: ";
            std::wcin >> name;
            std::wcout << L"평균 생산시간: ";
            std::wcin >> avgProdTime;
            std::wcout << L"수율 (0.0~1.0): ";
            std::wcin >> yield;
            std::wcout << L"재고: ";
            std::wcin >> stock;

            try
            {
                Sample sample(id, name, avgProdTime, yield, stock);
                service.registerSample(sample);
                std::wcout << L"[성공] 시료가 등록되었습니다.\n";
            }
            catch (const std::invalid_argument& e)
            {
                std::wcout << L"[오류] " << std::wstring(e.what(), e.what() + strlen(e.what())) << L"\n";
            }
        }
        else if (choice == 2)
        {
            auto samples = service.getAllSamples();
            if (samples.empty())
                std::wcout << L"등록된 시료가 없습니다.\n";
            else
                printSampleTable(samples);
        }
        else if (choice == 3)
        {
            std::wstring keyword;
            std::wcout << L"검색 키워드: ";
            std::wcin >> keyword;

            auto results = service.searchSamples(keyword);
            if (results.empty())
                std::wcout << L"검색 결과가 없습니다.\n";
            else
                printSampleTable(results);
        }
        else if (choice == 4)
        {
            std::wcout << L"종료합니다.\n";
            break;
        }
        else
        {
            std::wcout << L"잘못된 선택입니다.\n";
        }
    }

    return 0;
}