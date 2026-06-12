// Test_Phase2.cpp — Phase 2 모델 클래스 단위 테스트 (Red 상태)
// 인코딩: UTF-8 with BOM

#include <gtest/gtest.h>
#include "../src/model/Sample.h"
#include "../src/model/Order.h"
#include "../src/model/ProductionJob.h"
#include "../src/model/UserRole.h"

// ============================================================
// SampleTest
// ============================================================

// TODO: [FR-2-1] Sample 생성자로 전달한 값이 모든 getter에서 올바르게 반환되는지 확인
TEST(SampleTest, ConstructorSetsAllFields)
{
    // Arrange & Act
    Sample sample(L"S-001", L"테스트시료", 30.5, 0.95, 100);

    // Assert
    EXPECT_EQ(sample.getSampleId(), L"S-001");
    EXPECT_EQ(sample.getName(), L"테스트시료");
    EXPECT_DOUBLE_EQ(sample.getAvgProdTime(), 30.5);
    EXPECT_DOUBLE_EQ(sample.getYield(), 0.95);
    EXPECT_EQ(sample.getStock(), 100);
}

// TODO: [FR-2-1] getSampleId() 가 생성자에서 전달한 sampleId를 반환하는지 확인
TEST(SampleTest, GetSampleId)
{
    // Arrange
    Sample sample(L"S-002", L"시료A", 10.0, 0.80, 50);

    // Act & Assert
    EXPECT_EQ(sample.getSampleId(), L"S-002");
}

// TODO: [FR-2-1] getName() 이 생성자에서 전달한 name을 반환하는지 확인
TEST(SampleTest, GetName)
{
    // Arrange
    Sample sample(L"S-003", L"시료B", 20.0, 0.75, 30);

    // Act & Assert
    EXPECT_EQ(sample.getName(), L"시료B");
}

// TODO: [FR-2-1] getAvgProdTime() 이 생성자에서 전달한 avgProdTime을 반환하는지 확인
TEST(SampleTest, GetAvgProdTime)
{
    // Arrange
    Sample sample(L"S-004", L"시료C", 45.0, 0.90, 20);

    // Act & Assert
    EXPECT_DOUBLE_EQ(sample.getAvgProdTime(), 45.0);
}

// TODO: [FR-2-1] getYield() 가 생성자에서 전달한 yield를 반환하는지 확인
TEST(SampleTest, GetYield)
{
    // Arrange
    Sample sample(L"S-005", L"시료D", 15.0, 0.60, 10);

    // Act & Assert
    EXPECT_DOUBLE_EQ(sample.getYield(), 0.60);
}

// TODO: [FR-2-1] getStock() 이 생성자에서 전달한 stock을 반환하는지 확인
TEST(SampleTest, GetStock)
{
    // Arrange
    Sample sample(L"S-006", L"시료E", 25.0, 1.0, 999);

    // Act & Assert
    EXPECT_EQ(sample.getStock(), 999);
}

// TODO: [FR-2-1] toString() 결과 wstring 에 sampleId가 포함되는지 확인
TEST(SampleTest, ToStringContainsSampleId)
{
    // Arrange
    Sample sample(L"S-007", L"시료F", 12.0, 0.85, 40);

    // Act
    std::wstring result = sample.toString();

    // Assert
    EXPECT_TRUE(result.find(L"S-007") != std::wstring::npos);
}

// ============================================================
// OrderTest
// ============================================================

// TODO: [FR-2-2] Order 생성자로 전달한 값이 모든 getter에서 올바르게 반환되는지 확인
TEST(OrderTest, ConstructorSetsAllFields)
{
    // Arrange & Act
    Order order(L"O-001", L"S-001", L"홍길동", 5, L"2026-06-12");

    // Assert
    EXPECT_EQ(order.getOrderId(), L"O-001");
    EXPECT_EQ(order.getSampleId(), L"S-001");
    EXPECT_EQ(order.getCustomerName(), L"홍길동");
    EXPECT_EQ(order.getQuantity(), 5);
    EXPECT_EQ(order.getCreatedAt(), L"2026-06-12");
}

