// Test_Phase5.cpp — Phase 5 OrderService 단위 테스트 (Red 상태)
// 인코딩: UTF-8 with BOM

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <optional>
#include <vector>
#include <stdexcept>
#include <cmath>

#include "../src/model/Sample.h"
#include "../src/model/Order.h"
#include "../src/model/ProductionJob.h"
#include "repository/IOrderRepository.h"
#include "repository/ISampleRepository.h"
#include "repository/IProductionRepository.h"
#include "service/OrderService.h"   // 아직 없으므로 컴파일 실패 (정상)

using ::testing::Return;
using ::testing::_;
using ::testing::SaveArg;

// ============================================================
// Mock 정의
// ============================================================

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

class MockSampleRepository : public ISampleRepository
{
public:
    MOCK_METHOD(void, add, (const Sample&), (override));
    MOCK_METHOD(std::optional<Sample>, findById, (const std::wstring&), (const, override));
    MOCK_METHOD(std::vector<Sample>, findAll, (), (const, override));
    MOCK_METHOD(void, update, (const Sample&), (override));
    MOCK_METHOD(void, remove, (const std::wstring&), (override));
};

class MockProductionRepository : public IProductionRepository
{
public:
    MOCK_METHOD(void, enqueue, (const ProductionJob&), (override));
    MOCK_METHOD(std::optional<ProductionJob>, front, (), (const, override));
    MOCK_METHOD(void, dequeue, (), (override));
    MOCK_METHOD(std::vector<ProductionJob>, findAll, (), (const, override));
    MOCK_METHOD(bool, isEmpty, (), (const, override));
};

// ============================================================
// OrderServiceTest Fixture
// ============================================================

class OrderServiceTest : public ::testing::Test
{
protected:
    MockOrderRepository    mockOrderRepo;
    MockSampleRepository   mockSampleRepo;
    MockProductionRepository mockProductionRepo;
    // OrderService는 아직 없으므로 포인터 선언만
    // (컴파일 실패가 Red 단계의 목표)
};

// ============================================================
// placeOrder 테스트 (5건)
// ============================================================

// TODO: [FR-5-1] 정상 주문 시 Reserved 상태 Order 반환, orderRepo.add 1회 호출
TEST_F(OrderServiceTest, PlaceOrder_ValidInput_ReturnsReservedOrder)
{
    // Arrange
    Sample sample(L"S-001", L"테스트시료", 30.0, 0.95, 100);

    EXPECT_CALL(mockSampleRepo, findById(L"S-001"))
        .WillOnce(Return(std::optional<Sample>(sample)));
    EXPECT_CALL(mockOrderRepo, add(_)).Times(1);

    OrderService service(mockOrderRepo, mockSampleRepo, mockProductionRepo);

    // Act
    Order result = service.placeOrder(L"S-001", L"홍길동", 5, L"ORD-001", L"2026-06-12");

    // Assert
    EXPECT_EQ(result.getStatus(), OrderStatus::Reserved);
    EXPECT_EQ(result.getSampleId(), L"S-001");
    EXPECT_EQ(result.getCustomerName(), L"홍길동");
    EXPECT_EQ(result.getQuantity(), 5);
}

// TODO: [FR-5-1] sampleRepo.findById가 nullopt 반환 시 예외
TEST_F(OrderServiceTest, PlaceOrder_InvalidSampleId_Throws)
{
    // Arrange
    EXPECT_CALL(mockSampleRepo, findById(L"S-999"))
        .WillOnce(Return(std::nullopt));

    OrderService service(mockOrderRepo, mockSampleRepo, mockProductionRepo);

    // Act & Assert
    EXPECT_THROW(
        service.placeOrder(L"S-999", L"홍길동", 5, L"ORD-001", L"2026-06-12"),
        std::invalid_argument
    );
}

