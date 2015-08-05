#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <xll/expected.hpp>

using namespace xll;

TEST_CASE("expected with value")
{
  {
    expected<double, int> exp = 1.0;
    REQUIRE(exp);
    CHECK(1.0 == *exp);
  }

  {
    expected<long, int> exp = static_cast<char>(1);
    REQUIRE(exp);
    CHECK(1 == *exp);
  }
}

TEST_CASE("expected with value copy/move")
{
  expected<double, int> exp = 1.0;

  SECTION("copy construction")
  {
    expected<double, int> exp2(exp);
    REQUIRE(exp2);
    CHECK(1.0 == *exp2);
  }

  SECTION("assign construction")
  {
    expected<double, int> exp2 = exp;
    REQUIRE(exp2);
    CHECK(1.0 == *exp2);
  }

  SECTION("assignation")
  {
    expected<double, int> exp2(1.1);
    exp2 = exp;
    REQUIRE(exp2);
    CHECK(1.0 == *exp2);
  }
}

TEST_CASE("expected with error")
{
  {
    expected<float, int> exp = make_unexpected(10);
    REQUIRE_FALSE(exp);
    CHECK(10 == exp.error());
  }

  {
    expected<float, long> exp2 = make_unexpected(static_cast<char>(15));
    REQUIRE_FALSE(exp2);
    CHECK(15 == exp2.error());
  }

  {
    expected<float, long> exp(unexpect, 16);
    REQUIRE_FALSE(exp);
    CHECK(16 == exp.error());
  }
}

TEST_CASE("expected with error copy/move")
{
  expected<double, int> exp(unexpect, 16);

  SECTION("copy construction")
  {
    expected<double, int> exp2(exp);
    REQUIRE_FALSE(exp2);
    CHECK(16 == exp2.error());
  }

  SECTION("assign construction")
  {
    expected<double, int> exp2 = exp;
    REQUIRE_FALSE(exp2);
    CHECK(16 == exp2.error());
  }

  SECTION("assignation")
  {
    expected<double, int> exp2(1.1);
    exp2 = exp;
    REQUIRE_FALSE(exp2);
    CHECK(16 == exp2.error());
  }
}
