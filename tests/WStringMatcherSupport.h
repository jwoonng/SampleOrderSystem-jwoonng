// encoding: UTF-8 with BOM
// WStringMatcherSupport.h
// GMock에서 const wchar_t* 리터럴을 Matcher<const std::wstring&>로 변환 지원
#pragma once

#ifdef GTEST_INCLUDE_GTEST_GTEST_MATCHERS_H_
// gtest-matchers.h가 이미 포함된 경우에만 특수화 추가
#else

#include <gtest/gtest-matchers.h>

namespace testing {

template <>
class GTEST_API_ Matcher<const std::wstring&>
    : public internal::MatcherBase<const std::wstring&> {
public:
    Matcher() {}

    explicit Matcher(const MatcherInterface<const std::wstring&>* impl)
        : internal::MatcherBase<const std::wstring&>(impl) {}

    template <typename M, typename = typename std::remove_reference<
                              M>::type::is_gtest_matcher>
    Matcher(M&& m)
        : internal::MatcherBase<const std::wstring&>(std::forward<M>(m)) {}

    // wstring으로 직접 생성
    Matcher(const std::wstring& s);  // NOLINT

    // wchar_t* 리터럴에서 암시적 변환
    Matcher(const wchar_t* s);  // NOLINT
};

template <>
class GTEST_API_ Matcher<std::wstring>
    : public internal::MatcherBase<std::wstring> {
public:
    Matcher() {}

    explicit Matcher(const MatcherInterface<const std::wstring&>* impl)
        : internal::MatcherBase<std::wstring>(impl) {}
    explicit Matcher(const MatcherInterface<std::wstring>* impl)
        : internal::MatcherBase<std::wstring>(impl) {}

    template <typename M, typename = typename std::remove_reference<
                              M>::type::is_gtest_matcher>
    Matcher(M&& m)
        : internal::MatcherBase<std::wstring>(std::forward<M>(m)) {}

    Matcher(const std::wstring& s);  // NOLINT
    Matcher(const wchar_t* s);  // NOLINT
};

}  // namespace testing

#endif