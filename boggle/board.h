#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <stdint.h>


#include "gtest/gtest_prod.h"


class Board
{
public:
	Board();
	~Board();



private:
	FRIEND_TEST(BoogleUnitTest, TestIsALetter);
	bool IsALetter(const std::string& letter);
};