// TODO: [FR-5-1] quantity=0 시 예외
TEST_F(OrderServiceTest, PlaceOrder_ZeroQuantity_Throws)
{
    // Arrange
    OrderService service(mockOrderRepo, mockSampleRepo, mockProductionRepo);

    // Act & Assert
    EXPECT_THROW(
        service.placeOrder(L"S-001", L"홍길동", 0, L"ORD-001", L"2026-06-12"),
        std::invalid_argument
    );
}

// TODO: [FR-5-1] quantity=-1 시 예외
TEST_F(OrderServiceTest, PlaceOrder_NegativeQuantity_Throws)
{
    // Arrange
    OrderService service(mockOrderRepo, mockSampleRepo, mockProductionRepo);

    // Act & Assert
    EXPECT_THROW(
        service.placeOrder(L"S-001", L"홍길동", -1, L"ORD-001", L"2026-06-12"),
        std::invalid_argument
    );
}

// TODO: [FR-5-1] orderRepo.add에 Reserved 상태 Order가 전달됨
TEST_F(OrderServiceTest, PlaceOrder_ValidInput_OrderAddedToRepo)
{
    // Arrange
    Sample sample(L"S-001", L"테스트시료", 30.0, 0.95, 100);

    EXPECT_CALL(mockSampleRepo, findById(L"S-001"))
        .WillOnce(Return(std::optional<Sample>(sample)));

    Order capturedOrder(L"", L"", L"", 0, L"");
    EXPECT_CALL(mockOrderRepo, add(_))
        .WillOnce(SaveArg<0>(&capturedOrder));

    OrderService service(mockOrderRepo, mockSampleRepo, mockProductionRepo);

    // Act
    service.placeOrder(L"S-001", L"홍길동", 5, L"ORD-001", L"2026-06-12");

    // Assert
    EXPECT_EQ(capturedOrder.getStatus(), OrderStatus::Reserved);
    EXPECT_EQ(capturedOrder.getOrderId(), L"ORD-001");
}

// ============================================================
// approveOrder 테스트 (7건)
// ============================================================

// TODO: [FR-5-2] orderRepo.findById nullopt 시 예외
TEST_F(OrderServiceTest, ApproveOrder_NotFound_Throws)
{
    // Arrange
    EXPECT_CALL(mockOrderRepo, findById(L"ORD-999"))
        .WillOnce(Return(std::nullopt));

    OrderService service(mockOrderRepo, mockSampleRepo, mockProductionRepo);

    // Act & Assert
    EXPECT_THROW(
        service.approveOrder(L"ORD-999", L"JOB-001", L"2026-06-12 09:00"),
        std::invalid_argument
    );
}

// TODO: [FR-5-2] 상태가 Confirmed인 주문 승인 시 예외
TEST_F(OrderServiceTest, ApproveOrder_NotReserved_Throws)
{
    // Arrange
    Order confirmedOrder(L"ORD-001", L"S-001", L"홍길동", 5, L"2026-06-12");
    confirmedOrder.setStatus(OrderStatus::Confirmed);

    EXPECT_CALL(mockOrderRepo, findById(L"ORD-001"))
        .WillOnce(Return(std::optional<Order>(confirmedOrder)));

    OrderService service(mockOrderRepo, mockSampleRepo, mockProductionRepo);

    // Act & Assert
    EXPECT_THROW(
        service.approveOrder(L"ORD-001", L"JOB-001", L"2026-06-12 09:00"),
        std::invalid_argument
    );
}

