#include <gtest/gtest.h>
#include <filesystem>
#include <io.h>
#include <fcntl.h>
#include <cstdio>
#include "../src/util/ConsoleUtil.h"

namespace fs = std::filesystem;

// 프로젝트 루트: 이 테스트 파일 기준 두 단계 상위
static const fs::path PROJECT_ROOT =
    fs::path(__FILE__).parent_path().parent_path();

// ============================================================
// ConsoleUtilTest
// _O_U16TEXT 모드는 gtest 내부 printf 출력과 충돌하므로
// SetUp/TearDown 에서 stdio 모드를 저장하고 복원한다.
// ============================================================

class ConsoleUtilTest : public ::testing::Test
{
protected:
    int savedStdout = 0;
    int savedStdin  = 0;
    int savedStderr = 0;

    void SetUp() override
    {
        savedStdout = _setmode(_fileno(stdout), _O_TEXT);
        savedStdin  = _setmode(_fileno(stdin),  _O_TEXT);
        savedStderr = _setmode(_fileno(stderr), _O_TEXT);
    }

    void TearDown() override
    {
        _setmode(_fileno(stdout), savedStdout);
        _setmode(_fileno(stdin),  savedStdin);
        _setmode(_fileno(stderr), savedStderr);
    }
};

// TODO: [FR-1-2] ConsoleUtil::initEncoding() 이 true 를 반환하는지 확인
TEST_F(ConsoleUtilTest, InitEncodingReturnsTrue)
{
    // Act
    bool result = ConsoleUtil::initEncoding();

    // Assert
    EXPECT_TRUE(result);
}

// TODO: [FR-1-2] ConsoleUtil::initEncoding() 을 두 번 호출해도 true 를 반환하는지 확인
TEST_F(ConsoleUtilTest, InitEncodingCanBeCalledMultipleTimes)
{
    // Act
    bool firstCall  = ConsoleUtil::initEncoding();
    bool secondCall = ConsoleUtil::initEncoding();

    // Assert
    EXPECT_TRUE(firstCall);
    EXPECT_TRUE(secondCall);
}

// ============================================================
// DirectoryStructureTest
// ============================================================

// TODO: [FR-1-1] src/model/ 디렉터리가 존재하는지 확인
TEST(DirectoryStructureTest, SrcModelDirExists)
{
    // Arrange
    fs::path target = PROJECT_ROOT / "src" / "model";

    // Act & Assert
    EXPECT_TRUE(fs::exists(target) && fs::is_directory(target));
}

// TODO: [FR-1-1] src/view/ 디렉터리가 존재하는지 확인
TEST(DirectoryStructureTest, SrcViewDirExists)
{
    // Arrange
    fs::path target = PROJECT_ROOT / "src" / "view";

    // Act & Assert
    EXPECT_TRUE(fs::exists(target) && fs::is_directory(target));
}

// TODO: [FR-1-1] src/controller/ 디렉터리가 존재하는지 확인
TEST(DirectoryStructureTest, SrcControllerDirExists)
{
    // Arrange
    fs::path target = PROJECT_ROOT / "src" / "controller";

    // Act & Assert
    EXPECT_TRUE(fs::exists(target) && fs::is_directory(target));
}

// TODO: [FR-1-1] src/service/ 디렉터리가 존재하는지 확인
TEST(DirectoryStructureTest, SrcServiceDirExists)
{
    // Arrange
    fs::path target = PROJECT_ROOT / "src" / "service";

    // Act & Assert
    EXPECT_TRUE(fs::exists(target) && fs::is_directory(target));
}

// TODO: [FR-1-1] src/repository/ 디렉터리가 존재하는지 확인
TEST(DirectoryStructureTest, SrcRepositoryDirExists)
{
    // Arrange
    fs::path target = PROJECT_ROOT / "src" / "repository";

    // Act & Assert
    EXPECT_TRUE(fs::exists(target) && fs::is_directory(target));
}

// TODO: [FR-1-1] src/util/ 디렉터리가 존재하는지 확인
TEST(DirectoryStructureTest, SrcUtilDirExists)
{
    // Arrange
    fs::path target = PROJECT_ROOT / "src" / "util";

    // Act & Assert
    EXPECT_TRUE(fs::exists(target) && fs::is_directory(target));
}

// TODO: [FR-1-1] data/ 디렉터리가 존재하는지 확인
TEST(DirectoryStructureTest, DataDirExists)
{
    // Arrange
    fs::path target = PROJECT_ROOT / "data";

    // Act & Assert
    EXPECT_TRUE(fs::exists(target) && fs::is_directory(target));
}

// TODO: [FR-1-1] tests/ 디렉터리가 존재하는지 확인
TEST(DirectoryStructureTest, TestsDirExists)
{
    // Arrange
    fs::path target = PROJECT_ROOT / "tests";

    // Act & Assert
    EXPECT_TRUE(fs::exists(target) && fs::is_directory(target));
}

// ============================================================
// PlaceholderHeaderTest
// ============================================================

// TODO: [FR-1-1] src/model/Model.h placeholder 헤더가 존재하는지 확인
TEST(PlaceholderHeaderTest, ModelHeaderExists)
{
    // Arrange
    fs::path target = PROJECT_ROOT / "src" / "model" / "Model.h";

    // Act & Assert
    EXPECT_TRUE(fs::exists(target));
}

// TODO: [FR-1-1] src/view/View.h placeholder 헤더가 존재하는지 확인
TEST(PlaceholderHeaderTest, ViewHeaderExists)
{
    // Arrange
    fs::path target = PROJECT_ROOT / "src" / "view" / "View.h";

    // Act & Assert
    EXPECT_TRUE(fs::exists(target));
}

// TODO: [FR-1-1] src/controller/Controller.h placeholder 헤더가 존재하는지 확인
TEST(PlaceholderHeaderTest, ControllerHeaderExists)
{
    // Arrange
    fs::path target = PROJECT_ROOT / "src" / "controller" / "Controller.h";

    // Act & Assert
    EXPECT_TRUE(fs::exists(target));
}

// TODO: [FR-1-1] src/service/Service.h placeholder 헤더가 존재하는지 확인
TEST(PlaceholderHeaderTest, ServiceHeaderExists)
{
    // Arrange
    fs::path target = PROJECT_ROOT / "src" / "service" / "Service.h";

    // Act & Assert
    EXPECT_TRUE(fs::exists(target));
}

// TODO: [FR-1-1] src/repository/Repository.h placeholder 헤더가 존재하는지 확인
TEST(PlaceholderHeaderTest, RepositoryHeaderExists)
{
    // Arrange
    fs::path target = PROJECT_ROOT / "src" / "repository" / "Repository.h";

    // Act & Assert
    EXPECT_TRUE(fs::exists(target));
}

// TODO: [FR-1-2] src/util/ConsoleUtil.h 헤더가 존재하는지 확인
TEST(PlaceholderHeaderTest, ConsoleUtilHeaderExists)
{
    // Arrange
    fs::path target = PROJECT_ROOT / "src" / "util" / "ConsoleUtil.h";

    // Act & Assert
    EXPECT_TRUE(fs::exists(target));
}
