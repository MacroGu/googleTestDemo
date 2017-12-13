#include "pch.h"
#include "board.h"


TEST(BoogleUnitTest, TestIsALetter) {
  Board board;
  EXPECT_TRUE(!board.IsALetter("1"));
  EXPECT_TRUE(board.IsALetter("a"));
  EXPECT_TRUE(board.IsALetter("A"));
  EXPECT_TRUE(board.IsALetter("z"));
  EXPECT_TRUE(board.IsALetter("Z"));
  EXPECT_TRUE(!board.IsALetter("."));
  EXPECT_TRUE(!board.IsALetter("!"));
  EXPECT_TRUE(!board.IsALetter("`"));
}