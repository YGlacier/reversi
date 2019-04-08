#pragma once

#include <vector>
#include <array>
#include <chrono>

#include "board.hpp"
#include "player.hpp"

namespace reversi
{

//weighted squaresだけを考える
class MiniMaxPlayer1 : public Player
{
public:
	MiniMaxPlayer1(Side side);

	virtual CellPosition thinkNextMove(const Board& board) override;

	float evaluateFunction(const Board board);

/*
//枝刈りなし
    int maxNode(Board board,int depth,int max_depth,CellPosition& best_move);

    int minNode(Board board,int depth,int max_depth,CellPosition& best_move);
*/

    float maxNode(Board board,int depth,int max_depth,CellPosition& best_move,float alpha, float beta);

	float shallowMaxNode(Board board,int depth,int max_depth,CellPosition& best_move,float alpha, float beta);

    float minNode(Board board,int depth,int max_depth,CellPosition& best_move,float alpha, float beta);

	float shallowMinNode(Board board,int depth,int max_depth,CellPosition& best_move,float alpha, float beta);

	void setNodeCount(int count);

	int getNodeCount();

	void addNodeCount();

	void initializeWeightedSquare();

	void initializeWeights();

	void initializeGamePhaseBoarder();

	float calculateWeightSquare(const Board board);

	float calculateSimplecount(const Board board);

	//float calculateMobility(std::vector<CellPosition>& opponent_legal_moves, std::vector<CellPosition>& legal_moves);

	float calculateMobility(const Board board);

	void setTurnNumber(int turn);

	int getTurnNumber();

	int getGamePhase(int turn);

	bool endGameSearchNode(const Board board,Side side);

	bool endGameSearch(const Board board,CellPosition& best_move);

	int getSearchTimeMiniSec();

	void setStartTime();

	void setEndSearchNodeCount(int count);

	int getEndSearchNodeCount();

	void addEndSearchNodeCount();

	void setMaxDepth();

private:
	int max_depth = 6;
	static const int end_game_threshold = 57;
	int node_count;
	static const int game_phase = 5;
	static const int end_game_search_start_turn = 51;
	static const int end_game_search_max_root_node = 64;
	int turn_number;
	std::array<int,game_phase + 1> game_phase_boarder;
	std::array<std::array<float,Board::WIDTH>,Board::HEIGHT> weighted_squares;
	std::array<std::array<float,3>,game_phase> weights;
	std::chrono::system_clock::time_point start_time;
	int end_search_node_count;
	int max_time = 3000;
	int shallow_search_depth = 3;

	

};
}  // namespace reversi

