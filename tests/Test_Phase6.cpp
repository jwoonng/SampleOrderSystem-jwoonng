// Test_Phase6.cpp — Phase 6 ProductionService 단위 테스트 (Red 상태)
// 인코딩: UTF-8 with BOM

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <optional>
#include <vector>
#include <cmath>

#include "../src/model/Sample.h"
#include "../src/model/Order.h"
#include "../src/model/ProductionJob.h"
#include "repository/IProductionRepository.h"
#include "repository/ISampleRepository.h"
#include "repository/IOrderRepository.h"
#include "service/ProductionService.h"   // 아직 없으므로 컴파일 실패 (정상)

using ::testing::Return;
using ::testing::_;
using ::testing::SaveArg;

// ============================================================
// Mock 정의
// ============================================================

class MockProductionRepository : public IProductionRepository
{
public:
    MOCK_METHOD(void, enqueue, (const ProductionJob&), (override));
    MOCK_METHOD(std::optional<ProductionJob>, front, (), (const, override));
    MOCK_METHOD(void, dequeue, (), (override));
    MOCK_METHOD(std::vector<ProductionJob>, findAll, (), (const, override));
    MOCK_METHOD(bool, isEmpty, (), (const, override));
};

class MockSampleRepository : public ISampleRepository
{
public:
    MOCK_METHOD(void, add, (const Sample&), (override));
    MOCK_METHOD(std::optional<Sample>, findById, (const std::wstring&), (const, override));
    MOCK_METHOD(std::vector<Sample>, findAll, (), (const, override));
    MOCK_METHOD(void, update, (const Sample&), (override));
    MOCK_METHOD(void, remove, (const std::wstring&), (override));
};

class MockOrderRepository : public IOrderRepository
{
public:
    MOCK_METHOD(void, add, (const Order&), (override));
    MOCK_METHOD(std::optional<Order>, findById, (const std::wstring&), (const, override));
    MOCK_METHOD(std::vector<Order>, findAll, (), (const, override));
    MOCK_METHOD(std::vector<Order>, findByStatus, (OrderStatus), (const, override));
    MOCK_METHOD(void, update, (const Order&), (override));
    MOCK_METHOD(void, remove, (const std::wstring&), (override));
};

// ============================================================
// ProductionServiceTest Fixture
// ============================================================

class ProductionServiceTest : public ::testing::Test
{
protected:
    MockProductionRepository mockProductionRepo;
    MockSampleRepository     mockSampleRepo;
    MockOrderRepository      mockOrderRepo;
};

// ============================================================
// 큐 관리 테스트 (4건)
// ============================================================

// TODO: [FR-6-1] enqueue 호출 시 productionRepo.enqueue 1회 호출
TEST_F(ProductionServiceTest, Enqueue_AddsJobToRepo)
{
    // Arrange
    ProductionJob job(L"JOB-001", L"ORD-001", L"S-001", 7, 9, 270.0, L"2026-06-12 09:00");

    EXPECT_CALL(mockProductionRepo, enqueue(_)).Times(1);

    ProductionService service(mockProductionRepo, mockSampleRepo, mockOrderRepo);

    // Act
    service.enqueue(job);
}

// TODO: [FR-6-1] getQueue()가 productionRepo.findAll() 결과를 반환
TEST_F(ProductionServiceTest, GetQueue_ReturnsAllJobs)
{
    // Arrange
    std::vector<ProductionJob> jobs = {
        ProductionJob(L"JOB-001", L"ORD-001", L"S-001", 7, 9, 270.0, L"2026-06-12 09:00"),
        ProductionJob(L"JOB-002", L"ORD-002", L"S-002", 3, 4, 120.0, L"2026-06-12 10:00")
    };

    EXPECT_CALL(mockProductionRepo, findAll()).WillOnce(Return(jobs));

    ProductionService service(mockProductionRepo, mockSampleRepo, mockOrderRepo);

    // Act
    std::vector<ProductionJob> result = service.getQueue();

    // Assert
    EXPECT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0].getJobId(), L"JOB-001");
    EXPECT_EQ(result[1].getJobId(), L"JOB-002");
}

