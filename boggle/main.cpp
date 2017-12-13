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

	// dictionary file
	std::ifstream dictionaryFile;
	dictionaryFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
	dictionaryFile.open(dictionaryFilePath);
	if (!dictionaryFile.is_open()) 
	{
		std::cout << "dictionary path: " << dictionaryFilePath << " boardFilePath: "
			<< boardFilePath << " boardOutputFilePath: " << boardOuputFilePath << std::endl;
		return -1;
	}
	dictionaryFile.exceptions(std::ifstream::badbit);
	std::string oneLine;
	uint64_t dictionaryLines = 0;
	while (std::getline(dictionaryFile, oneLine)) 
	{
		dictionaryLines++;
	}
	std::cout << dictionaryLines << std::endl;
	dictionaryFile.close();

	

	return 0;
}