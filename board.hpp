#pragma once

#include <array>
#include <cstdint>
#include <ostream>
#include <vector>

namespace reversi
{

enum class Side : std::uint8_t
{
	BLACK,
	WHITE
};

Side getOpponentSide(Side side);

std::ostream& operator<<(std::ostream& os, Side side);

enum class CellState : std::uint8_t
{
	EMPTY,
	BLACK,
	WHITE
};

CellState getOwnState(Side side);

struct CellPosition
{
	int x;
	int y;
};

std::ostream& operator<<(std::ostream& os, CellPosition position);
CellPosition parseCellPosition(const std::string& str);

class Board
{
public:
	static constexpr int WIDTH = 8;
	static constexpr int HEIGHT = 8;

	Board();

	CellState get(CellPosition position) const;
	void set(CellPosition position, CellState state);

	bool isLegalMove(CellPosition position, Side side) const;
	//std::vector<CellPosition> getAllLegalMoves(Side side) const;
	std::vector<CellPosition> calculateAllLegalMoves(Side side) const;

	void placeDisk(CellPosition position, Side side);

	int count(CellState target) const;

	static bool boundsCheck(CellPosition position);

	void updateLegalMoves();

	std::vector<CellPosition> getAllLegalMoves(Side side) const;

	bool checkGameEnd() const;

	CellState getWinner() const;

	int getTurnNumber() const;

private:
	std::array<std::array<CellState, WIDTH>, HEIGHT> m_states;
	std::vector<CellPosition> black_Legal_moves;
	std::vector<CellPosition> white_legal_moves;
};

std::ostream& operator<<(std::ostream& os, const Board& board);

Board updateBoard(const Board& board, CellPosition position, Side side);

}  // namespace reversi