// TODO: [FR-2-2] Order 생성 시 기본 status가 OrderStatus::Reserved 인지 확인
TEST(OrderTest, DefaultStatusIsReserved)
{
    // Arrange & Act
    Order order(L"O-002", L"S-001", L"김철수", 3, L"2026-06-12");

    // Assert
    EXPECT_EQ(order.getStatus(), OrderStatus::Reserved);
}

// TODO: [FR-2-2] setStatus() 로 status를 변경할 수 있는지 확인
TEST(OrderTest, SetStatusChangesStatus)
{
    // Arrange
    Order order(L"O-003", L"S-001", L"이영희", 2, L"2026-06-12");

    // Act
    order.setStatus(OrderStatus::Confirmed);

    // Assert
    EXPECT_EQ(order.getStatus(), OrderStatus::Confirmed);
}

// TODO: [FR-2-2] getOrderId() 가 생성자에서 전달한 orderId를 반환하는지 확인
TEST(OrderTest, GetOrderId)
{
    // Arrange
    Order order(L"O-004", L"S-002", L"박민준", 1, L"2026-06-12");

    // Act & Assert
    EXPECT_EQ(order.getOrderId(), L"O-004");
}

// TODO: [FR-2-2] getSampleId() 가 생성자에서 전달한 sampleId를 반환하는지 확인
TEST(OrderTest, GetSampleId)
{
    // Arrange
    Order order(L"O-005", L"S-003", L"최수진", 7, L"2026-06-12");

    // Act & Assert
    EXPECT_EQ(order.getSampleId(), L"S-003");
}

// TODO: [FR-2-2] getCustomerName() 이 생성자에서 전달한 customerName을 반환하는지 확인
TEST(OrderTest, GetCustomerName)
{
    // Arrange
    Order order(L"O-006", L"S-004", L"정다은", 4, L"2026-06-12");

    // Act & Assert
    EXPECT_EQ(order.getCustomerName(), L"정다은");
}

// TODO: [FR-2-2] getQuantity() 가 생성자에서 전달한 quantity를 반환하는지 확인
TEST(OrderTest, GetQuantity)
{
    // Arrange
    Order order(L"O-007", L"S-005", L"강지훈", 10, L"2026-06-12");

    // Act & Assert
    EXPECT_EQ(order.getQuantity(), 10);
}

// TODO: [FR-2-2] getCreatedAt() 이 생성자에서 전달한 createdAt을 반환하는지 확인
TEST(OrderTest, GetCreatedAt)
{
    // Arrange
    Order order(L"O-008", L"S-006", L"윤서연", 6, L"2026-01-01");

    // Act & Assert
    EXPECT_EQ(order.getCreatedAt(), L"2026-01-01");
}

// TODO: [FR-2-2] toString() 결과 wstring 에 orderId가 포함되는지 확인
TEST(OrderTest, ToStringContainsOrderId)
{
    // Arrange
    Order order(L"O-009", L"S-007", L"임현우", 8, L"2026-06-12");

    // Act
    std::wstring result = order.toString();

    // Assert
    EXPECT_TRUE(result.find(L"O-009") != std::wstring::npos);
}

// ============================================================
// ProductionJobTest
// ============================================================

// TODO: [FR-2-3] ProductionJob 생성자로 전달한 값이 모든 getter에서 올바르게 반환되는지 확인
TEST(ProductionJobTest, ConstructorSetsAllFields)
{
    // Arrange & Act
    ProductionJob job(L"J-001", L"O-001", L"S-001", 5, 10, 120.0, L"2026-06-12 09:00");

    // Assert
    EXPECT_EQ(job.getJobId(), L"J-001");
    EXPECT_EQ(job.getOrderId(), L"O-001");
    EXPECT_EQ(job.getSampleId(), L"S-001");
    EXPECT_EQ(job.getShortage(), 5);
    EXPECT_EQ(job.getActualQty(), 10);
    EXPECT_DOUBLE_EQ(job.getTotalTime(), 120.0);
    EXPECT_EQ(job.getStartTime(), L"2026-06-12 09:00");
}

