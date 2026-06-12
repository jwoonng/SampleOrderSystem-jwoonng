// [임시 UI - Phase 7에서 MVC로 교체]
// encoding: UTF-8 with BOM
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <windows.h>
#include "util/ConsoleUtil.h"
#include "repository/JsonSampleRepository.h"
#include "repository/JsonOrderRepository.h"
#include "repository/JsonProductionRepository.h"
#include "service/SampleService.h"
#include "service/OrderService.h"
#include "service/ProductionService.h"

// ──────────────────────────────────────────────
//  출력 헬퍼
// ──────────────────────────────────────────────

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

static void printOrderTable(const std::vector<Order>& orders)
{
    std::wcout << L"------------------------------------------------------------------\n";
    std::wcout << L" 주문ID      | 시료ID  | 고객명   | 수량 | 상태\n";
    std::wcout << L"------------------------------------------------------------------\n";
    for (const auto& o : orders)
    {
        std::wstring statusStr;
        switch (o.getStatus())
        {
        case OrderStatus::Reserved:  statusStr = L"Reserved";  break;
        case OrderStatus::Confirmed: statusStr = L"Confirmed"; break;
        case OrderStatus::Producing: statusStr = L"Producing"; break;
        case OrderStatus::Rejected:  statusStr = L"Rejected";  break;
        case OrderStatus::Release:   statusStr = L"Release";   break;
        }
        std::wcout << L" " << o.getOrderId()
                   << L" | " << o.getSampleId()
                   << L" | " << o.getCustomerName()
                   << L" | " << o.getQuantity()
                   << L" | " << statusStr
                   << L"\n";
    }
    std::wcout << L"------------------------------------------------------------------\n";
}

// 간단한 타임스탬프 기반 ID 생성 (Phase 7에서 정식 발번 로직으로 교체)
static std::wstring generateTimestampId(const std::wstring& prefix)
{
    auto now = std::chrono::system_clock::now().time_since_epoch().count();
    return prefix + std::to_wstring(now % 100000);
}

// ──────────────────────────────────────────────
//  시료 관리 서브메뉴
// ──────────────────────────────────────────────

static void runSampleMenu(SampleService& sampleService)
{
    int choice = 0;
    while (true)
    {
        std::wcout << L"\n=== 시료 관리 ===\n";
        std::wcout << L"1. 시료 등록\n";
        std::wcout << L"2. 시료 전체 조회\n";
        std::wcout << L"3. 시료 ID/이름 검색\n";
        std::wcout << L"0. 뒤로\n";
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
                sampleService.registerSample(sample);
                std::wcout << L"[성공] 시료가 등록되었습니다.\n";
            }
            catch (const std::invalid_argument& e)
            {
                std::wcout << L"[오류] "
                           << std::wstring(e.what(), e.what() + strlen(e.what()))
                           << L"\n";
            }
        }
        else if (choice == 2)
        {
            auto samples = sampleService.getAllSamples();
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

            auto results = sampleService.searchSamples(keyword);
            if (results.empty())
                std::wcout << L"검색 결과가 없습니다.\n";
            else
                printSampleTable(results);
        }
        else if (choice == 0)
        {
            break;
        }
        else
        {
            std::wcout << L"잘못된 선택입니다.\n";
        }
    }
}

// ──────────────────────────────────────────────
//  주문 관리 서브메뉴
// ──────────────────────────────────────────────

