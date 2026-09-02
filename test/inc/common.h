// Common for all tests
#ifndef TEST_INC_COMMON_H_
#define TEST_INC_COMMON_H_

#include <cmath>
#include <sstream>
#include <string>

#include <gtest/gtest.h>
#include "code/inc/easy_math.h"
#include "test/inc/signal_access_reporter.h"

const char *ERR_PREFIX = ">> ";     // For printed error descriptions
const char *NOTE_PREFIX = "  $$ ";  // For printed notes/comments

template <typename ActualType, typename ExpectedType>
bool IsEquivalentAssertion(const ActualType &actual,
                          const ExpectedType &expected) {
  return actual == expected;
}

template <>
bool IsEquivalentAssertion<float>(const float &actual, const float &expected) {
  const float diff = std::fabs(actual - expected);
  const float tolerance = 1.0e-6f;
  return diff <= tolerance;
}

template <>
bool IsEquivalentAssertion<double>(const double &actual, const double &expected) {
  const double diff = std::fabs(actual - expected);
  const double tolerance = 1.0e-12;
  return diff <= tolerance;
}

template <typename ActualType, typename ExpectedType>
std::string MakeAssertionSummary(const char *assertion_name,
                                const ActualType &actual,
                                const ExpectedType &expected) {
  std::ostringstream summary;
  summary << assertion_name << ": actual=" << actual << ", expected="
          << expected;
  return summary.str();
}

#define EXPECT_EQ_REPORT(actual, expected)                                  \
  do {                                                                      \
    const auto __actual = (actual);                                        \
    const auto __expected = (expected);                                    \
    const bool __passed = IsEquivalentAssertion(__actual, __expected);      \
    RecordAssertionResult(MakeAssertionSummary("EXPECT_EQ", __actual, __expected), __passed); \
    EXPECT_EQ(__actual, __expected);                                       \
  } while (0)

#define EXPECT_FLOAT_EQ_REPORT(actual, expected)                            \
  do {                                                                      \
    const auto __actual = (actual);                                        \
    const auto __expected = (expected);                                    \
    const bool __passed = IsEquivalentAssertion(__actual, __expected);      \
    RecordAssertionResult(MakeAssertionSummary("EXPECT_FLOAT_EQ", __actual, __expected), __passed); \
    EXPECT_FLOAT_EQ(__actual, __expected);                                 \
  } while (0)

#define EXPECT_TRUE_REPORT(actual)                                          \
  do {                                                                      \
    const bool __actual = !!(actual);                                      \
    RecordAssertionResult(MakeAssertionSummary("EXPECT_TRUE", __actual, true), __actual); \
    EXPECT_TRUE(__actual);                                                 \
  } while (0)

#define EXPECT_FALSE_REPORT(actual)                                         \
  do {                                                                      \
    const bool __actual = !!(actual);                                      \
    RecordAssertionResult(MakeAssertionSummary("EXPECT_FALSE", __actual, false), !__actual); \
    EXPECT_FALSE(__actual);                                                \
  } while (0)

#endif  // TEST_INC_COMMON_H_
