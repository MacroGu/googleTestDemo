#include "board.h"


int main(int argc, char** argv)
{

	if (argc <= 3)
	{
		return -1;
	}

	// get file configuration
	std::string dictionaryFilePath = argv[1];
	std::string boardFilePath = argv[2];
	std::string boardOuputFilePath = argv[3];


	Board board;
	board.FileToBoard(boardFilePath);
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			std::cout << "(" << i << ", " << j << ") [";
			std::list<BoardID> neighbors;
			if (! board.GetNeighbors(BoardID(i,j), neighbors))
			{
				std::cout << "error " << i << ", " << j << std::endl;
				return -1;
			}
			for (auto oneNeighbor : neighbors)
			{
				std::cout << "(" << oneNeighbor.first << ", " << oneNeighbor.second
					<< "),";
			}
			std::cout << "]" << std::endl;
		}
	}


	// write the resul to file
	std::ofstream boardOuputFile; 
	boardOuputFile.open(boardOuputFilePath, std::ofstream::out | std::ofstream::app);
	if (!boardOuputFile.is_open())
	{
		std::cout << "can not open boardOutputFile, path: " << boardOuputFilePath << std::endl;
		return -1;		
	}

	// dictionary file
	std::ifstream dictionaryFile;
	dictionaryFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
	dictionaryFile.open(dictionaryFilePath);
	if (!dictionaryFile.is_open()) 
	{
		std::cout << "can not open dictionaryFile, path: " << dictionaryFilePath << std::endl;
		return -1;
	}
	dictionaryFile.exceptions(std::ifstream::badbit);
	std::string oneLine;
	while (std::getline(dictionaryFile, oneLine)) 
	{
		if (oneLine.length() < 3) continue;

		std::transform(oneLine.begin(), oneLine.end(), oneLine.begin(), tolower);
		if (board.CheckForWord(oneLine))
		{
			boardOuputFile << oneLine << std::endl;
		}
	}

	boardOuputFile.close();
	dictionaryFile.close();


	return 0;
}