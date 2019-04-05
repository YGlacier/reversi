#pragma once

#include <vector>
#include <array>

#include "board.hpp"
#include "player.hpp"

namespace reversi
{

class MiniMaxPlayer : public Player
{
public:
	MiniMaxPlayer(Side side);

	virtual CellPosition thinkNextMove(const Board& board) override;

	int evaluateFunction(const Board board);

/*
//枝刈りなし
    int maxNode(Board board,int depth,int max_depth,CellPosition& best_move);

    int minNode(Board board,int depth,int max_depth,CellPosition& best_move);
*/

    int maxNode(Board board,int depth,int max_depth,CellPosition& best_move,int alpha, int beta);

    int minNode(Board board,int depth,int max_depth,CellPosition& best_move,int alpha, int beta);

	void setNodeCount(int count);

	int getNodeCount();

	void addNodeCount();

	void initializeWeightedSquare();

	int calculateWeightSquare(const Board board);

	int calculateSimplecount(const Board board);

private:
	int max_depth = 6;
	int end_game_threshold = 57;
	int node_count;
	std::array<std::array<int,Board::WIDTH>,Board::HEIGHT> weighted_squares;

	

};


Board updateBoard(const Board& board, CellPosition position, Side side);

}  // namespace reversi

