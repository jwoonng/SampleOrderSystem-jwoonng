// Test_Phase4.cpp — Phase 4 SampleService 단위 테스트 (Red 상태)
// 인코딩: UTF-8 with BOM

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <optional>
#include <vector>
#include <stdexcept>

#include "../src/model/Sample.h"
#include "repository/ISampleRepository.h"
#include "service/SampleService.h"   // 아직 없으므로 컴파일 실패 (정상)

using ::testing::Return;
using ::testing::_;

// ============================================================
// MockSampleRepository
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

// ============================================================
// SampleServiceTest Fixture
// ============================================================

class SampleServiceTest : public ::testing::Test
{
protected:
    MockSampleRepository mockRepo;
    // SampleService는 아직 없으므로 포인터 선언만
    // (컴파일 실패가 Red 단계의 목표)
};

// ============================================================
// 등록 테스트 (RegisterSample)
// ============================================================

// TODO: [FR-4-1] 정상 등록 시 repo.add 1회 호출
TEST_F(SampleServiceTest, RegisterSample_ValidSample_CallsRepoAdd)
{
    // Arrange
    Sample sample(L"S-001", L"테스트시료", 30.0, 0.95, 100);

    EXPECT_CALL(mockRepo, findById(L"S-001"))
        .WillOnce(Return(std::nullopt));
    EXPECT_CALL(mockRepo, add(_)).Times(1);

    SampleService service(mockRepo);

    // Act
    service.registerSample(sample);
}

// TODO: [FR-4-1] 중복 ID 시 std::invalid_argument 예외 발생
TEST_F(SampleServiceTest, RegisterSample_DuplicateId_ThrowsInvalidArgument)
{
    // Arrange
    Sample sample(L"S-001", L"테스트시료", 30.0, 0.95, 100);

    EXPECT_CALL(mockRepo, findById(L"S-001"))
        .WillOnce(Return(std::optional<Sample>(sample)));

    SampleService service(mockRepo);

    // Act & Assert
    EXPECT_THROW(service.registerSample(sample), std::invalid_argument);
}

// TODO: [FR-4-1] ID 형식이 "ABC"이면 std::invalid_argument 예외 발생
TEST_F(SampleServiceTest, RegisterSample_InvalidIdFormat_ThrowsInvalidArgument)
{
    // Arrange
    Sample sample(L"ABC", L"테스트시료", 30.0, 0.95, 100);

    SampleService service(mockRepo);

    // Act & Assert
    EXPECT_THROW(service.registerSample(sample), std::invalid_argument);
}

// TODO: [FR-4-1] yield=0.0이면 std::invalid_argument 예외 발생
TEST_F(SampleServiceTest, RegisterSample_InvalidYieldZero_ThrowsInvalidArgument)
{
    // Arrange
    Sample sample(L"S-001", L"테스트시료", 30.0, 0.0, 100);

    SampleService service(mockRepo);

    // Act & Assert
    EXPECT_THROW(service.registerSample(sample), std::invalid_argument);
}

// TODO: [FR-4-1] yield=1.1이면 std::invalid_argument 예외 발생
TEST_F(SampleServiceTest, RegisterSample_InvalidYieldOverOne_ThrowsInvalidArgument)
{
    // Arrange
    Sample sample(L"S-001", L"테스트시료", 30.0, 1.1, 100);

    SampleService service(mockRepo);

    // Act & Assert
    EXPECT_THROW(service.registerSample(sample), std::invalid_argument);
}

// TODO: [FR-4-1] avgProdTime=0.0이면 std::invalid_argument 예외 발생
TEST_F(SampleServiceTest, RegisterSample_InvalidAvgProdTimeZero_ThrowsInvalidArgument)
{
    // Arrange
    Sample sample(L"S-001", L"테스트시료", 0.0, 0.95, 100);

    SampleService service(mockRepo);

    // Act & Assert
    EXPECT_THROW(service.registerSample(sample), std::invalid_argument);
}

// ============================================================
// 조회 테스트 (GetAllSamples)
// ============================================================

