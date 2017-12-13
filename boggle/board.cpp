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
		std::string cLetter = letter;
		std::transform(cLetter.begin(), cLetter.end(), cLetter.begin(), tolower);
		node.cLetter = cLetter;
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

bool Board::CheckForWord(const std::string & word)
{
	std::string tempWord = word;
	tempWord.replace(tempWord.find("qu"), tempWord.length(), "q");

	for (auto nodesMapIter = nodesMap.begin(); nodesMapIter != nodesMap.end(); nodesMapIter++)
	{
		auto TempNode = nodesMapIter->first;
		std::deque<BoardID> queue;
		queue.push_back(TempNode);
		std::list<BoardID> path, neighbors;

		while (queue.size() > 0)
		{
			auto id = queue.front();
			queue.pop_front();
			BoardNodes node;
			if (!GetNode(id, node))
			{
				std::cout << "can not find this id, first: " << id.first << " second: "
					<< id.second << std::endl;
				break;
			}

			if (node.cLetter.at(0) == tempWord.at(path.size()))  // cLetter just one char
			{
				auto pathIter = std::find(path.begin(), path.end(), id);
				if (pathIter == path.end()) continue;

				path.push_back(id);
				if (tempWord.length() == path.size()) return true;

				if (!GetNeighbors(id, neighbors))
				{
					std::cout << "GetNeighbors can not find this id, first: " << id.first << " second: "
						<< id.second << std::endl;
				}

				path.insert(path.end(), neighbors.begin(), neighbors.end());
			}
			else if (neighbors.size() > 0)
			{
				neighbors.pop_front();
			}

			if (neighbors.size() == 0 && path.size() > 0)
			{
				auto bad_id = path.front();
				path.pop_front();
				queue.clear();

				if (!GetNeighbors(path.back(), neighbors))
				{
					std::cout << "get neighbors failed, first: " << path.back().first
						<< " second: " << path.back().second << std::endl;
				}
				for (auto oneNeighbor : neighbors)
				{
					queue.push_back(oneNeighbor);
					auto delID = std::find(queue.begin(), queue.end(), bad_id);
					if (delID != queue.end())
					{
						queue.erase(delID);
					}
				}
			}
		}
	}
	return false;
}

bool Board::GetNeighbors(const BoardID& id, std::list<BoardID>& neighorsNode)
{
	BoardNodes boardNode ;
	if (!GetNode(id, boardNode))
	{
		std::cout << "can not find this id, first : " << id.first
			<< " second : " << id.second << std::endl;
		return false;
	}

	for (auto oneNeighbor : boardNode.neighborsList)
	{
		neighorsNode.push_back(oneNeighbor.first);
	}

	return true;
}

 bool Board::GetNode(const BoardID & id, BoardNodes& boardNodes)
{
	auto boardNode = nodesMap.find(id);
	if (boardNode != nodesMap.end())
	{
		boardNodes = boardNode->second;
		return true;
	}
	else
	{
		std::cout << "can not find this id, first : " << id.first
			<< " second : " << id.second << std::endl;
		return false;
	}
}

bool Board::GetNodes(std::map<BoardID, BoardNodes>& nodesMap)
{
	nodesMap = this->nodesMap;
	return true;
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

