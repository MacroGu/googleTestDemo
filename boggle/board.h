#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <queue>
#include <tuple>
#include <algorithm>     // transform
#include <set>
#include <type_traits>
#include <sstream>
#include <iterator>
#include <stdint.h>


#include "gtest/gtest_prod.h"

typedef std::pair<uint32_t, uint32_t> BoardID;

typedef struct stNode
{
	std::string cLetter;   // actually it has one char
	std::map<BoardID, int32_t> neighborsList;

	stNode()
	{
		cLetter = "";
		neighborsList.clear();
	}
} BoardNodes;

template<typename... Ts, typename FunctionType>
void for_each(std::tuple<Ts...> &, FunctionType, std::integral_constant<size_t, sizeof...(Ts)>) {}

template<std::size_t I, typename... Ts, typename FunctionType, typename = typename std::enable_if<I!=sizeof ...(Ts)>::type >
void for_each(std::tuple<Ts...>& t, FunctionType f, std::integral_constant<size_t, I>)
{
	f(std::get<I>(t));
	for_each(t, f, std::integral_constant<size_t, I + 1>());
}

template<typename... Ts, typename FunctionType>
void for_each(std::tuple<Ts...>& t, FunctionType f)
{
	for_each(t, f, std::integral_constant<size_t, 0>());
}

class Board
{
public:
	Board();
	~Board();

	void AddNode(const BoardID& id, const std::string& letter);
	// change to matrix
	void FileToBoard(const std::string& boardFilePath);
	bool CheckForWord(const std::string& word);

	// just get the key of the neighbors 
	bool GetNeighbors(const BoardID& id, std::list<BoardID>& neighorsNode);
	bool GetNode(const BoardID& id, BoardNodes& boardNodes);
	bool GetNodes(std::map<BoardID, BoardNodes>& nodesMap);
private:
	
	FRIEND_TEST(BoogleUnitTest, TestAddNeighbors);
	void AddNeighbors(const BoardID& id);

	FRIEND_TEST(BoogleUnitTest, TestIsALetter);
	bool IsALetter(const std::string& letter);

private:
	// key: BoardID, value: BoardNodes
	std::map<BoardID, BoardNodes> nodesMap;
	// to get neighbors
	FRIEND_TEST(BoogleUnitTest, TestTupleInBoard);
	std::tuple<int32_t, int32_t, int32_t> NEIGHBORS_RANGE;
};
