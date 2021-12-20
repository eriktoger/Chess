#include "../chess/helpers.h"
#include <gtest/gtest.h>

TEST(HelperTests, calculateSquareColor) {
  auto color1 = calcSquareColor(0, 0);
  auto color2 = calcSquareColor(0, 1);
  EXPECT_EQ(color1, WHITE);
  EXPECT_EQ(color2, BLACK);
}

TEST(HelperTests, SanitizeBoardLength) {
  auto value1 = sanitizeBoardLength(-1);
  auto value2 = sanitizeBoardLength(8);
  auto value3 = sanitizeBoardLength(5);
  EXPECT_EQ(value1, 0);
  EXPECT_EQ(value2, 7);
  EXPECT_EQ(value3, 5);
}
