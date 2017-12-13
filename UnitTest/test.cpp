#include "pch.h"
#include "board.h"


TEST(BoogleUnitTest, TestIsALetter) 
{
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


// test pair equal
TEST(BoogleUnitTest, TestPairEqual)
{
	BoardID id1(1,2);
	BoardID id2(1,2);
	EXPECT_EQ(id1, id2);
}

// tuple in board, testing
TEST(BoogleUnitTest, TestTupleInBoard)
{
	std::tuple<uint32_t, uint32_t, uint32_t> t1(1,2,3);
	EXPECT_EQ(1, std::get<0>(t1));
	EXPECT_EQ(2, std::get<1>(t1));
	EXPECT_EQ(3, std::get<2>(t1));

	std::tuple<uint32_t, uint32_t, uint32_t> t2(1,2,3);
	EXPECT_EQ(t1, t2);

	Board b1;
	EXPECT_EQ(3, b1.RANGE_TO_NEIGHBORS._Mysize);
}


TEST(BoogleUnitTest, TestFileToBoard)
{
	std::string str = "This is a string";
    std::istringstream buf(str);
    std::istream_iterator<std::string> beg(buf), end;
	std::vector<std::string> tokens(beg, end); // done!

    for(auto& s: tokens)
        TEST_COUT << s << std::endl;
// TEST_COUT << "Hello world" << std::endl;

}
