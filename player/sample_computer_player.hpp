#pragma once

#include "board.hpp"
#include "player.hpp"

namespace reversi
{

class SampleComputerPlayer : public Player
{
public:
	SampleComputerPlayer(Side side);

	virtual CellPosition thinkNextMove(const Board& board) override;
};

}  // namespace reversi
