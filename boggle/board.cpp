#include "board.h"



Board::Board()
{
	std::get<0>(RANGE_TO_NEIGHBORS) = -1;
	std::get<1>(RANGE_TO_NEIGHBORS) = 0;
	std::get<2>(RANGE_TO_NEIGHBORS) = 1;
}


Board::~Board()
{
	nodesMap.clear();
}

void Board::AddNode(const BoardID& id, const std::string & letter)
{
	if (IsALetter(letter))
	{
		BoardNodes node;
		std::locale loc;
		node.cLetter = std::tolower(letter[0], loc);
		node.neighborsList.clear();
		auto result = nodesMap.insert(std::make_pair(id, node));
		if (false == result.second)
		{
			std::cout << "insert value failed id first: " << id.first
			<< " ,second: " << id.second << " , letter: " << letter << std::endl;
		}

		AddNeighbors(id);
	}
	else
	{
		std::cout << "board can only have letters as nodes" << std::endl;
	}

}

void Board::FileToBoard(const std::string & boardFilePath)
{
	// board file configuration
	std::ifstream boardFile;
	boardFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
	boardFile.open(boardFilePath);
	if (!boardFile.is_open())
	{
		std::cout << "can not open boardFilePath: " << boardFilePath << std::endl;
		return;
	}
	boardFile.exceptions(std::ifstream::badbit);
	std::string oneLine;
	uint64_t i = 0;
	uint64_t j = 0;
	while (std::getline(boardFile, oneLine))
	{
		std::istringstream buf(oneLine);
		std::istream_iterator<std::string> beg(buf), end;
		std::vector<std::string> tokens(beg, end);  // done!
		for (auto& s: tokens )
		{
			if (IsALetter(s))
			{
				AddNode(BoardID(i,j), s);
			}
			else
			{
				std::cout << s << " is not a letter error! " << std::endl;
			}
			++j;
		}
		++i;
		j = 0;
	}

	boardFile.close();
}

void Board::AddNeighbors(const BoardID & id)
{
	auto owner = nodesMap.find(id);
	if (owner == nodesMap.end())
	{
		std::cout << "can not find this id, first: " << id.first
			<< " , second: " << id.second << std::endl;
		return;
	}

	for_each(RANGE_TO_NEIGHBORS, [&](const auto& i) 
	{
		int32_t r = i + id.first;
		for_each(RANGE_TO_NEIGHBORS, [&](const auto& j) 
		{
			int32_t c = j + id.second;
			if (BoardID(r, c) != id)
			{
				auto neighbor = nodesMap.find(BoardID(r, c));
				if (neighbor != nodesMap.end())
				{
					// todo... check if it still available
					owner->second.neighborsList[BoardID(r, c)] = 1;
					neighbor->second.neighborsList[id] = 1;
				}
			}
		});
	});
}

bool Board::IsALetter(const std::string & letter)
{
	if (letter.length() != 1)
	{
		std::cout << "letter length should be 1, letter is :" << letter << std::endl;
		return false;
	}

	return isalpha(letter.at(0));
}

