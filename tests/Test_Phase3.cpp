// Test_Phase3.cpp — Phase 3 Repository 계층 단위 테스트 (Red 상태)
// 인코딩: UTF-8 with BOM

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <optional>
#include <vector>
#include <cstdio>

#include "../src/model/Sample.h"
#include "../src/model/Order.h"
#include "../src/model/ProductionJob.h"
#include "repository/ISampleRepository.h"
#include "repository/IOrderRepository.h"
#include "repository/IProductionRepository.h"
#include "repository/JsonSampleRepository.h"
#include "repository/JsonOrderRepository.h"
#include "repository/JsonProductionRepository.h"

using ::testing::Return;
using ::testing::_;

// ============================================================
// Mock 정의
// ============================================================

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
// MockSampleRepositoryTest
// ============================================================

// TODO: [FR-3-1] add 호출 시 Mock의 add가 1회 호출됨
TEST(MockSampleRepositoryTest, AddCallsRepository)
{
    // Arrange
    MockSampleRepository repo;
    Sample sample(L"S-001", L"테스트시료", 30.0, 0.95, 100);

    EXPECT_CALL(repo, add(testing::_)).Times(1);

    // Act
    repo.add(sample);
}

// TODO: [FR-3-1] findById 호출 시 optional 반환
TEST(MockSampleRepositoryTest, FindByIdReturnsOptional)
{
    // Arrange
    MockSampleRepository repo;
    Sample sample(L"S-001", L"테스트시료", 30.0, 0.95, 100);

    EXPECT_CALL(repo, findById(L"S-001"))
        .WillOnce(Return(std::optional<Sample>(sample)));

    // Act
    std::optional<Sample> result = repo.findById(L"S-001");

    // Assert
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->getSampleId(), L"S-001");
}

// TODO: [FR-3-1] findAll 호출 시 vector 반환
TEST(MockSampleRepositoryTest, FindAllReturnsVector)
{
    // Arrange
    MockSampleRepository repo;
    std::vector<Sample> samples = {
        Sample(L"S-001", L"시료A", 10.0, 0.9, 50),
        Sample(L"S-002", L"시료B", 20.0, 0.8, 30)
    };

    EXPECT_CALL(repo, findAll()).WillOnce(Return(samples));

    // Act
    std::vector<Sample> result = repo.findAll();

    // Assert
    EXPECT_EQ(result.size(), 2u);
}

// TODO: [FR-3-1] update 호출 시 1회 호출됨
TEST(MockSampleRepositoryTest, UpdateCallsRepository)
{
    // Arrange
    MockSampleRepository repo;
    Sample sample(L"S-001", L"수정시료", 25.0, 0.85, 200);

    EXPECT_CALL(repo, update(testing::_)).Times(1);

    // Act
    repo.update(sample);
}

// TODO: [FR-3-1] remove 호출 시 1회 호출됨
TEST(MockSampleRepositoryTest, RemoveCallsRepository)
{
    // Arrange
    MockSampleRepository repo;

    EXPECT_CALL(repo, remove(L"S-001")).Times(1);

    // Act
    repo.remove(L"S-001");
}

// ============================================================
// MockOrderRepositoryTest
// ============================================================

// TODO: [FR-3-1] add 호출 시 Mock의 add가 1회 호출됨
TEST(MockOrderRepositoryTest, AddCallsRepository)
{
    // Arrange
    MockOrderRepository repo;
    Order order(L"O-001", L"S-001", L"홍길동", 5, L"2026-06-12");

    EXPECT_CALL(repo, add(testing::_)).Times(1);

    // Act
    repo.add(order);
}

// TODO: [FR-3-1] findByStatus 호출 시 OrderStatus 파라미터 전달
TEST(MockOrderRepositoryTest, FindByStatusReturnsFiltered)
{
    // Arrange
    MockOrderRepository repo;
    std::vector<Order> reservedOrders = {
        Order(L"O-001", L"S-001", L"홍길동", 5, L"2026-06-12"),
        Order(L"O-002", L"S-002", L"김철수", 3, L"2026-06-12")
    };

    EXPECT_CALL(repo, findByStatus(OrderStatus::Reserved))
        .WillOnce(Return(reservedOrders));

    // Act
    std::vector<Order> result = repo.findByStatus(OrderStatus::Reserved);

    // Assert
    EXPECT_EQ(result.size(), 2u);
}

// TODO: [FR-3-1] update 호출 시 1회 호출됨
TEST(MockOrderRepositoryTest, UpdateCallsRepository)
{
    // Arrange
    MockOrderRepository repo;
    Order order(L"O-001", L"S-001", L"홍길동", 5, L"2026-06-12");

    EXPECT_CALL(repo, update(testing::_)).Times(1);

    // Act
    repo.update(order);
}