// TODO: [FR-4-2] repo.findAll() 결과를 그대로 반환
TEST_F(SampleServiceTest, GetAllSamples_ReturnsAllFromRepo)
{
    // Arrange
    std::vector<Sample> samples = {
        Sample(L"S-001", L"시료A", 10.0, 0.9, 50),
        Sample(L"S-002", L"시료B", 20.0, 0.8, 30)
    };

    EXPECT_CALL(mockRepo, findAll()).WillOnce(Return(samples));

    SampleService service(mockRepo);

    // Act
    std::vector<Sample> result = service.getAllSamples();

    // Assert
    EXPECT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0].getSampleId(), L"S-001");
    EXPECT_EQ(result[1].getSampleId(), L"S-002");
}

// TODO: [FR-4-2] 빈 저장소인 경우 빈 vector 반환
TEST_F(SampleServiceTest, GetAllSamples_EmptyRepo_ReturnsEmptyVector)
{
    // Arrange
    EXPECT_CALL(mockRepo, findAll()).WillOnce(Return(std::vector<Sample>{}));

    SampleService service(mockRepo);

    // Act
    std::vector<Sample> result = service.getAllSamples();

    // Assert
    EXPECT_TRUE(result.empty());
}

// ============================================================
// 단건 조회 테스트 (FindSampleById)
// ============================================================

// TODO: [FR-4-3] 존재하는 ID 조회 시 optional 값 반환
TEST_F(SampleServiceTest, FindSampleById_ExistsReturnsOptional)
{
    // Arrange
    Sample sample(L"S-001", L"테스트시료", 30.0, 0.95, 100);

    EXPECT_CALL(mockRepo, findById(L"S-001"))
        .WillOnce(Return(std::optional<Sample>(sample)));

    SampleService service(mockRepo);

    // Act
    std::optional<Sample> result = service.findSampleById(L"S-001");

    // Assert
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->getSampleId(), L"S-001");
}

// TODO: [FR-4-3] 없는 ID 조회 시 nullopt 반환
TEST_F(SampleServiceTest, FindSampleById_NotExistsReturnsNullopt)
{
    // Arrange
    EXPECT_CALL(mockRepo, findById(L"S-999"))
        .WillOnce(Return(std::nullopt));

    SampleService service(mockRepo);

    // Act
    std::optional<Sample> result = service.findSampleById(L"S-999");

    // Assert
    EXPECT_FALSE(result.has_value());
}

// ============================================================
// 검색 테스트 (SearchSamples)
// ============================================================

// TODO: [FR-4-4] 이름에 키워드가 포함된 항목만 반환
TEST_F(SampleServiceTest, SearchSamples_ByName_ReturnsMatching)
{
    // Arrange
    std::vector<Sample> allSamples = {
        Sample(L"S-001", L"알파시료", 10.0, 0.9, 50),
        Sample(L"S-002", L"베타시료", 20.0, 0.8, 30),
        Sample(L"S-003", L"알파베타", 15.0, 0.85, 40)
    };

    EXPECT_CALL(mockRepo, findAll()).WillOnce(Return(allSamples));

    SampleService service(mockRepo);

    // Act
    std::vector<Sample> result = service.searchSamples(L"알파");

    // Assert
    EXPECT_EQ(result.size(), 2u);
}

// TODO: [FR-4-4] ID에 키워드가 포함된 항목만 반환
TEST_F(SampleServiceTest, SearchSamples_ById_ReturnsMatching)
{
    // Arrange
    std::vector<Sample> allSamples = {
        Sample(L"S-001", L"시료A", 10.0, 0.9, 50),
        Sample(L"S-002", L"시료B", 20.0, 0.8, 30),
        Sample(L"S-010", L"시료C", 15.0, 0.85, 40)
    };

    EXPECT_CALL(mockRepo, findAll()).WillOnce(Return(allSamples));

    SampleService service(mockRepo);

    // Act
    std::vector<Sample> result = service.searchSamples(L"S-00");

    // Assert
    EXPECT_EQ(result.size(), 2u);
}

// TODO: [FR-4-4] 매칭 항목 없으면 빈 vector 반환
TEST_F(SampleServiceTest, SearchSamples_NoMatch_ReturnsEmpty)
{
    // Arrange
    std::vector<Sample> allSamples = {
        Sample(L"S-001", L"알파시료", 10.0, 0.9, 50),
        Sample(L"S-002", L"베타시료", 20.0, 0.8, 30)
    };

    EXPECT_CALL(mockRepo, findAll()).WillOnce(Return(allSamples));

    SampleService service(mockRepo);

    // Act
    std::vector<Sample> result = service.searchSamples(L"감마");

    // Assert
    EXPECT_TRUE(result.empty());
}
