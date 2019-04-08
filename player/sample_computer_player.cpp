#include <vector>
#include <ctime>
#include <cstdlib>
#include <iostream>

#include "sample_computer_player.hpp"

namespace reversi
{

SampleComputerPlayer::SampleComputerPlayer(Side side) : Player(side){
	srand((unsigned int)time(NULL));
}

CellPosition SampleComputerPlayer::thinkNextMove(const Board& board)
{
	// choose the first legal move
	std::vector<CellPosition> legal_moves = board.getAllLegalMoves(getSide());
	int index = std::rand() % (int)legal_moves.size();
	std::cout << "Sample index : " << index << "\n";
	return legal_moves[index];
}

}  // namespace reversi
