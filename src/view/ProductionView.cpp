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
    std::wcout << L"  주문번호  " << job.getOrderId() << L"\n";
    std::wcout << L"  시료명    " << sample.getName() << L"\n";

    // 진행률은 주문에 필요한 수량(shortage) 기준으로 표시
    // shortage 달성(= 주문 충족) 시 100%에서 유지, 이후 잉여분 생산은 계속
    int progressTarget = (job.getShortage() > 0) ? job.getShortage() : job.getActualQty();
    int percent = (progressTarget > 0) ? (elapsedQty * 100 / progressTarget) : 100;
    if (percent > 100) percent = 100;

    std::wcout << L"  진행      "
               << ConsoleHelper::buildProgressBar(percent, 10)
               << L"  " << percent << L"%\n";

    long long startMin     = parseToMinutes(job.getStartTime());
    long long completionMin = startMin + static_cast<long long>(job.getTotalTime());
    int hh = static_cast<int>((completionMin / 60) % 24);
    int mm = static_cast<int>(completionMin % 60);

    wchar_t timeBuf[16];
    swprintf_s(timeBuf, 16, L"%02d:%02d", hh, mm);

    std::wcout << L"  완료예정  " << timeBuf << L"\n";
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
    // 컬럼 표시 너비: 순서=5, 주문번호=20, 시료명=12, 부족분=6, 실생산량=8, 예상완료=나머지
    std::wcout << ConsoleHelper::padRight(L"순서", 5)
               << ConsoleHelper::padRight(L"주문번호", 20)
               << ConsoleHelper::padRight(L"시료명", 12)
               << ConsoleHelper::padRight(L"부족분", 6)
               << ConsoleHelper::padRight(L"실생산량", 8)
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

        std::wcout << L"  " << std::left << std::setw(3) << seq++
                   << ConsoleHelper::padRight(j.getOrderId(), 20)
                   << ConsoleHelper::padRight(sampleName, 12)
                   << std::left << std::setw(4) << j.getShortage()  << L"  "
                   << std::left << std::setw(6) << j.getActualQty() << L"  "
                   << timeBuf << L"\n";
    }
    ConsoleHelper::printDivider();
}

int ProductionView::promptChoice()
{
    int choice = InputHelper::promptInt(L"선택");
    std::wcout << L"\n";
    return choice;
}