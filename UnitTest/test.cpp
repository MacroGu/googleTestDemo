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

   // for(auto& s: tokens)
	// TEST_COUT << "Hello world" << std::endl;

		/*
		P W Y R
		E N T H
		G S I Q
		O L S A
	*/



	std::vector<std::vector<std::string>> boardFile =
	{ {"P", "W", "Y", "R"}, {"E", "N", "T", "H"},
	{"G", "S", "I", "Q"},{"O", "L", "S", "A"} };

	Board board;
	board.FileToBoard("D:\\random\\board_1.txt");
	std::map<BoardID, BoardNodes> nodesMap;
	if (!board.GetNodes(nodesMap))
	{
		//EXPECT_TRUE(false);
	}

	int i = 0;
	int j = 0;
	for (auto nodesMapIter : nodesMap)
	{
		std::transform(boardFile[i][j].begin(), boardFile[i][j].end(), 
			boardFile[i][j].begin(), tolower);
		EXPECT_EQ(boardFile[i][j], nodesMapIter.second.cLetter);
		j++;
		if (j == 4)
		{
			j = 0;
			i++;
		}
		if (i == 4)
		{
		//	break;
		}
	}

}

