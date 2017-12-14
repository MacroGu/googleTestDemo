#include "board.h"



Board::Board()
{
	std::get<0>(NEIGHBORS_RANGE) = -1;
	std::get<1>(NEIGHBORS_RANGE) = 0;
	std::get<2>(NEIGHBORS_RANGE) = 1;
}


Board::~Board()
{
	nodesMap.clear();
}

void Board::AddNode(const BoardID& id, const std::string & letter)
{
	if (!&id)
	{
		std::cout << "error id is null " << std::endl;
		return;
	}

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
			return;
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
	while (tempWord.find("qu") != std::string::npos)
	{
		tempWord.replace(tempWord.find("qu"), strlen("qu"), "q");
	}

	for (auto nodesMapIter = nodesMap.begin(); nodesMapIter != nodesMap.end(); nodesMapIter++)
	{
		std::deque<BoardID> queue;
		std::list<BoardID> path, neighbors;
		auto TempNode = nodesMapIter->first;
		queue.push_back(TempNode);

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
				if (std::find(path.begin(), path.end(), id) != path.end()) continue;   // means this char in path

				path.push_back(id);
				if (tempWord.length() == path.size()) 
					return true;

				if (!GetNeighbors(id, neighbors))
				{
					std::cout << "GetNeighbors can not find this id, first: " << id.first << " second: "
						<< id.second << std::endl;
				}

				queue.insert(queue.begin(), neighbors.crbegin(), neighbors.crend());
			}
			else if (neighbors.size() > 0)
			{
				neighbors.pop_back();
			}

			if (neighbors.size() == 0 && path.size() > 0)
			{
				auto bad_id = path.back();
				path.pop_back();
				queue.clear();

				if (path.size() > 0)
				{
					if (!GetNeighbors(path.back(), neighbors))
					{
						std::cout << "get neighbors failed, first: " << path.back().first
							<< " second: " << path.back().second << std::endl;
					}
					queue.insert(queue.begin(), neighbors.crbegin(), neighbors.crend());
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
	if (!&id)
	{
		std::cout << "id is null " << std::endl;
		return false;
	}

	neighorsNode.clear();
	BoardNodes boardNode ;
	if (!GetNode(id, boardNode))
	{
		std::cout << "can not find this id, first : " << id.first
			<< " second : " << id.second << std::endl;
		return false;
	}

	std::for_each(boardNode.neighborsList.begin(), 
		boardNode.neighborsList.end(), 
		[&](const auto& oneNeighbor) { neighorsNode.push_back(oneNeighbor.first);});

	return true;
}

 bool Board::GetNode(const BoardID & id, BoardNodes& boardNodes)
{
	if (!&id)
	{
		std::cout << "id is null " << std::endl;
		return false;
	}

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
	if (!&id)
	{
		std::cout << "error id is null " << std::endl;
		return;
	}

	auto owner = nodesMap.find(id);
	if (owner == nodesMap.end())
	{
		std::cout << "can not find this id, first: " << id.first
			<< " , second: " << id.second << std::endl;
		return;
	}

	for_each(NEIGHBORS_RANGE, [&](const auto& i) 
	{
		int32_t r = i + id.first;
		for_each(NEIGHBORS_RANGE, [&](const auto& j) 
		{
			int32_t c = j + id.second;
			if (BoardID(r, c) != id)
			{
				auto neighbor = nodesMap.find(BoardID(r, c));
				if (neighbor != nodesMap.end())
				{
					// merge repeat, if not exist, create it
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