static void runOrderMenu(OrderService& orderService, SampleService& sampleService)
{
    int choice = 0;
    while (true)
    {
        std::wcout << L"\n=== 주문 관리 ===\n";
        std::wcout << L"1. 주문 등록\n";
        std::wcout << L"2. 주문 목록 조회 (Reserved)\n";
        std::wcout << L"3. 주문 승인\n";
        std::wcout << L"4. 주문 거절\n";
        std::wcout << L"5. 출고 처리\n";
        std::wcout << L"6. 확정 주문 조회 (Confirmed)\n";
        std::wcout << L"0. 뒤로\n";
        std::wcout << L"선택 > ";
        std::wcin >> choice;

        if (choice == 1)
        {
            std::wstring sampleId, customerName;
            int quantity = 0;

            std::wcout << L"시료 ID: ";
            std::wcin >> sampleId;
            std::wcout << L"고객 이름: ";
            std::wcin >> customerName;
            std::wcout << L"주문 수량: ";
            std::wcin >> quantity;

            std::wstring orderId   = generateTimestampId(L"ORD-");
            std::wstring createdAt = L"2026-06-12";   // Phase 7에서 실제 날짜로 교체

            try
            {
                Order order = orderService.placeOrder(sampleId, customerName,
                                                      quantity, orderId, createdAt);
                std::wcout << L"[성공] 주문이 등록되었습니다. 주문 ID: "
                           << order.getOrderId() << L"\n";
            }
            catch (const std::invalid_argument& e)
            {
                std::wcout << L"[오류] "
                           << std::wstring(e.what(), e.what() + strlen(e.what()))
                           << L"\n";
            }
        }
        else if (choice == 2)
        {
            auto orders = orderService.getReservedOrders();
            if (orders.empty())
                std::wcout << L"대기 중인 주문이 없습니다.\n";
            else
                printOrderTable(orders);
        }
        else if (choice == 3)
        {
            std::wstring orderId;
            std::wcout << L"승인할 주문 ID: ";
            std::wcin >> orderId;

            // 승인 전: 주문 정보 + 현재 재고 표시
            auto orderOpt = orderService.findOrderById(orderId);
            if (!orderOpt.has_value())
            {
                std::wcout << L"[오류] 존재하지 않는 주문 ID입니다.\n";
            }
            else
            {
                const Order& order = orderOpt.value();
                auto sampleOpt = sampleService.findSampleById(order.getSampleId());
                int currentStock = sampleOpt.has_value() ? sampleOpt->getStock() : 0;

                std::wcout << L"\n[재고 확인]\n";
                std::wcout << L"  시료 ID   : " << order.getSampleId() << L"\n";
                std::wcout << L"  현재 재고 : " << currentStock << L"개\n";
                std::wcout << L"  주문 수량 : " << order.getQuantity() << L"개\n";
                if (currentStock >= order.getQuantity())
                    std::wcout << L"  판정      : 재고 충분 → 확정(Confirmed) 처리 예정\n";
                else
                    std::wcout << L"  판정      : 재고 부족 (" << (order.getQuantity() - currentStock)
                               << L"개 부족) → 생산 등록(Producing) 처리 예정\n";

                std::wstring jobId     = L"JOB-" + orderId;
                std::wstring startTime = L"2026-06-12 09:00";   // Phase 7에서 실제 시각으로 교체

                try
                {
                    orderService.approveOrder(orderId, jobId, startTime);

                    // 승인 후: 실제 결과 상태 표시
                    auto updatedOpt = orderService.findOrderById(orderId);
                    if (updatedOpt.has_value())
                    {
                        OrderStatus status = updatedOpt->getStatus();
                        if (status == OrderStatus::Confirmed)
                            std::wcout << L"[성공] 재고 차감 완료 → 주문이 확정(Confirmed) 상태로 변경되었습니다.\n";
                        else if (status == OrderStatus::Producing)
                            std::wcout << L"[성공] 생산 라인 등록 완료 → 주문이 생산 중(Producing) 상태로 변경되었습니다.\n";
                    }
                }
                catch (const std::invalid_argument& e)
                {
                    std::wcout << L"[오류] "
                               << std::wstring(e.what(), e.what() + strlen(e.what()))
                               << L"\n";
                }
            }
        }
        else if (choice == 4)
        {
            std::wstring orderId;
            std::wcout << L"거절할 주문 ID: ";
            std::wcin >> orderId;

            try
            {
                orderService.rejectOrder(orderId);
                std::wcout << L"[성공] 주문이 거절되었습니다.\n";
            }
            catch (const std::invalid_argument& e)
            {
                std::wcout << L"[오류] "
                           << std::wstring(e.what(), e.what() + strlen(e.what()))
                           << L"\n";
            }
        }
        else if (choice == 5)
        {
            std::wstring orderId;
            std::wcout << L"출고할 주문 ID: ";
            std::wcin >> orderId;

            try
            {
                orderService.releaseOrder(orderId);
                std::wcout << L"[성공] 출고 처리되었습니다.\n";
            }
            catch (const std::invalid_argument& e)
            {
                std::wcout << L"[오류] "
                           << std::wstring(e.what(), e.what() + strlen(e.what()))
                           << L"\n";
            }
        }
        else if (choice == 6)
        {
            auto orders = orderService.getConfirmedOrders();
            if (orders.empty())
                std::wcout << L"확정된 주문이 없습니다.\n";
            else
                printOrderTable(orders);
        }
        else if (choice == 0)
        {
            break;
        }
        else
        {
            std::wcout << L"잘못된 선택입니다.\n";
        }
    }
}

