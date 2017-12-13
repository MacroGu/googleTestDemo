#include "board.h"



Board::Board()
{
}


Board::~Board()
{
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