// TODO: [FR-5-2] 재고 충분 시 상태 Confirmed, sampleRepo.update + orderRepo.update 호출
TEST_F(OrderServiceTest, ApproveOrder_SufficientStock_UpdatesToConfirmed)
{
    // Arrange
    Order reservedOrder(L"ORD-001", L"S-001", L"홍길동", 5, L"2026-06-12");
    // 기본 생성 상태가 Reserved이어야 하므로 명시적 설정
    reservedOrder.setStatus(OrderStatus::Reserved);
    Sample sample(L"S-001", L"테스트시료", 30.0, 0.95, 100);   // stock=100 >= quantity=5

    EXPECT_CALL(mockOrderRepo, findById(L"ORD-001"))
        .WillOnce(Return(std::optional<Order>(reservedOrder)));
    EXPECT_CALL(mockSampleRepo, findById(L"S-001"))
        .WillOnce(Return(std::optional<Sample>(sample)));
    EXPECT_CALL(mockSampleRepo, update(_)).Times(1);

    Order updatedOrder(L"", L"", L"", 0, L"");
    EXPECT_CALL(mockOrderRepo, update(_))
        .WillOnce(SaveArg<0>(&updatedOrder));

    OrderService service(mockOrderRepo, mockSampleRepo, mockProductionRepo);

    // Act
    service.approveOrder(L"ORD-001", L"JOB-001", L"2026-06-12 09:00");

    // Assert
    EXPECT_EQ(updatedOrder.getStatus(), OrderStatus::Confirmed);
}

// TODO: [FR-5-2] 재고 차감 후 update 호출되는 Sample의 stock이 (기존 재고 - 주문 수량)
TEST_F(OrderServiceTest, ApproveOrder_SufficientStock_DeductsStock)
{
    // Arrange
    Order reservedOrder(L"ORD-001", L"S-001", L"홍길동", 5, L"2026-06-12");
    reservedOrder.setStatus(OrderStatus::Reserved);
    Sample sample(L"S-001", L"테스트시료", 30.0, 0.95, 100);   // stock=100, quantity=5 → 95

    EXPECT_CALL(mockOrderRepo, findById(L"ORD-001"))
        .WillOnce(Return(std::optional<Order>(reservedOrder)));
    EXPECT_CALL(mockSampleRepo, findById(L"S-001"))
        .WillOnce(Return(std::optional<Sample>(sample)));

    Sample updatedSample(L"", L"", 0.0, 0.0, 0);
    EXPECT_CALL(mockSampleRepo, update(_))
        .WillOnce(SaveArg<0>(&updatedSample));
    EXPECT_CALL(mockOrderRepo, update(_)).Times(1);

    OrderService service(mockOrderRepo, mockSampleRepo, mockProductionRepo);

    // Act
    service.approveOrder(L"ORD-001", L"JOB-001", L"2026-06-12 09:00");

    // Assert
    EXPECT_EQ(updatedSample.getStock(), 95);   // 100 - 5
}

// TODO: [FR-5-2] 재고 부족 시 상태 Producing, productionRepo.enqueue + orderRepo.update 호출
TEST_F(OrderServiceTest, ApproveOrder_InsufficientStock_UpdatesToProducing)
{
    // Arrange
    Order reservedOrder(L"ORD-001", L"S-001", L"홍길동", 10, L"2026-06-12");
    reservedOrder.setStatus(OrderStatus::Reserved);
    Sample sample(L"S-001", L"테스트시료", 30.0, 0.9, 3);   // stock=3 < quantity=10

    EXPECT_CALL(mockOrderRepo, findById(L"ORD-001"))
        .WillOnce(Return(std::optional<Order>(reservedOrder)));
    EXPECT_CALL(mockSampleRepo, findById(L"S-001"))
        .WillOnce(Return(std::optional<Sample>(sample)));
    EXPECT_CALL(mockProductionRepo, enqueue(_)).Times(1);

    Order updatedOrder(L"", L"", L"", 0, L"");
    EXPECT_CALL(mockOrderRepo, update(_))
        .WillOnce(SaveArg<0>(&updatedOrder));

    OrderService service(mockOrderRepo, mockSampleRepo, mockProductionRepo);

    // Act
    service.approveOrder(L"ORD-001", L"JOB-001", L"2026-06-12 09:00");

    // Assert
    EXPECT_EQ(updatedOrder.getStatus(), OrderStatus::Producing);
}

