// Test_Phase7.cpp — Phase 7 유틸리티 클래스 단위 테스트 (Red 상태)
// 인코딩: UTF-8 with BOM

#include <gtest/gtest.h>
#include <vector>
#include <string>

#include "../src/model/Order.h"
#include "../src/util/ConsoleHelper.h"    // 아직 없으므로 컴파일 실패 (정상)
#include "../src/util/OrderIdGenerator.h" // 아직 없으므로 컴파일 실패 (정상)

// ============================================================
// ConsoleHelper — buildProgressBar 테스트 (5건)
// ============================================================

// TODO: [FR-7-1] 0% 진행바는 모두 빈 블록(░)으로 구성됨
TEST(ConsoleHelperTest, BuildProgressBar_ZeroPercent_AllEmpty)
{
    // Arrange & Act
    std::wstring result = ConsoleHelper::buildProgressBar(0, 10);

    // Assert
    EXPECT_EQ(result, L"░░░░░░░░░░");
}

// TODO: [FR-7-1] 100% 진행바는 모두 채운 블록(█)으로 구성됨
TEST(ConsoleHelperTest, BuildProgressBar_HundredPercent_AllFilled)
{
    // Arrange & Act
    std::wstring result = ConsoleHelper::buildProgressBar(100, 10);

    // Assert
    EXPECT_EQ(result, L"██████████");
}

// TODO: [FR-7-1] 50% 진행바는 절반이 채운 블록, 절반이 빈 블록
TEST(ConsoleHelperTest, BuildProgressBar_FiftyPercent_HalfFilled)
{
    // Arrange & Act
    std::wstring result = ConsoleHelper::buildProgressBar(50, 10);

    // Assert
    EXPECT_EQ(result, L"█████░░░░░");
}

// TODO: [FR-7-1] 80% 진행바는 8칸 채운 블록, 2칸 빈 블록
TEST(ConsoleHelperTest, BuildProgressBar_EightyPercent_EightFilled)
{
    // Arrange & Act
    std::wstring result = ConsoleHelper::buildProgressBar(80, 10);

    // Assert
    EXPECT_EQ(result, L"████████░░");
}

// TODO: [FR-7-1] width=5 진행바의 길이는 정확히 5
TEST(ConsoleHelperTest, BuildProgressBar_Width5_CorrectLength)
{
    // Arrange & Act
    std::wstring result = ConsoleHelper::buildProgressBar(60, 5);

    // Assert
    EXPECT_EQ(result.length(), static_cast<std::size_t>(5));
}

// ============================================================
// ConsoleHelper — getStockStatus 테스트 (4건)
// ============================================================

// TODO: [FR-7-2] stock==0 이면 "고갈" 반환
TEST(ConsoleHelperTest, GetStockStatus_ZeroStock_ReturnsDepleted)
{
    // Arrange & Act
    std::wstring result = ConsoleHelper::getStockStatus(0, 100);

    // Assert
    EXPECT_EQ(result, L"고갈");
}

// TODO: [FR-7-2] stock==0, pending==0 이어도 "고갈" 반환
TEST(ConsoleHelperTest, GetStockStatus_ZeroStockZeroPending_ReturnsDepleted)
{
    // Arrange & Act
    std::wstring result = ConsoleHelper::getStockStatus(0, 0);

    // Assert
    EXPECT_EQ(result, L"고갈");
}

// TODO: [FR-7-2] stock > 0 이고 stock < pending 이면 "부족" 반환
TEST(ConsoleHelperTest, GetStockStatus_StockLessThanPending_ReturnsShort)
{
    // Arrange & Act
    std::wstring result = ConsoleHelper::getStockStatus(30, 200);

    // Assert
    EXPECT_EQ(result, L"부족");
}

// TODO: [FR-7-2] stock >= pending 이면 "여유" 반환
TEST(ConsoleHelperTest, GetStockStatus_StockEqualOrMoreThanPending_ReturnsSufficient)
{
    // Arrange & Act (stock > pending)
    std::wstring result1 = ConsoleHelper::getStockStatus(480, 100);

    // Arrange & Act (stock == pending)
    std::wstring result2 = ConsoleHelper::getStockStatus(100, 100);

    // Assert
    EXPECT_EQ(result1, L"여유");
    EXPECT_EQ(result2, L"여유");
}

// ============================================================
// OrderIdGenerator — generate 테스트 (4건)
// ============================================================

// TODO: [FR-7-3] 주문 0건 → 시퀀스 번호 0001
TEST(OrderIdGeneratorTest, Generate_ZeroOrders_ReturnsPaddedOne)
{
    // Arrange
    std::vector<Order> emptyOrders;

    // Act
    std::wstring result = OrderIdGenerator::generate(emptyOrders, L"20260612");

    // Assert
    EXPECT_EQ(result, L"ORD-20260612-0001");
}

// TODO: [FR-7-3] 주문 5건 → 시퀀스 번호 0006
TEST(OrderIdGeneratorTest, Generate_FiveOrders_ReturnsSix)
{
    // Arrange
    std::vector<Order> orders;
    for (int i = 0; i < 5; ++i)
    {
        orders.emplace_back(
            L"ORD-00" + std::to_wstring(i + 1),
            L"S-001",
            L"고객",
            10,
            L"2026-06-12"
        );
    }

    // Act
    std::wstring result = OrderIdGenerator::generate(orders, L"20260612");

    // Assert
    EXPECT_EQ(result, L"ORD-20260612-0006");
}

// TODO: [FR-7-3] 주문 9건 → 시퀀스 번호 0010 (4자리 패딩)
TEST(OrderIdGeneratorTest, Generate_NineOrders_ReturnsTen)
{
    // Arrange
    std::vector<Order> orders;
    for (int i = 0; i < 9; ++i)
    {
        orders.emplace_back(
            L"ORD-00" + std::to_wstring(i + 1),
            L"S-001",
            L"고객",
            10,
            L"2026-06-12"
        );
    }

    // Act
    std::wstring result = OrderIdGenerator::generate(orders, L"20260612");

    // Assert
    EXPECT_EQ(result, L"ORD-20260612-0010");
}

// TODO: [FR-7-3] 결과 ID에 입력된 dateStr이 포함됨
TEST(OrderIdGeneratorTest, Generate_DatePrefixMatchesInput)
{
    // Arrange
    std::vector<Order> emptyOrders;

    // Act
    std::wstring result = OrderIdGenerator::generate(emptyOrders, L"20260101");

    // Assert
    EXPECT_TRUE(result.find(L"20260101") != std::wstring::npos);
}