// TODO: [FR-3-1] remove 호출 시 1회 호출됨
TEST(MockOrderRepositoryTest, RemoveCallsRepository)
{
    // Arrange
    MockOrderRepository repo;

    EXPECT_CALL(repo, remove(L"O-001")).Times(1);

    // Act
    repo.remove(L"O-001");
}

// ============================================================
// MockProductionRepositoryTest
// ============================================================

// TODO: [FR-3-1] enqueue 호출 시 Mock의 enqueue가 1회 호출됨
TEST(MockProductionRepositoryTest, EnqueueCallsRepository)
{
    // Arrange
    MockProductionRepository repo;
    ProductionJob job(L"J-001", L"O-001", L"S-001", 5, 10, 120.0, L"2026-06-12 09:00");

    EXPECT_CALL(repo, enqueue(testing::_)).Times(1);

    // Act
    repo.enqueue(job);
}

// TODO: [FR-3-1] front 호출 시 optional 반환
TEST(MockProductionRepositoryTest, FrontReturnsOptional)
{
    // Arrange
    MockProductionRepository repo;
    ProductionJob job(L"J-001", L"O-001", L"S-001", 5, 10, 120.0, L"2026-06-12 09:00");

    EXPECT_CALL(repo, front())
        .WillOnce(Return(std::optional<ProductionJob>(job)));

    // Act
    std::optional<ProductionJob> result = repo.front();

    // Assert
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->getJobId(), L"J-001");
}

// TODO: [FR-3-1] dequeue 호출 시 1회 호출됨
TEST(MockProductionRepositoryTest, DequeueCallsRepository)
{
    // Arrange
    MockProductionRepository repo;

    EXPECT_CALL(repo, dequeue()).Times(1);

    // Act
    repo.dequeue();
}

// TODO: [FR-3-1] isEmpty 호출 시 bool 반환
TEST(MockProductionRepositoryTest, IsEmptyReturnsBool)
{
    // Arrange
    MockProductionRepository repo;

    EXPECT_CALL(repo, isEmpty()).WillOnce(Return(true));

    // Act
    bool result = repo.isEmpty();

    // Assert
    EXPECT_TRUE(result);
}

// ============================================================
// JsonSampleRepositoryTest
// ============================================================

class JsonSampleRepositoryTest : public ::testing::Test
{
protected:
    std::wstring testFilePath = L"test_samples_temp.json";

    void TearDown() override
    {
        _wremove(testFilePath.c_str());
    }
};

// TODO: [FR-3-2] 파일 없을 때 findAll() 빈 vector 반환
TEST_F(JsonSampleRepositoryTest, InitiallyEmpty)
{
    // Arrange
    JsonSampleRepository repo(testFilePath);

    // Act
    std::vector<Sample> result = repo.findAll();

    // Assert
    EXPECT_TRUE(result.empty());
}

// TODO: [FR-3-2] add 후 findById로 조회 가능
TEST_F(JsonSampleRepositoryTest, AddAndFindById)
{
    // Arrange
    JsonSampleRepository repo(testFilePath);
    Sample sample(L"S-001", L"테스트시료", 30.0, 0.95, 100);

    // Act
    repo.add(sample);
    std::optional<Sample> result = repo.findById(L"S-001");

    // Assert
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->getSampleId(), L"S-001");
    EXPECT_EQ(result->getName(), L"테스트시료");
}

// TODO: [FR-3-2] add 2개 후 findAll 2개 반환
TEST_F(JsonSampleRepositoryTest, AddAndFindAll)
{
    // Arrange
    JsonSampleRepository repo(testFilePath);
    Sample sample1(L"S-001", L"시료A", 10.0, 0.9, 50);
    Sample sample2(L"S-002", L"시료B", 20.0, 0.8, 30);

    // Act
    repo.add(sample1);
    repo.add(sample2);
    std::vector<Sample> result = repo.findAll();

    // Assert
    EXPECT_EQ(result.size(), 2u);
}

// TODO: [FR-3-2] update 후 findById 값 변경
TEST_F(JsonSampleRepositoryTest, UpdateChangesData)
{
    // Arrange
    JsonSampleRepository repo(testFilePath);
    Sample original(L"S-001", L"원본시료", 30.0, 0.90, 100);
    repo.add(original);

    Sample updated(L"S-001", L"수정시료", 45.0, 0.85, 200);

    // Act
    repo.update(updated);
    std::optional<Sample> result = repo.findById(L"S-001");

    // Assert
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->getName(), L"수정시료");
    EXPECT_EQ(result->getStock(), 200);
}

