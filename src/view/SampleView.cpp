// encoding: UTF-8 with BOM
#include "view/SampleView.h"
#include "util/ConsoleHelper.h"
#include "util/InputHelper.h"
#include <iostream>
#include <iomanip>

void SampleView::showMenu()
{
    ConsoleHelper::printDoubleDivider();
    ConsoleHelper::printCyan(L"  [1] 시료 관리\n");
    ConsoleHelper::printDivider();
    std::wcout << L"  [1] 시료 등록   [2] 시료 목록   [3] 시료 검색   [0] 위로\n";
    ConsoleHelper::printDivider();
}

void SampleView::showSampleTable(const std::vector<Sample>& samples,
                                  int page, int totalCount)
{
    ConsoleHelper::printDivider();
    // 컬럼 표시 너비(display columns): ID=10, 시료명=22, 평균생산시간=17(값8+단위9), 수율=8(값6+공백2)
    std::wcout << ConsoleHelper::padRight(L" ID", 10)
               << ConsoleHelper::padRight(L"시료명", 22)
               << ConsoleHelper::padRight(L"평균생산시간", 17)
               << ConsoleHelper::padRight(L"수율", 8)
               << L"현재재고\n";
    ConsoleHelper::printDivider();

    for (const auto& s : samples)
    {
        std::wcout << L" "
                   << ConsoleHelper::padRight(s.getSampleId(), 9)
                   << ConsoleHelper::padRight(s.getName(), 22)
                   << std::left << std::setw(8) << s.getAvgProdTime()
                   << L" min/ea  "
                   << std::left << std::setw(6) << s.getYield()
                   << L"  "
                   << s.getStock() << L" ea\n";
    }

    ConsoleHelper::printDivider();
    constexpr int PAGE_SIZE = 5;
    int totalPages = (totalCount + PAGE_SIZE - 1) / PAGE_SIZE;
    std::wcout << L"  페이지 " << page << L" / " << totalPages
               << L"   [N] 다음페이지  [P] 이전페이지\n";
}

void SampleView::showRegisterSuccess(const std::wstring& sampleId)
{
    ConsoleHelper::printGreen(L"등록 완료.\n");
    std::wcout << L"  시료 ID  " << sampleId << L"  이(가) 등록되었습니다.\n";
}

void SampleView::showSearchResult(const std::vector<Sample>& results)
{
    if (results.empty())
    {
        ConsoleHelper::printYellow(L"검색 결과가 없습니다.\n");
        return;
    }
    ConsoleHelper::printDivider();
    for (const auto& s : results)
    {
        std::wcout << L"  " << s.getSampleId()
                   << L"  " << s.getName()
                   << L"  재고: " << s.getStock() << L" ea\n";
    }
    ConsoleHelper::printDivider();
}

int SampleView::promptChoice()
{
    return InputHelper::promptInt(L"선택");
}