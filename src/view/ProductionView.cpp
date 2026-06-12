// encoding: UTF-8 with BOM
#include "view/ProductionView.h"
#include "util/ConsoleHelper.h"
#include "util/InputHelper.h"
#include <iostream>
#include <iomanip>

// 문자열 "YYYY-MM-DD HH:MM" 또는 "YYYY-MM-DD HH:MM:SS" 를 분 단위로 변환
static long long parseToMinutes(const std::wstring& dateTime)
{
    // "YYYY-MM-DD HH:MM" 최소 16자
    if (dateTime.size() < 16) return 0LL;
    long long hh = std::stoll(dateTime.substr(11, 2));
    long long mm = std::stoll(dateTime.substr(14, 2));
    return hh * 60LL + mm;
}

void ProductionView::showHeader()
{
    ConsoleHelper::printDoubleDivider();
    ConsoleHelper::printCyan(L"  [5] 생산라인 조회   FIFO 방식\n");
    ConsoleHelper::printDivider();
    std::wcout << L"  생산라인  1개 (단일 라인)\n";
}

void ProductionView::showCurrentJob(const ProductionJob& job,
                                     const Sample& sample,
                                     int elapsedQty,
                                     const std::wstring& currentTime)
{
    ConsoleHelper::printDivider();
    std::wcout << L"  주문번호  " << job.getOrderId()
               << L"   시료  " << sample.getName() << L"\n";

    int actualQty = job.getActualQty();
    int percent   = (actualQty > 0) ? (elapsedQty * 100 / actualQty) : 100;

    std::wcout << L"  진행  "
               << ConsoleHelper::buildProgressBar(percent, 10)
               << L"  " << percent << L"%\n";

    long long startMin     = parseToMinutes(job.getStartTime());
    long long completionMin = startMin + static_cast<long long>(job.getTotalTime());
    int hh = static_cast<int>((completionMin / 60) % 24);
    int mm = static_cast<int>(completionMin % 60);

    wchar_t timeBuf[16];
    swprintf_s(timeBuf, 16, L"%02d:%02d", hh, mm);

    std::wcout << L"  완료 예정  " << timeBuf << L"\n";
    ConsoleHelper::printDivider();
}

void ProductionView::showIdleMessage()
{
    ConsoleHelper::printDivider();
    ConsoleHelper::printYellow(L"  현재 처리 중인 작업이 없습니다.\n");
    ConsoleHelper::printDivider();
}

void ProductionView::showQueue(const std::vector<ProductionJob>& jobs,
                                const std::vector<Sample>& allSamples,
                                const std::wstring& currentTime)
{
    if (jobs.empty()) return;

    ConsoleHelper::printDivider();
    std::wcout << L"  대기 중인 주문 (FIFO 순)\n";
    ConsoleHelper::printDivider();
    std::wcout << std::left
               << std::setw(5)  << L"순서"
               << std::setw(22) << L"주문번호"
               << std::setw(16) << L"시료"
               << std::setw(8)  << L"주문량"
               << std::setw(8)  << L"부족분"
               << std::setw(10) << L"실생산량"
               << L"예상완료\n";
    ConsoleHelper::printDivider();

    long long accMin = parseToMinutes(currentTime);
    int seq = 1;
    for (const auto& j : jobs)
    {
        std::wstring sampleName = j.getSampleId();
        for (const auto& s : allSamples)
        {
            if (s.getSampleId() == j.getSampleId())
            {
                sampleName = s.getName();
                break;
            }
        }

        long long completionMin = accMin + static_cast<long long>(j.getTotalTime());
        int hh = static_cast<int>((completionMin / 60) % 24);
        int mm = static_cast<int>(completionMin % 60);
        wchar_t timeBuf[16];
        swprintf_s(timeBuf, 16, L"%02d:%02d", hh, mm);
        accMin = completionMin;

        std::wcout << L"  " << std::setw(3) << seq++
                   << std::setw(22) << j.getOrderId()
                   << std::setw(16) << sampleName
                   << std::setw(8)  << j.getActualQty()
                   << std::setw(8)  << j.getShortage()
                   << std::setw(10) << j.getActualQty()
                   << timeBuf << L"\n";
    }
    ConsoleHelper::printDivider();
}

int ProductionView::promptChoice()
{
    return InputHelper::promptInt(L"선택");
}