// TODO: [FR-6-1] getCurrentJob()이 productionRepo.front() 결과를 반환
TEST_F(ProductionServiceTest, GetCurrentJob_ReturnsFirstJob)
{
    // Arrange
    ProductionJob job(L"JOB-001", L"ORD-001", L"S-001", 7, 9, 270.0, L"2026-06-12 09:00");

    EXPECT_CALL(mockProductionRepo, front())
        .WillOnce(Return(std::optional<ProductionJob>(job)));

    ProductionService service(mockProductionRepo, mockSampleRepo, mockOrderRepo);

    // Act
    std::optional<ProductionJob> result = service.getCurrentJob();

    // Assert
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->getJobId(), L"JOB-001");
}

// TODO: [FR-6-1] front()가 nullopt 반환 시 getCurrentJob()도 nullopt 반환
TEST_F(ProductionServiceTest, GetCurrentJob_EmptyQueue_ReturnsNullopt)
{
    // Arrange
    EXPECT_CALL(mockProductionRepo, front())
        .WillOnce(Return(std::nullopt));

    ProductionService service(mockProductionRepo, mockSampleRepo, mockOrderRepo);

    // Act
    std::optional<ProductionJob> result = service.getCurrentJob();

    // Assert
    EXPECT_FALSE(result.has_value());
}

// ============================================================
// 경과 생산량 테스트 (3건)
// ============================================================

// TODO: [FR-6-2] startTime과 currentTime이 같으면 경과 생산량 0 반환
TEST_F(ProductionServiceTest, GetElapsedQty_ZeroElapsed)
{
    // Arrange
    // avgProdTime=30, startTime="2026-06-12 09:00", actualQty=4
    // currentTime="2026-06-12 09:00" → elapsed=0min → floor(0/30)=0
    ProductionJob job(L"JOB-001", L"ORD-001", L"S-001", 2, 4, 120.0, L"2026-06-12 09:00");
    Sample sample(L"S-001", L"테스트시료", 30.0, 0.95, 5);

    EXPECT_CALL(mockSampleRepo, findById(L"S-001"))
        .WillOnce(Return(std::optional<Sample>(sample)));

    ProductionService service(mockProductionRepo, mockSampleRepo, mockOrderRepo);

    // Act
    int result = service.getElapsedQty(job, L"2026-06-12 09:00");

    // Assert
    EXPECT_EQ(result, 0);
}

// TODO: [FR-6-2] 부분 생산 시 floor 값 반환
TEST_F(ProductionServiceTest, GetElapsedQty_PartialProduction)
{
    // Arrange
    // avgProdTime=30, startTime="2026-06-12 09:00", actualQty=5
    // currentTime="2026-06-12 11:00" → elapsed=120min → floor(120/30)=4
    ProductionJob job(L"JOB-001", L"ORD-001", L"S-001", 3, 5, 150.0, L"2026-06-12 09:00");
    Sample sample(L"S-001", L"테스트시료", 30.0, 0.95, 5);

    EXPECT_CALL(mockSampleRepo, findById(L"S-001"))
        .WillOnce(Return(std::optional<Sample>(sample)));

    ProductionService service(mockProductionRepo, mockSampleRepo, mockOrderRepo);

    // Act
    int result = service.getElapsedQty(job, L"2026-06-12 11:00");

    // Assert
    EXPECT_EQ(result, 4);
}

// TODO: [FR-6-2] 경과 생산량이 actualQty 초과해도 actualQty로 cap
TEST_F(ProductionServiceTest, GetElapsedQty_CappedAtActualQty)
{
    // Arrange
    // avgProdTime=30, actualQty=3, startTime="2026-06-12 09:00"
    // currentTime="2026-06-12 13:00" → elapsed=240min → floor(240/30)=8 → cap → 3
    ProductionJob job(L"JOB-001", L"ORD-001", L"S-001", 2, 3, 90.0, L"2026-06-12 09:00");
    Sample sample(L"S-001", L"테스트시료", 30.0, 0.95, 5);

    EXPECT_CALL(mockSampleRepo, findById(L"S-001"))
        .WillOnce(Return(std::optional<Sample>(sample)));

    ProductionService service(mockProductionRepo, mockSampleRepo, mockOrderRepo);

    // Act
    int result = service.getElapsedQty(job, L"2026-06-12 13:00");

    // Assert
    EXPECT_EQ(result, 3);
}

// ============================================================
// 완료 처리 테스트 (6건)
// ============================================================

// TODO: [FR-6-2] 빈 큐일 때 checkCompletion은 false 반환
TEST_F(ProductionServiceTest, CheckCompletion_EmptyQueue_ReturnsFalse)
{
    // Arrange
    EXPECT_CALL(mockProductionRepo, front())
        .WillOnce(Return(std::nullopt));

    ProductionService service(mockProductionRepo, mockSampleRepo, mockOrderRepo);

    // Act
    bool result = service.checkCompletion(L"2026-06-12 12:00");

    // Assert
    EXPECT_FALSE(result);
}

