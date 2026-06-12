// encoding: UTF-8 with BOM
#define NOMINMAX
#include "controller/SampleController.h"
#include "view/SampleView.h"
#include "util/ConsoleHelper.h"
#include "util/InputHelper.h"
#include "model/Sample.h"
#include <iostream>
#include <stdexcept>

void SampleController::run(SampleService& sampleService)
{
    SampleView view;
    while (true)
    {
        view.showMenu();
        int choice = view.promptChoice();

        if (choice == 1)      handleRegister(sampleService);
        else if (choice == 2) handleList(sampleService);
        else if (choice == 3) handleSearch(sampleService);
        else if (choice == 0) break;
        else ConsoleHelper::printRed(L"잘못된 선택입니다.\n");
    }
}

void SampleController::handleRegister(SampleService& sampleService)
{
    std::wstring id      = InputHelper::promptWString(L"시료 ID (예: S-001)");
    std::wstring name    = InputHelper::promptWString(L"시료 이름");
    double avgProdTime   = InputHelper::promptDouble(L"평균 생산시간(min)");
    double yield         = InputHelper::promptDouble(L"수율 (0.0~1.0)");
    int    stock         = InputHelper::promptInt(L"초기 재고");

    try
    {
        Sample sample(id, name, avgProdTime, yield, stock);
        sampleService.registerSample(sample);
        SampleView view;
        view.showRegisterSuccess(id);
    }
    catch (const std::invalid_argument& e)
    {
        ConsoleHelper::printRed(
            std::wstring(e.what(), e.what() + strlen(e.what())) + L"\n");
    }
}

void SampleController::handleList(SampleService& sampleService)
{
    auto all = sampleService.getAllSamples();
    int total = static_cast<int>(all.size());
    if (total == 0)
    {
        ConsoleHelper::printYellow(L"등록된 시료가 없습니다.\n");
        return;
    }

    int page = 1;
    SampleView view;
    while (true)
    {
        int startIdx = (page - 1) * PAGE_SIZE;
        int endIdx   = (startIdx + PAGE_SIZE < total) ? (startIdx + PAGE_SIZE) : total;
        std::vector<Sample> pageSlice(all.begin() + startIdx,
                                       all.begin() + endIdx);
        view.showSampleTable(pageSlice, page, total);

        std::wstring input = InputHelper::promptWString(L"[N] 다음 / [P] 이전 / [0] 위로");
        if (input == L"N" || input == L"n")
        {
            if (page * PAGE_SIZE < total) ++page;
            else ConsoleHelper::printYellow(L"마지막 페이지입니다.\n");
        }
        else if (input == L"P" || input == L"p")
        {
            if (page > 1) --page;
            else ConsoleHelper::printYellow(L"첫 번째 페이지입니다.\n");
        }
        else break;
    }
}

void SampleController::handleSearch(SampleService& sampleService)
{
    std::wstring keyword = InputHelper::promptWString(L"검색 키워드");
    auto results = sampleService.searchSamples(keyword);
    SampleView view;
    view.showSearchResult(results);
}