// TODO: [FR-5-2] 부족분/actualQty/totalTime 계산 검증
// shortage = 10 - 3 = 7
// actualQty = ceil(7 / (0.9 * 0.9)) = ceil(7 / 0.81) = ceil(8.641...) = 9
// totalTime = 30.0 * 9 = 270.0
TEST_F(OrderServiceTest, ApproveOrder_InsufficientStock_EnqueuesCorrectJob)
{
    // Arrange
    Order reservedOrder(L"ORD-001", L"S-001", L"홍길동", 10, L"2026-06-12");
    reservedOrder.setStatus(OrderStatus::Reserved);
    Sample sample(L"S-001", L"테스트시료", 30.0, 0.9, 3);   // stock=3, yield=0.9, avgProdTime=30.0

    EXPECT_CALL(mockOrderRepo, findById(L"ORD-001"))
        .WillOnce(Return(std::optional<Order>(reservedOrder)));
    EXPECT_CALL(mockSampleRepo, findById(L"S-001"))
        .WillOnce(Return(std::optional<Sample>(sample)));

    ProductionJob capturedJob(L"", L"", L"", 0, 0, 0.0, L"");
    EXPECT_CALL(mockProductionRepo, enqueue(_))
        .WillOnce(SaveArg<0>(&capturedJob));
    EXPECT_CALL(mockOrderRepo, update(_)).Times(1);

    OrderService service(mockOrderRepo, mockSampleRepo, mockProductionRepo);

    // Act
    service.approveOrder(L"ORD-001", L"JOB-001", L"2026-06-12 09:00");

    // Assert
    const int expectedShortage  = 7;                                              // 10 - 3
    const int expectedActualQty = static_cast<int>(std::ceil(7.0 / (0.9 * 0.9))); // ceil(8.641) = 9
    const double expectedTotalTime = 30.0 * expectedActualQty;                    // 270.0

    EXPECT_EQ(capturedJob.getJobId(),    L"JOB-001");
    EXPECT_EQ(capturedJob.getOrderId(),  L"ORD-001");
    EXPECT_EQ(capturedJob.getSampleId(), L"S-001");
    EXPECT_EQ(capturedJob.getShortage(),  expectedShortage);
    EXPECT_EQ(capturedJob.getActualQty(), expectedActualQty);
    EXPECT_DOUBLE_EQ(capturedJob.getTotalTime(), expectedTotalTime);
}

// TODO: [FR-5-2] stock == quantity 정확히 같을 때 Confirmed (충분으로 처리)
TEST_F(OrderServiceTest, ApproveOrder_ExactStock_UpdatesToConfirmed)
{
    // Arrange
    Order reservedOrder(L"ORD-001", L"S-001", L"홍길동", 10, L"2026-06-12");
    reservedOrder.setStatus(OrderStatus::Reserved);
    Sample sample(L"S-001", L"테스트시료", 30.0, 0.95, 10);   // stock=10 == quantity=10

    EXPECT_CALL(mockOrderRepo, findById(L"ORD-001"))
        .WillOnce(Return(std::optional<Order>(reservedOrder)));
    EXPECT_CALL(mockSampleRepo, findById(L"S-001"))
        .WillOnce(Return(std::optional<Sample>(sample)));
    EXPECT_CALL(mockSampleRepo, update(_)).Times(1);

    Order updatedOrder(L"", L"", L"", 0, L"");
    EXPECT_CALL(mockOrderRepo, update(_))
        .WillOnce(SaveArg<0>(&updatedOrder));

    OrderService service(mockOrderRepo, mockSampleRepo, mockProductionRepo);

    // Act
    service.approveOrder(L"ORD-001", L"JOB-001", L"2026-06-12 09:00");

    // Assert
    EXPECT_EQ(updatedOrder.getStatus(), OrderStatus::Confirmed);
}

// ============================================================
// rejectOrder 테스트 (3건)
// ============================================================