// TODO: [FR-6-2] 경과 생산량 < actualQty 시 false 반환, update/dequeue 미호출
TEST_F(ProductionServiceTest, CheckCompletion_NotComplete_ReturnsFalse)
{
    // Arrange
    // avgProdTime=30, actualQty=5, startTime="2026-06-12 09:00"
    // currentTime="2026-06-12 11:00" → elapsed=120min → floor(120/30)=4 < 5 → 미완료
    ProductionJob job(L"JOB-001", L"ORD-001", L"S-001", 3, 5, 150.0, L"2026-06-12 09:00");
    Sample sample(L"S-001", L"테스트시료", 30.0, 0.95, 5);

    EXPECT_CALL(mockProductionRepo, front())
        .WillOnce(Return(std::optional<ProductionJob>(job)));
    EXPECT_CALL(mockSampleRepo, findById(L"S-001"))
        .WillOnce(Return(std::optional<Sample>(sample)));

    // update와 dequeue는 호출되지 않아야 함
    EXPECT_CALL(mockSampleRepo, update(_)).Times(0);
    EXPECT_CALL(mockOrderRepo, update(_)).Times(0);
    EXPECT_CALL(mockProductionRepo, dequeue()).Times(0);

    ProductionService service(mockProductionRepo, mockSampleRepo, mockOrderRepo);

    // Act
    bool result = service.checkCompletion(L"2026-06-12 11:00");

    // Assert
    EXPECT_FALSE(result);
}

// TODO: [FR-6-2] 경과 생산량 >= actualQty 시 true 반환
TEST_F(ProductionServiceTest, CheckCompletion_Complete_ReturnsTrue)
{
    // Arrange
    // avgProdTime=30, actualQty=4, startTime="2026-06-12 09:00"
    // currentTime="2026-06-12 11:00" → elapsed=120min → floor(120/30)=4 >= 4 → 완료
    ProductionJob job(L"JOB-001", L"ORD-001", L"S-001", 2, 4, 120.0, L"2026-06-12 09:00");
    Sample sample(L"S-001", L"테스트시료", 30.0, 0.95, 5);
    Order order(L"ORD-001", L"S-001", L"홍길동", 8, L"2026-06-12");
    order.setStatus(OrderStatus::Producing);

    EXPECT_CALL(mockProductionRepo, front())
        .WillOnce(Return(std::optional<ProductionJob>(job)));
    EXPECT_CALL(mockSampleRepo, findById(L"S-001"))
        .WillRepeatedly(Return(std::optional<Sample>(sample)));
    EXPECT_CALL(mockOrderRepo, findById(L"ORD-001"))
        .WillOnce(Return(std::optional<Order>(order)));
    EXPECT_CALL(mockSampleRepo, update(_)).Times(1);
    EXPECT_CALL(mockOrderRepo, update(_)).Times(1);
    EXPECT_CALL(mockProductionRepo, dequeue()).Times(1);

    ProductionService service(mockProductionRepo, mockSampleRepo, mockOrderRepo);

    // Act
    bool result = service.checkCompletion(L"2026-06-12 11:00");

    // Assert
    EXPECT_TRUE(result);
}

// TODO: [FR-6-2] 완료 시 재고가 올바르게 갱신됨
// currentStock=5, actualQty=10, order.quantity=8
// newStock = 5 + 10 - 8 = 7
TEST_F(ProductionServiceTest, CheckCompletion_Complete_UpdatesStockCorrectly)
{
    // Arrange
    // avgProdTime=30, actualQty=10, startTime="2026-06-12 09:00"
    // currentTime="2026-06-12 14:00" → elapsed=300min → floor(300/30)=10 >= 10 → 완료
    ProductionJob job(L"JOB-001", L"ORD-001", L"S-001", 5, 10, 300.0, L"2026-06-12 09:00");
    Sample sample(L"S-001", L"테스트시료", 30.0, 0.95, 5);   // currentStock=5
    Order order(L"ORD-001", L"S-001", L"홍길동", 8, L"2026-06-12");   // quantity=8
    order.setStatus(OrderStatus::Producing);

    EXPECT_CALL(mockProductionRepo, front())
        .WillOnce(Return(std::optional<ProductionJob>(job)));
    EXPECT_CALL(mockSampleRepo, findById(L"S-001"))
        .WillRepeatedly(Return(std::optional<Sample>(sample)));
    EXPECT_CALL(mockOrderRepo, findById(L"ORD-001"))
        .WillOnce(Return(std::optional<Order>(order)));

    Sample updatedSample(L"", L"", 0.0, 0.0, 0);
    EXPECT_CALL(mockSampleRepo, update(_))
        .WillOnce(SaveArg<0>(&updatedSample));
    EXPECT_CALL(mockOrderRepo, update(_)).Times(1);
    EXPECT_CALL(mockProductionRepo, dequeue()).Times(1);

    ProductionService service(mockProductionRepo, mockSampleRepo, mockOrderRepo);

    // Act
    service.checkCompletion(L"2026-06-12 14:00");

    // Assert
    // newStock = 5 + 10 - 8 = 7
    EXPECT_EQ(updatedSample.getStock(), 7);
}