// TODO: [FR-3-2] remove 후 findById nullopt
TEST_F(JsonSampleRepositoryTest, RemoveDeletesItem)
{
    // Arrange
    JsonSampleRepository repo(testFilePath);
    Sample sample(L"S-001", L"삭제시료", 30.0, 0.95, 100);
    repo.add(sample);

    // Act
    repo.remove(L"S-001");
    std::optional<Sample> result = repo.findById(L"S-001");

    // Assert
    EXPECT_FALSE(result.has_value());
}

// TODO: [FR-3-2] 같은 파일 경로로 새 인스턴스 생성 시 데이터 유지
TEST_F(JsonSampleRepositoryTest, PersistsAcrossInstances)
{
    // Arrange
    {
        JsonSampleRepository repo1(testFilePath);
        Sample sample(L"S-001", L"영속시료", 30.0, 0.95, 100);
        repo1.add(sample);
    }

    // Act
    JsonSampleRepository repo2(testFilePath);
    std::optional<Sample> result = repo2.findById(L"S-001");

    // Assert
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->getSampleId(), L"S-001");
}

// ============================================================
// JsonOrderRepositoryTest
// ============================================================

class JsonOrderRepositoryTest : public ::testing::Test
{
protected:
    std::wstring testFilePath = L"test_orders_temp.json";

    void TearDown() override
    {
        _wremove(testFilePath.c_str());
    }
};

// TODO: [FR-3-2] 파일 없을 때 findAll() 빈 vector 반환
TEST_F(JsonOrderRepositoryTest, InitiallyEmpty)
{
    // Arrange
    JsonOrderRepository repo(testFilePath);

    // Act
    std::vector<Order> result = repo.findAll();

    // Assert
    EXPECT_TRUE(result.empty());
}

// TODO: [FR-3-2] add 후 findById로 조회 가능
TEST_F(JsonOrderRepositoryTest, AddAndFindById)
{
    // Arrange
    JsonOrderRepository repo(testFilePath);
    Order order(L"O-001", L"S-001", L"홍길동", 5, L"2026-06-12");

    // Act
    repo.add(order);
    std::optional<Order> result = repo.findById(L"O-001");

    // Assert
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->getOrderId(), L"O-001");
    EXPECT_EQ(result->getCustomerName(), L"홍길동");
}

// TODO: [FR-3-2] Reserved 상태 2개 추가 후 findByStatus(Reserved) 2개 반환
TEST_F(JsonOrderRepositoryTest, FindByStatusReturnsMatching)
{
    // Arrange
    JsonOrderRepository repo(testFilePath);
    Order order1(L"O-001", L"S-001", L"홍길동", 5, L"2026-06-12");
    Order order2(L"O-002", L"S-002", L"김철수", 3, L"2026-06-12");

    // Act
    repo.add(order1);
    repo.add(order2);
    std::vector<Order> result = repo.findByStatus(OrderStatus::Reserved);

    // Assert
    EXPECT_EQ(result.size(), 2u);
    for (const auto& o : result)
    {
        EXPECT_EQ(o.getStatus(), OrderStatus::Reserved);
    }
}

// TODO: [FR-3-2] setStatus 후 update, 재조회 시 변경 확인
TEST_F(JsonOrderRepositoryTest, UpdateChangesStatus)
{
    // Arrange
    JsonOrderRepository repo(testFilePath);
    Order order(L"O-001", L"S-001", L"홍길동", 5, L"2026-06-12");
    repo.add(order);

    order.setStatus(OrderStatus::Confirmed);

    // Act
    repo.update(order);
    std::optional<Order> result = repo.findById(L"O-001");

    // Assert
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->getStatus(), OrderStatus::Confirmed);
}

// TODO: [FR-3-2] remove 후 findById nullopt
TEST_F(JsonOrderRepositoryTest, RemoveDeletesItem)
{
    // Arrange
    JsonOrderRepository repo(testFilePath);
    Order order(L"O-001", L"S-001", L"홍길동", 5, L"2026-06-12");
    repo.add(order);

    // Act
    repo.remove(L"O-001");
    std::optional<Order> result = repo.findById(L"O-001");

    // Assert
    EXPECT_FALSE(result.has_value());
}