// TODO: [FR-2-3] getJobId() 가 생성자에서 전달한 jobId를 반환하는지 확인
TEST(ProductionJobTest, GetJobId)
{
    // Arrange
    ProductionJob job(L"J-002", L"O-002", L"S-002", 3, 5, 60.0, L"2026-06-12 10:00");

    // Act & Assert
    EXPECT_EQ(job.getJobId(), L"J-002");
}

// TODO: [FR-2-3] getOrderId() 가 생성자에서 전달한 orderId를 반환하는지 확인
TEST(ProductionJobTest, GetOrderId)
{
    // Arrange
    ProductionJob job(L"J-003", L"O-003", L"S-003", 0, 8, 90.0, L"2026-06-12 11:00");

    // Act & Assert
    EXPECT_EQ(job.getOrderId(), L"O-003");
}

// TODO: [FR-2-3] getSampleId() 가 생성자에서 전달한 sampleId를 반환하는지 확인
TEST(ProductionJobTest, GetSampleId)
{
    // Arrange
    ProductionJob job(L"J-004", L"O-004", L"S-004", 2, 4, 45.0, L"2026-06-12 12:00");

    // Act & Assert
    EXPECT_EQ(job.getSampleId(), L"S-004");
}

// TODO: [FR-2-3] getShortage() 가 생성자에서 전달한 shortage를 반환하는지 확인
TEST(ProductionJobTest, GetShortage)
{
    // Arrange
    ProductionJob job(L"J-005", L"O-005", L"S-005", 7, 12, 200.0, L"2026-06-12 13:00");

    // Act & Assert
    EXPECT_EQ(job.getShortage(), 7);
}

// TODO: [FR-2-3] getActualQty() 가 생성자에서 전달한 actualQty를 반환하는지 확인
TEST(ProductionJobTest, GetActualQty)
{
    // Arrange
    ProductionJob job(L"J-006", L"O-006", L"S-006", 1, 15, 180.0, L"2026-06-12 14:00");

    // Act & Assert
    EXPECT_EQ(job.getActualQty(), 15);
}

// TODO: [FR-2-3] getTotalTime() 이 생성자에서 전달한 totalTime을 반환하는지 확인
TEST(ProductionJobTest, GetTotalTime)
{
    // Arrange
    ProductionJob job(L"J-007", L"O-007", L"S-007", 4, 6, 300.5, L"2026-06-12 15:00");

    // Act & Assert
    EXPECT_DOUBLE_EQ(job.getTotalTime(), 300.5);
}

// TODO: [FR-2-3] getStartTime() 이 생성자에서 전달한 startTime을 반환하는지 확인
TEST(ProductionJobTest, GetStartTime)
{
    // Arrange
    ProductionJob job(L"J-008", L"O-008", L"S-008", 0, 20, 150.0, L"2026-06-12 16:00");

    // Act & Assert
    EXPECT_EQ(job.getStartTime(), L"2026-06-12 16:00");
}

// TODO: [FR-2-3] toString() 결과 wstring 에 jobId가 포함되는지 확인
TEST(ProductionJobTest, ToStringContainsJobId)
{
    // Arrange
    ProductionJob job(L"J-009", L"O-009", L"S-009", 2, 5, 75.0, L"2026-06-12 17:00");

    // Act
    std::wstring result = job.toString();

    // Assert
    EXPECT_TRUE(result.find(L"J-009") != std::wstring::npos);
}

// ============================================================
// UserRoleTest
// ============================================================

// TODO: [FR-2-4] UserRole::OrderManager 가 UserRole::ProductionManager 와 다른지 확인
TEST(UserRoleTest, OrderManagerValue)
{
    // Arrange & Act
    UserRole orderManager = UserRole::OrderManager;
    UserRole productionManager = UserRole::ProductionManager;

    // Assert
    EXPECT_NE(orderManager, productionManager);
}

// TODO: [FR-2-4] UserRole::ProductionManager 가 UserRole::OrderManager 와 다른지 확인
TEST(UserRoleTest, ProductionManagerValue)
{
    // Arrange & Act
    UserRole productionManager = UserRole::ProductionManager;
    UserRole orderManager = UserRole::OrderManager;

    // Assert
    EXPECT_NE(productionManager, orderManager);
}