// TODO: [FR-5-3] orderRepo.findById nullopt 시 예외
TEST_F(OrderServiceTest, RejectOrder_NotFound_Throws)
{
    // Arrange
    EXPECT_CALL(mockOrderRepo, findById(L"ORD-999"))
        .WillOnce(Return(std::nullopt));

    OrderService service(mockOrderRepo, mockSampleRepo, mockProductionRepo);

    // Act & Assert
    EXPECT_THROW(
        service.rejectOrder(L"ORD-999"),
        std::invalid_argument
    );
}

// TODO: [FR-5-3] 상태가 Confirmed인 주문 거절 시 예외
TEST_F(OrderServiceTest, RejectOrder_NotReserved_Throws)
{
    // Arrange
    Order confirmedOrder(L"ORD-001", L"S-001", L"홍길동", 5, L"2026-06-12");
    confirmedOrder.setStatus(OrderStatus::Confirmed);

    EXPECT_CALL(mockOrderRepo, findById(L"ORD-001"))
        .WillOnce(Return(std::optional<Order>(confirmedOrder)));

    OrderService service(mockOrderRepo, mockSampleRepo, mockProductionRepo);

    // Act & Assert
    EXPECT_THROW(
        service.rejectOrder(L"ORD-001"),
        std::invalid_argument
    );
}

// TODO: [FR-5-3] 상태 Rejected, orderRepo.update 1회 호출
TEST_F(OrderServiceTest, RejectOrder_ValidOrder_UpdatesToRejected)
{
    // Arrange
    Order reservedOrder(L"ORD-001", L"S-001", L"홍길동", 5, L"2026-06-12");
    reservedOrder.setStatus(OrderStatus::Reserved);

    EXPECT_CALL(mockOrderRepo, findById(L"ORD-001"))
        .WillOnce(Return(std::optional<Order>(reservedOrder)));

    Order updatedOrder(L"", L"", L"", 0, L"");
    EXPECT_CALL(mockOrderRepo, update(_))
        .WillOnce(SaveArg<0>(&updatedOrder));

    OrderService service(mockOrderRepo, mockSampleRepo, mockProductionRepo);

    // Act
    service.rejectOrder(L"ORD-001");

    // Assert
    EXPECT_EQ(updatedOrder.getStatus(), OrderStatus::Rejected);
}

// ============================================================
// releaseOrder 테스트 (3건)
// ============================================================

// TODO: [FR-5-4] orderRepo.findById nullopt 시 예외
TEST_F(OrderServiceTest, ReleaseOrder_NotFound_Throws)
{
    // Arrange
    EXPECT_CALL(mockOrderRepo, findById(L"ORD-999"))
        .WillOnce(Return(std::nullopt));

    OrderService service(mockOrderRepo, mockSampleRepo, mockProductionRepo);

    // Act & Assert
    EXPECT_THROW(
        service.releaseOrder(L"ORD-999"),
        std::invalid_argument
    );
}

// TODO: [FR-5-4] 상태가 Reserved인 주문 출고 시 예외
TEST_F(OrderServiceTest, ReleaseOrder_NotConfirmed_Throws)
{
    // Arrange
    Order reservedOrder(L"ORD-001", L"S-001", L"홍길동", 5, L"2026-06-12");
    reservedOrder.setStatus(OrderStatus::Reserved);

    EXPECT_CALL(mockOrderRepo, findById(L"ORD-001"))
        .WillOnce(Return(std::optional<Order>(reservedOrder)));

    OrderService service(mockOrderRepo, mockSampleRepo, mockProductionRepo);

    // Act & Assert
    EXPECT_THROW(
        service.releaseOrder(L"ORD-001"),
        std::invalid_argument
    );
}

