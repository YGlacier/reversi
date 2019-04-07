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

	float evaluateFunction(const Board board);

/*
//枝刈りなし
    int maxNode(Board board,int depth,int max_depth,CellPosition& best_move);

    int minNode(Board board,int depth,int max_depth,CellPosition& best_move);
*/

    float maxNode(Board board,int depth,int max_depth,CellPosition& best_move,float alpha, float beta);

    float minNode(Board board,int depth,int max_depth,CellPosition& best_move,float alpha, float beta);

	void setNodeCount(int count);

	int getNodeCount();

	void addNodeCount();

	void initializeWeightedSquare();

	void initializeWeights();

	void initializeGamePhaseBoarder();

	float calculateWeightSquare(const Board board);

	float calculateSimplecount(const Board board);

	float calculateSelfMobility(const Board board);

	float calculateOpponentMobility(const Board board);

	float calculateMobility(const Board board);

	void setTurnNumber(int turn);

	int getTurnNumber();

	int getGamePhase(int turn);

	bool endGameSearch(const Board board,Side side);

private:
	int max_depth = 6;
	static const int end_game_threshold = 57;
	int node_count;
	static const int game_phase = 3;
	int turn_number;
	std::array<int,game_phase + 1> game_phase_boarder;
	std::array<std::array<float,Board::WIDTH>,Board::HEIGHT> weighted_squares;
	std::array<std::array<float,3>,game_phase> weights;

	

};


Board updateBoard(const Board& board, CellPosition position, Side side);

}  // namespace reversi

