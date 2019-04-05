#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "mini_max_player.hpp"

namespace reversi
{

MiniMaxPlayer::MiniMaxPlayer(Side side) : Player(side){
    initializeWeightedSquare();
}

CellPosition MiniMaxPlayer::thinkNextMove(const Board &board)
{

    setNodeCount(0);
   CellPosition best_move;
   int alpha = -(int)INFINITY;
   int beta = (int)INFINITY;

   int score = maxNode(board,1,max_depth,best_move,alpha,beta);
   std::cout << "best score : " << score << " , nodes explored : " << getNodeCount() <<  "\n";
   return best_move;
}

/*
//ディスク数で評価する評価関数
int MiniMaxPlayer::evaluateFunction(const Board board)
{
    return board.count(getOwnState(getSide()));
}
*/

//weighted squaresを用いた評価関数
int MiniMaxPlayer::evaluateFunction(const Board board)
{
    int count = board.count(getOwnState(getSide())) + board.count(getOwnState(getOpponentSide(getSide())));
    int score;
    if(count < end_game_threshold){
        score = calculateWeightSquare(board);
    }else if(count >= end_game_threshold){
        score = calculateSimplecount(board);
    }
    return score;
}
/*
//枝刈りなしのMax
int MiniMaxPlayer::maxNode(Board board, int depth, int max_depth, CellPosition &best_move)
{
    addNodeCount();
    //std::cout << "Enter max Node, depth = " << depth << "\n";

    if (depth == max_depth){
        return evaluateFunction(board);
    }   

    int best_score = -(int)INFINITY;
    std::vector<CellPosition> legal_moves = board.getAllLegalMoves(getSide());

    if(legal_moves.empty()){
        return evaluateFunction(board);
    }

    std::vector<int> scores;
    for(CellPosition move : legal_moves){
        Board new_board = updateBoard(board,move,getSide());
        int v = minNode(new_board,depth + 1,max_depth,best_move);
        scores.push_back(v);
    }

    best_move = legal_moves[std::distance(scores.begin(), std::max_element(scores.begin(), scores.end()))];
    best_score = scores[std::distance(scores.begin(), std::max_element(scores.begin(), scores.end()))];
    
    //std::cout << "in max Node, depth = " << depth;
    //std::cout << ", best score = " << best_score << ", best move = " << best_move << "\n";

    return best_score;
    
}

//枝刈りなしのMini
int MiniMaxPlayer::minNode(Board board, int depth, int max_depth, CellPosition &best_move)
{
    addNodeCount();
    //std::cout << "Enter min Node, depth = " << depth << "\n";

    if(depth == max_depth){
        return evaluateFunction(board);
    }

    int worst_score = (int)INFINITY;
    std::vector<CellPosition> legal_moves=board.getAllLegalMoves(getOpponentSide(getSide()));

    if(legal_moves.empty()){
        return evaluateFunction(board);
    }

    std::vector<int> scores;
    for(CellPosition move : legal_moves){
        Board new_board = updateBoard(board, move, getOpponentSide(getSide()));
        int v = maxNode(new_board,depth+1,max_depth,best_move);
        scores.push_back(v);
    }

    worst_score = scores[std::distance(scores.begin(), std::min_element(scores.begin(), scores.end()))];

    //std::cout << "in max Node, depth = " << depth;
    //std::cout << ", worst score = " << worst_score << "\n";

    return worst_score;
}
*/

//枝刈りありのMax
int MiniMaxPlayer::maxNode(Board board, int depth, int max_depth, CellPosition &best_move,int alpha, int beta)
{
    addNodeCount();
    //std::cout << "Enter max Node, depth = " << depth << "\n";

    if (depth == max_depth){
        return evaluateFunction(board);
    }   

    int best_score = -(int)INFINITY;
    std::vector<CellPosition> legal_moves = board.getAllLegalMoves(getSide());

    if(legal_moves.empty()){
        return evaluateFunction(board);
    }

    std::vector<int> scores;
    for(CellPosition move : legal_moves){
        Board new_board = updateBoard(board,move,getSide());
        int v = minNode(new_board,depth + 1,max_depth,best_move,alpha,beta);
        scores.push_back(v);

        if(v > alpha){
            alpha = v;
            if(alpha >= beta){
                break;
            }
        }
    }

    best_move = legal_moves[std::distance(scores.begin(), std::max_element(scores.begin(), scores.end()))];
    //best_score = scores[std::distance(scores.begin(), std::max_element(scores.begin(), scores.end()))];
    
    //std::cout << "in max Node, depth = " << depth;
    //std::cout << ", best score = " << best_score << ", best move = " << best_move << "\n";

    //return best_score;
    return alpha;
    
}

//枝刈りありのMini
int MiniMaxPlayer::minNode(Board board, int depth, int max_depth, CellPosition &best_move,int alpha, int beta)
{
    addNodeCount();
    //std::cout << "Enter min Node, depth = " << depth << "\n";

    if(depth == max_depth){
        return evaluateFunction(board);
    }

    int worst_score = (int)INFINITY;
    std::vector<CellPosition> legal_moves=board.getAllLegalMoves(getOpponentSide(getSide()));

    if(legal_moves.empty()){
        return evaluateFunction(board);
    }

    std::vector<int> scores;
    for(CellPosition move : legal_moves){
        Board new_board = updateBoard(board, move, getOpponentSide(getSide()));
        int v = maxNode(new_board,depth+1,max_depth,best_move,alpha,beta);
        scores.push_back(v);

        if(v < beta){
            beta = v;
            if(alpha > beta){
                break;
            }
        }
    }

    //worst_score = scores[std::distance(scores.begin(), std::min_element(scores.begin(), scores.end()))];

    //std::cout << "in max Node, depth = " << depth;
    //std::cout << ", worst score = " << worst_score << "\n";

    //return worst_score;
    return beta;
}

void MiniMaxPlayer::setNodeCount(int count){
    node_count = count;
}

int MiniMaxPlayer::getNodeCount(){
    return node_count;
}

void MiniMaxPlayer::addNodeCount(){
    node_count ++;
}

void MiniMaxPlayer::initializeWeightedSquare(){

    std::cout << "initializing weighted squared\n";

    weighted_squares[0] = {120, -20, 20, 5, 5, 20, -20, 120};
    weighted_squares[1] = {-20, -40, -5, -5, -5, -5, -40, -20};
    weighted_squares[2] = {20, -5, 15, 3, 3, 15, -5, 20};
    weighted_squares[3] = {5, -5, 3, 3, 3, 3, -5, 5};
    weighted_squares[4] = {5, -5, 3, 3, 3, 3, -5, 5};
    weighted_squares[5] = {20, -5, 15, 3, 3, 15, -5, 20};
    weighted_squares[6] = {-20, -40, -5, -5, -5, -5, -40, -20};
    weighted_squares[7] = {120, -20, 20, 5, 5, 20, -20, 120};
}

int MiniMaxPlayer::calculateWeightSquare(const Board board)
{
    int score = 0;
    for(int i = 0 ; i < Board::WIDTH; i++){//i -> a-h
        for(int j = 0; j<Board::HEIGHT; j++){// j -> 1-8
            CellPosition position = {i,j};
            if(board.get(position) == getOwnState(getSide())){
                score += weighted_squares[j][i];
            }
        }
    }
    return score;
}

int MiniMaxPlayer::calculateSimplecount(const Board board){
     return board.count(getOwnState(getSide()));
}

Board updateBoard(const Board &board, CellPosition position, Side side)
{
    Board new_board = board;
    new_board.placeDisk(position, side);
    return new_board;
}

} // namespace reversi