// ──────────────────────────────────────────────
//  생산라인 관리 서브메뉴
// ──────────────────────────────────────────────

static void runProductionMenu(ProductionService& productionService)
{
    int choice = 0;
    while (true)
    {
        std::wcout << L"\n=== 생산라인 관리 ===\n";
        std::wcout << L"1. 생산 큐 조회\n";
        std::wcout << L"2. 현재 생산 작업 확인\n";
        std::wcout << L"3. 생산 완료 체크\n";
        std::wcout << L"0. 뒤로\n";
        std::wcout << L"선택 > ";
        std::wcin >> choice;

        if (choice == 1)
        {
            auto queue = productionService.getQueue();
            if (queue.empty())
                std::wcout << L"생산 큐가 비어 있습니다.\n";
            else
            {
                std::wcout << L"[생산 큐 목록]\n";
                for (const auto& job : queue)
                {
                    std::wcout << L"  " << job.getJobId()
                               << L" | 주문:" << job.getOrderId()
                               << L" | 시료:" << job.getSampleId()
                               << L" | 실생산량:" << job.getActualQty() << L"ea"
                               << L" | 총생산시간:" << job.getTotalTime() << L"min\n";
                }
            }
        }
        else if (choice == 2)
        {
            auto jobOpt = productionService.getCurrentJob();
            if (!jobOpt.has_value())
                std::wcout << L"현재 진행 중인 생산 작업이 없습니다.\n";
            else
            {
                const auto& job = jobOpt.value();
                std::wcout << L"[현재 작업] " << job.getJobId()
                           << L" | 시작:" << job.getStartTime()
                           << L" | 실생산량:" << job.getActualQty() << L"ea\n";
            }
        }
        else if (choice == 3)
        {
            std::wstring datePart, timePart;
            std::wcout << L"현재 시간 입력 (예: 2026-06-12 11:00): ";
            std::wcin >> datePart >> timePart;
            std::wstring currentTime = datePart + L" " + timePart;

            bool completed = productionService.checkCompletion(currentTime);
            if (completed)
                std::wcout << L"[완료] 생산이 완료되었습니다. 재고 및 주문 상태가 갱신되었습니다.\n";
            else
                std::wcout << L"[진행 중] 아직 생산이 완료되지 않았습니다.\n";
        }
        else if (choice == 0)
        {
            break;
        }
        else
        {
            std::wcout << L"잘못된 선택입니다.\n";
        }
    }
}

// ──────────────────────────────────────────────
//  진입점
// ──────────────────────────────────────────────

int main()
{
    ConsoleUtil::initEncoding();

    // data/ 디렉터리가 없으면 생성
    CreateDirectoryW(L"data", nullptr);

    JsonSampleRepository     sampleRepo(L"data/samples.json");
    JsonOrderRepository      orderRepo(L"data/orders.json");
    JsonProductionRepository productionRepo(L"data/production.json");

    SampleService     sampleService(sampleRepo);
    OrderService      orderService(orderRepo, sampleRepo, productionRepo);
    ProductionService productionService(productionRepo, sampleRepo, orderRepo);

    int roleChoice = 0;
    while (true)
    {
        std::wcout << L"\n=== 시료 생산 주문 관리 시스템 (임시 UI) ===\n";
        std::wcout << L"1. 시료 관리  (생산 담당자)\n";
        std::wcout << L"2. 주문 관리  (주문 담당자)\n";
        std::wcout << L"3. 생산라인 관리 (생산 담당자)\n";
        std::wcout << L"0. 종료\n";
        std::wcout << L"선택 > ";
        std::wcin >> roleChoice;

        if (roleChoice == 1)
        {
            runSampleMenu(sampleService);
        }
        else if (roleChoice == 2)
        {
            runOrderMenu(orderService, sampleService);
        }
        else if (roleChoice == 3)
        {
            runProductionMenu(productionService);
        }
        else if (roleChoice == 0)
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
