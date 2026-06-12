// encoding: UTF-8 with BOM
// WStringGMockFix.h
// GMock Matcher<const std::wstring&> 특수화 — const wchar_t* 리터럴 지원
#pragma once
#include <gtest/gtest-matchers.h>
#include <string>

// wstring equality matcher 헬퍼
namespace testing {
namespace internal {

class WStringEqMatcher {
public:
    explicit WStringEqMatcher(std::wstring expected) : expected_(std::move(expected)) {}
    bool MatchAndExplain(const std::wstring& s, MatchResultListener*) const {
        return s == expected_;
    }
    void DescribeTo(::std::ostream* os) const {
        *os << "equals wstring";
    }
    void DescribeNegationTo(::std::ostream* os) const {
        *os << "does not equal wstring";
    }
    using is_gtest_matcher = void;
private:
    std::wstring expected_;
};

}  // namespace internal

// Matcher<const std::wstring&> 특수화
template <>
class Matcher<const std::wstring&>
    : public internal::MatcherBase<const std::wstring&>
{
public:
    Matcher() {}

    explicit Matcher(const MatcherInterface<const std::wstring&>* impl)
        : internal::MatcherBase<const std::wstring&>(impl) {}

    template <typename M, typename = typename std::remove_reference<M>::type::is_gtest_matcher>
    Matcher(M&& m)  // NOLINT
        : internal::MatcherBase<const std::wstring&>(std::forward<M>(m)) {}

    // wstring 값으로 직접 초기화 (Eq 매처)
    Matcher(const std::wstring& s)  // NOLINT
        : internal::MatcherBase<const std::wstring&>(
            internal::WStringEqMatcher(s)) {}

    // wchar_t* 리터럴에서 초기화
    Matcher(const wchar_t* s)  // NOLINT
        : internal::MatcherBase<const std::wstring&>(
            internal::WStringEqMatcher(std::wstring(s))) {}
};

// Matcher<std::wstring> 특수화
template <>
class Matcher<std::wstring>
    : public internal::MatcherBase<std::wstring>
{
public:
    Matcher() {}

    explicit Matcher(const MatcherInterface<const std::wstring&>* impl)
        : internal::MatcherBase<std::wstring>(impl) {}
    explicit Matcher(const MatcherInterface<std::wstring>* impl)
        : internal::MatcherBase<std::wstring>(impl) {}

    template <typename M, typename = typename std::remove_reference<M>::type::is_gtest_matcher>
    Matcher(M&& m)  // NOLINT
        : internal::MatcherBase<std::wstring>(std::forward<M>(m)) {}

    Matcher(const std::wstring& s)  // NOLINT
        : internal::MatcherBase<std::wstring>(
            internal::WStringEqMatcher(s)) {}

    Matcher(const wchar_t* s)  // NOLINT
        : internal::MatcherBase<std::wstring>(
            internal::WStringEqMatcher(std::wstring(s))) {}
};

}  // namespace testing