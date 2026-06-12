// [RED] gtest 라이브러리 미설치로 컴파일 실패 예상
// external/googletest 가 설치되기 전까지 #include <gtest/gtest.h> 로 인해
// 컴파일 오류가 발생합니다. 이것이 Red 상태의 의도된 조건입니다.

#include <gtest/gtest.h>

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
