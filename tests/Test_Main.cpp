// [RED] 현재 구현이 없으므로 컴파일/링크 실패 예상
// ConsoleUtil.h 가 src/util/ 에 존재하지 않아 컴파일 오류 발생

#include <iostream>
#include "../src/util/ConsoleUtil.h"

#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            std::wcerr << L"[FAIL] " << (message) << L"\n"; \
            return false; \
        } \
    } while(0)

// TODO: [FR-1-2] ConsoleUtil::initEncoding() 함수가 존재하고 성공적으로 초기화되는지 확인
bool ConsoleUtil_InitEncoding_ReturnsTrueOnSuccess()
{
    // Arrange
    // (no preconditions)

    // Act
    bool result = ConsoleUtil::initEncoding();

    // Assert
    TEST_ASSERT(result == true, L"ConsoleUtil::initEncoding() 은 true 를 반환해야 합니다.");
    return true;
}

int main()
{
    int passed = 0;
    int failed = 0;

    auto run = [&](bool (*testFn)(), const wchar_t* name) {
        if (testFn()) {
            std::wcout << L"[PASS] " << name << L"\n";
            ++passed;
        } else {
            ++failed;
        }
    };

    run(ConsoleUtil_InitEncoding_ReturnsTrueOnSuccess,
        L"ConsoleUtil_InitEncoding_ReturnsTrueOnSuccess");

    std::wcout << L"\n결과: " << passed << L" 통과 / " << failed << L" 실패\n";
    return (failed > 0) ? 1 : 0;
}