// TODO: [FR-3-2] 같은 파일 경로로 새 인스턴스 생성 시 데이터 유지
TEST_F(JsonOrderRepositoryTest, PersistsAcrossInstances)
{
    // Arrange
    {
        JsonOrderRepository repo1(testFilePath);
        Order order(L"O-001", L"S-001", L"홍길동", 5, L"2026-06-12");
        repo1.add(order);
    }

    // Act
    JsonOrderRepository repo2(testFilePath);
    std::optional<Order> result = repo2.findById(L"O-001");

    // Assert
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->getOrderId(), L"O-001");
}

// ============================================================
// JsonProductionRepositoryTest
// ============================================================

class JsonProductionRepositoryTest : public ::testing::Test
{
protected:
    std::wstring testFilePath = L"test_production_temp.json";

    void TearDown() override
    {
        _wremove(testFilePath.c_str());
    }
};

// TODO: [FR-3-2] 파일 없을 때 isEmpty()가 true이고 findAll()이 빈 vector 반환
TEST_F(JsonProductionRepositoryTest, InitiallyEmptyAndIsEmptyTrue)
{
    // Arrange
    JsonProductionRepository repo(testFilePath);

    // Act & Assert
    EXPECT_TRUE(repo.isEmpty());
    EXPECT_TRUE(repo.findAll().empty());
}

// TODO: [FR-3-2] enqueue 2개 후 front()는 첫 번째
TEST_F(JsonProductionRepositoryTest, EnqueueAndFrontReturnsFirst)
{
    // Arrange
    JsonProductionRepository repo(testFilePath);
    ProductionJob job1(L"J-001", L"O-001", L"S-001", 5, 10, 120.0, L"2026-06-12 09:00");
    ProductionJob job2(L"J-002", L"O-002", L"S-002", 3, 6, 60.0, L"2026-06-12 10:00");

    // Act
    repo.enqueue(job1);
    repo.enqueue(job2);
    std::optional<ProductionJob> result = repo.front();

    // Assert
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->getJobId(), L"J-001");
}

// TODO: [FR-3-2] dequeue 후 front()는 두 번째
TEST_F(JsonProductionRepositoryTest, DequeueRemovesFirst)
{
    // Arrange
    JsonProductionRepository repo(testFilePath);
    ProductionJob job1(L"J-001", L"O-001", L"S-001", 5, 10, 120.0, L"2026-06-12 09:00");
    ProductionJob job2(L"J-002", L"O-002", L"S-002", 3, 6, 60.0, L"2026-06-12 10:00");
    repo.enqueue(job1);
    repo.enqueue(job2);

    // Act
    repo.dequeue();
    std::optional<ProductionJob> result = repo.front();

    // Assert
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->getJobId(), L"J-002");
}

// TODO: [FR-3-2] findAll 전체 항목 반환
TEST_F(JsonProductionRepositoryTest, FindAllReturnsAll)
{
    // Arrange
    JsonProductionRepository repo(testFilePath);
    ProductionJob job1(L"J-001", L"O-001", L"S-001", 5, 10, 120.0, L"2026-06-12 09:00");
    ProductionJob job2(L"J-002", L"O-002", L"S-002", 3, 6, 60.0, L"2026-06-12 10:00");
    ProductionJob job3(L"J-003", L"O-003", L"S-003", 1, 4, 45.0, L"2026-06-12 11:00");

    // Act
    repo.enqueue(job1);
    repo.enqueue(job2);
    repo.enqueue(job3);
    std::vector<ProductionJob> result = repo.findAll();

    // Assert
    EXPECT_EQ(result.size(), 3u);
}

// TODO: [FR-3-2] 전체 dequeue 후 isEmpty() true
TEST_F(JsonProductionRepositoryTest, IsEmptyAfterDequeueAll)
{
    // Arrange
    JsonProductionRepository repo(testFilePath);
    ProductionJob job(L"J-001", L"O-001", L"S-001", 5, 10, 120.0, L"2026-06-12 09:00");
    repo.enqueue(job);

    // Act
    repo.dequeue();

    // Assert
    EXPECT_TRUE(repo.isEmpty());
}

// TODO: [FR-3-2] 같은 파일 경로로 새 인스턴스 생성 시 데이터 유지
TEST_F(JsonProductionRepositoryTest, PersistsAcrossInstances)
{
    // Arrange
    {
        JsonProductionRepository repo1(testFilePath);
        ProductionJob job(L"J-001", L"O-001", L"S-001", 5, 10, 120.0, L"2026-06-12 09:00");
        repo1.enqueue(job);
    }

    // Act
    JsonProductionRepository repo2(testFilePath);
    std::optional<ProductionJob> result = repo2.front();

    // Assert
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->getJobId(), L"J-001");
}
