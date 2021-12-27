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

TEST(HelperTests, IsInsideBoard) {
  auto isInside = isInsideBoard(5, 5);
  auto isOutside1 = isInsideBoard(-1, 1);
  auto isOutside2 = isInsideBoard(1, -1);
  auto isOutside3 = isInsideBoard(8, 1);
  auto isOutside4 = isInsideBoard(1, 8);

  EXPECT_TRUE(isInside);
  EXPECT_FALSE(isOutside1);
  EXPECT_FALSE(isOutside1);
  EXPECT_FALSE(isOutside2);
  EXPECT_FALSE(isOutside3);
  EXPECT_FALSE(isOutside4);
}