// TODO: [FR-5-4] 상태 Release, orderRepo.update 1회 호출
TEST_F(OrderServiceTest, ReleaseOrder_ValidOrder_UpdatesToRelease)
{
    // Arrange
    Order confirmedOrder(L"ORD-001", L"S-001", L"홍길동", 5, L"2026-06-12");
    confirmedOrder.setStatus(OrderStatus::Confirmed);

    EXPECT_CALL(mockOrderRepo, findById(L"ORD-001"))
        .WillOnce(Return(std::optional<Order>(confirmedOrder)));

    Order updatedOrder(L"", L"", L"", 0, L"");
    EXPECT_CALL(mockOrderRepo, update(_))
        .WillOnce(SaveArg<0>(&updatedOrder));

    OrderService service(mockOrderRepo, mockSampleRepo, mockProductionRepo);

    // Act
    service.releaseOrder(L"ORD-001");

    // Assert
    EXPECT_EQ(updatedOrder.getStatus(), OrderStatus::Release);
}

// ============================================================
// 조회 테스트 (2건)
// ============================================================

// TODO: [FR-5-5] findByStatus(Reserved) 결과 반환
TEST_F(OrderServiceTest, GetReservedOrders_ReturnsReservedList)
{
    // Arrange
    std::vector<Order> reservedOrders = {
        Order(L"ORD-001", L"S-001", L"홍길동", 5, L"2026-06-12"),
        Order(L"ORD-002", L"S-002", L"김철수", 3, L"2026-06-12")
    };

    EXPECT_CALL(mockOrderRepo, findByStatus(OrderStatus::Reserved))
        .WillOnce(Return(reservedOrders));

    OrderService service(mockOrderRepo, mockSampleRepo, mockProductionRepo);

    // Act
    std::vector<Order> result = service.getReservedOrders();

    // Assert
    EXPECT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0].getOrderId(), L"ORD-001");
    EXPECT_EQ(result[1].getOrderId(), L"ORD-002");
}

// TODO: [FR-5-7] findOrderById — 존재하는 orderId 조회 시 optional 값 반환
TEST_F(OrderServiceTest, FindOrderById_ExistsReturnsOrder)
{
    // Arrange
    Order order(L"ORD-001", L"S-001", L"홍길동", 5, L"2026-06-12");
    EXPECT_CALL(mockOrderRepo, findById(L"ORD-001"))
        .WillOnce(Return(std::optional<Order>(order)));

    OrderService service(mockOrderRepo, mockSampleRepo, mockProductionRepo);

    // Act
    std::optional<Order> result = service.findOrderById(L"ORD-001");

    // Assert
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->getOrderId(), L"ORD-001");
}

// TODO: [FR-5-7] findOrderById — 없는 orderId 조회 시 nullopt 반환
TEST_F(OrderServiceTest, FindOrderById_NotExistsReturnsNullopt)
{
    // Arrange
    EXPECT_CALL(mockOrderRepo, findById(L"ORD-999"))
        .WillOnce(Return(std::nullopt));

    OrderService service(mockOrderRepo, mockSampleRepo, mockProductionRepo);

    // Act
    std::optional<Order> result = service.findOrderById(L"ORD-999");

    // Assert
    EXPECT_FALSE(result.has_value());
}

// TODO: [FR-5-6] findByStatus(Confirmed) 결과 반환
TEST_F(OrderServiceTest, GetConfirmedOrders_ReturnsConfirmedList)
{
    // Arrange
    Order order1(L"ORD-003", L"S-001", L"이영희", 2, L"2026-06-12");
    order1.setStatus(OrderStatus::Confirmed);
    std::vector<Order> confirmedOrders = { order1 };

    EXPECT_CALL(mockOrderRepo, findByStatus(OrderStatus::Confirmed))
        .WillOnce(Return(confirmedOrders));

    OrderService service(mockOrderRepo, mockSampleRepo, mockProductionRepo);

    // Act
    std::vector<Order> result = service.getConfirmedOrders();

    // Assert
    EXPECT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0].getOrderId(), L"ORD-003");
}