// TODO: [FR-6-2] 완료 시 Order 상태가 Confirmed로 갱신됨
TEST_F(ProductionServiceTest, CheckCompletion_Complete_OrderStatusBecomesConfirmed)
{
    // Arrange
    // avgProdTime=30, actualQty=4, startTime="2026-06-12 09:00"
    // currentTime="2026-06-12 11:00" → elapsed=120min → floor(120/30)=4 >= 4 → 완료
    ProductionJob job(L"JOB-001", L"ORD-001", L"S-001", 2, 4, 120.0, L"2026-06-12 09:00");
    Sample sample(L"S-001", L"테스트시료", 30.0, 0.95, 3);
    Order order(L"ORD-001", L"S-001", L"홍길동", 5, L"2026-06-12");
    order.setStatus(OrderStatus::Producing);

    EXPECT_CALL(mockProductionRepo, front())
        .WillOnce(Return(std::optional<ProductionJob>(job)));
    EXPECT_CALL(mockSampleRepo, findById(L"S-001"))
        .WillRepeatedly(Return(std::optional<Sample>(sample)));
    EXPECT_CALL(mockOrderRepo, findById(L"ORD-001"))
        .WillOnce(Return(std::optional<Order>(order)));
    EXPECT_CALL(mockSampleRepo, update(_)).Times(1);

    Order updatedOrder(L"", L"", L"", 0, L"");
    EXPECT_CALL(mockOrderRepo, update(_))
        .WillOnce(SaveArg<0>(&updatedOrder));
    EXPECT_CALL(mockProductionRepo, dequeue()).Times(1);

    ProductionService service(mockProductionRepo, mockSampleRepo, mockOrderRepo);

    // Act
    service.checkCompletion(L"2026-06-12 11:00");

    // Assert
    EXPECT_EQ(updatedOrder.getStatus(), OrderStatus::Confirmed);
}

// TODO: [FR-6-2] 완료 시 productionRepo.dequeue() 1회 호출
TEST_F(ProductionServiceTest, CheckCompletion_Complete_DequeuesJob)
{
    // Arrange
    // avgProdTime=30, actualQty=4, startTime="2026-06-12 09:00"
    // currentTime="2026-06-12 11:00" → elapsed=120min → floor(120/30)=4 >= 4 → 완료
    ProductionJob job(L"JOB-001", L"ORD-001", L"S-001", 2, 4, 120.0, L"2026-06-12 09:00");
    Sample sample(L"S-001", L"테스트시료", 30.0, 0.95, 3);
    Order order(L"ORD-001", L"S-001", L"홍길동", 5, L"2026-06-12");
    order.setStatus(OrderStatus::Producing);

    EXPECT_CALL(mockProductionRepo, front())
        .WillOnce(Return(std::optional<ProductionJob>(job)));
    EXPECT_CALL(mockSampleRepo, findById(L"S-001"))
        .WillRepeatedly(Return(std::optional<Sample>(sample)));
    EXPECT_CALL(mockOrderRepo, findById(L"ORD-001"))
        .WillOnce(Return(std::optional<Order>(order)));
    EXPECT_CALL(mockSampleRepo, update(_)).Times(1);
    EXPECT_CALL(mockOrderRepo, update(_)).Times(1);

    // 핵심 검증: dequeue 정확히 1회 호출
    EXPECT_CALL(mockProductionRepo, dequeue()).Times(1);

    ProductionService service(mockProductionRepo, mockSampleRepo, mockOrderRepo);

    // Act
    service.checkCompletion(L"2026-06-12 11:00");
}
