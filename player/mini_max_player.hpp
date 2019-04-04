#pragma once

#include "board.hpp"
#include "player.hpp"

namespace reversi
{

class MiniMaxPlayer : public Player
{
public:
	MiniMaxPlayer(Side side) : Player(side) {}

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

private:
	int max_depth = 5;
	int node_count;

};


Board updateBoard(const Board& board, CellPosition position, Side side);

}  // namespace reversi

