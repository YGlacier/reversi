#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <chrono>
#include <numeric>

#include "mini_max_player_3.hpp"

namespace reversi
{

MiniMaxPlayer3::MiniMaxPlayer3(Side side) : Player(side){
    initializeWeightedSquare();
    initializeGamePhaseBoarder();
    initializeWeights();
}

CellPosition MiniMaxPlayer3::thinkNextMove(const Board &board)
{
    setStartTime();
    setNodeCount(0);
    setTurnNumber(board.count(getOwnState(getSide())) + board.count(getOwnState(getOpponentSide(getSide()))));
    setMaxDepth();

    std::cout << "Turn : " << getTurnNumber() << "\n";

    CellPosition best_move;
    float alpha = -(float)INFINITY;
    float beta = (float)INFINITY;

    int root_legal_moves_count = (int)board.getAllLegalMoves(getSide()).size();

    //確勝ルートがあるかどうかを探す
    setEndSearchNodeCount(0);
    if(getTurnNumber() > end_game_search_start_turn && root_legal_moves_count <= end_game_search_max_root_node){
        std::cout << "Root legal moves count : " << root_legal_moves_count << "\n";
        auto end_game_search_start = std::chrono::system_clock::now();
        if(endGameSearch(board,best_move)){
            auto t_end = std::chrono::system_clock::now();
            std::cout << "Time for end-game-search : " << std::chrono::duration_cast<std::chrono::milliseconds>(t_end - end_game_search_start).count() << " ms\n";
            std::cout << "End-game-search Node : " << getEndSearchNodeCount() << "\n";
            std::cout << "Time : " << getSearchTimeMiniSec() << " ms\n";
            return best_move;
        }
        auto t_end = std::chrono::system_clock::now();
        std::cout << "Time for end-game-search : " << std::chrono::duration_cast<std::chrono::milliseconds>(t_end - end_game_search_start).count() << " ms\n";
        std::cout << "End-game-search Node : " << getEndSearchNodeCount() << "\n";
    }

    float score = maxNode(board,1,max_depth,best_move,alpha,beta);
    std::cout << "best score : " << score << " , nodes explored : " << getNodeCount() <<  "\n";
    std::cout << "Time : " << getSearchTimeMiniSec() << " ms\n";
    return best_move;
}

/*
//ディスク数で評価する評価関数
int MiniMaxPlayer3::evaluateFunction(const Board board)
{
    return board.count(getOwnState(getSide()));
}
*/

//weighted squaresを用いた評価関数
float MiniMaxPlayer3::evaluateFunction(const Board board)
{
    float score;
    score = calculateWeightSquare(board) * weights[getGamePhase(board.getTurnNumber())][0];
    score += calculateMobility(board) * weights[getGamePhase(board.getTurnNumber())][1];
    score += calculateSimplecount(board) * weights[getGamePhase(board.getTurnNumber())][2];
    return score;
}

//枝刈りありのMax
float MiniMaxPlayer3::maxNode(Board board, int depth, int max_depth, CellPosition &best_move,float alpha, float beta)
{
    addNodeCount();

    /*
    if(getSearchTimeMiniSec() > max_time){
        return alpha;
    }
    */

    std::vector<CellPosition> legal_moves = board.getAllLegalMoves(getSide());
    std::vector<CellPosition> opponent_legal_moves=board.getAllLegalMoves(getOpponentSide(getSide()));

    if (depth == max_depth){
        return evaluateFunction(board);
    }

    if(legal_moves.empty()){
        return evaluateFunction(board);
    }

    std::vector<float> scores;

    //浅く探索
    if(depth == 1){
        std::cout << "Shallow search\n";
        std::vector<float> shallow_search_scores;
        for(CellPosition move : legal_moves){
            if(getSearchTimeMiniSec() > max_time){
                break;
            }
            Board new_board = updateBoard(board,move,getSide());
            float v = shallowMinNode(new_board,1,shallow_search_depth,best_move,alpha,beta);

            shallow_search_scores.push_back(v);
        }

        std::vector<size_t> indices(shallow_search_scores.size());
        std::iota(indices.begin(), indices.end(), 0);

        std::sort(indices.begin(), indices.end(), [&shallow_search_scores](size_t i1, size_t i2) {
            return shallow_search_scores[i1] > shallow_search_scores[i2];
        });
        std::vector<CellPosition> new_legal_moves_array;
        for(size_t index : indices){
            new_legal_moves_array.push_back(legal_moves[index]);
            std::cout << legal_moves[index] << " : " << shallow_search_scores[index] << " ";
        }
        std::cout << "\n";
        legal_moves = new_legal_moves_array;
    }

    for(CellPosition move : legal_moves){
        if(getSearchTimeMiniSec() > max_time && depth == 1){
            break;
        }
        Board new_board = updateBoard(board,move,getSide());
        float v = minNode(new_board,depth + 1,max_depth,best_move,alpha,beta);

        scores.push_back(v);

        if(v > alpha){
            alpha = v;
            if(alpha >= beta) {
                break;
            }
        }
    }

    if(depth == 1){
        for(int i = 0 ; i < (int)scores.size() ; i++){
            std::cout << legal_moves[i] << " : " << scores[i] << " ";
        }
        std::cout << "\n";
    }

    best_move = legal_moves[std::distance(scores.begin(), std::max_element(scores.begin(), scores.end()))];

    return alpha;
    
}

float MiniMaxPlayer3::shallowMaxNode(Board board, int depth, int max_depth, CellPosition &best_move,float alpha, float beta)
{
    std::vector<CellPosition> legal_moves = board.getAllLegalMoves(getSide());
    std::vector<CellPosition> opponent_legal_moves=board.getAllLegalMoves(getOpponentSide(getSide()));

    if (depth == max_depth){
        return evaluateFunction(board);
    }

    if(legal_moves.empty()){
        return evaluateFunction(board);
    }

    std::vector<float> scores;

    for(CellPosition move : legal_moves){
        Board new_board = updateBoard(board,move,getSide());
        float v = shallowMinNode(new_board,depth + 1,max_depth,best_move,alpha,beta);

        scores.push_back(v);

        if(v > alpha){
            alpha = v;
            if(alpha >= beta){
                break;
            }
        }
    }

    best_move = legal_moves[std::distance(scores.begin(), std::max_element(scores.begin(), scores.end()))];

    return alpha;
    
}

//枝刈りありのMini
float MiniMaxPlayer3::minNode(Board board, int depth, int max_depth, CellPosition &best_move,float alpha, float beta)
{
    /*
    if(getSearchTimeMiniSec() > max_time){
        return beta;
    }
    */
    addNodeCount();

    std::vector<CellPosition> opponent_legal_moves=board.getAllLegalMoves(getOpponentSide(getSide()));
    std::vector<CellPosition> legal_moves = board.getAllLegalMoves(getSide());

    if(depth == max_depth){
        return evaluateFunction(board);
    }

    if(opponent_legal_moves.empty()){
        return evaluateFunction(board);
    }

    std::vector<float> scores;
    std::vector<float> shallow_search_scores;

    for(CellPosition move : opponent_legal_moves){
        /*
        if(getSearchTimeMiniSec() > max_time){
            break;
        }
        */
        Board new_board = updateBoard(board, move, getOpponentSide(getSide()));
        float v = maxNode(new_board,depth+1,max_depth,best_move,alpha,beta);

        scores.push_back(v);

        if(v < beta){
            beta = v;
            if(alpha >= beta){
                break;
            }
        }
    }

    return beta;
}

float MiniMaxPlayer3::shallowMinNode(Board board, int depth, int max_depth, CellPosition &best_move,float alpha, float beta)
{
    if(getSearchTimeMiniSec() > max_time){
        return beta;
    }
    addNodeCount();

    std::vector<CellPosition> opponent_legal_moves=board.getAllLegalMoves(getOpponentSide(getSide()));
    std::vector<CellPosition> legal_moves = board.getAllLegalMoves(getSide());

    if(depth == max_depth){
        return evaluateFunction(board);
    }

    if(opponent_legal_moves.empty()){
        return evaluateFunction(board);
    }

    std::vector<float> scores;
    for(CellPosition move : opponent_legal_moves){
        if(getSearchTimeMiniSec() > max_time){
            break;
        }
        Board new_board = updateBoard(board, move, getOpponentSide(getSide()));
        float v = shallowMaxNode(new_board,depth+1,max_depth,best_move,alpha,beta);

        scores.push_back(v);

        if(v < beta){
            beta = v;
            if(alpha > beta){
                break;
            }
        }
    }

    return beta;
}

void MiniMaxPlayer3::setNodeCount(int count){
    node_count = count;
}

int MiniMaxPlayer3::getNodeCount(){
    return node_count;
}

void MiniMaxPlayer3::addNodeCount(){
    node_count ++;
}

void MiniMaxPlayer3::initializeWeightedSquare(){

    weighted_squares[0] = {120, -20, 20, 5, 5, 20, -20, 120};
    weighted_squares[1] = {-20, -40, -5, -5, -5, -5, -40, -20};
    weighted_squares[2] = {20, -5, 15, 3, 3, 15, -5, 20};
    weighted_squares[3] = {5, -5, 3, 3, 3, 3, -5, 5};
    weighted_squares[4] = {5, -5, 3, 3, 3, 3, -5, 5};
    weighted_squares[5] = {20, -5, 15, 3, 3, 15, -5, 20};
    weighted_squares[6] = {-20, -40, -5, -5, -5, -5, -40, -20};
    weighted_squares[7] = {120, -20, 20, 5, 5, 20, -20, 120};

    //sum = 376;
}

float MiniMaxPlayer3::calculateWeightSquare(const Board board)
{
    float score = 0;
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

float MiniMaxPlayer3::calculateSimplecount(const Board board)
{
     return (float)board.count(getOwnState(getSide()));
}
/*
float MiniMaxPlayer3::calculateMobility(std::vector<CellPosition>& opponent_legal_moves, std::vector<CellPosition>& legal_moves)
{
    return ((float)legal_moves.size() - (float)opponent_legal_moves.size()) * weights[getGamePhase(getTurnNumber())][1];
}
*/

float MiniMaxPlayer3::calculateMobility(const Board board){
    return (float)board.getAllLegalMoves(getSide()).size() - (float)board.getAllLegalMoves(getOpponentSide(getSide())).size();
}

void MiniMaxPlayer3::initializeGamePhaseBoarder(){
    game_phase_boarder = {0, 12, 36, 50, 58, 64};
}

void MiniMaxPlayer3::initializeWeights(){
    //{weighted squares, mobility, simple count}
    weights[0] = {(float)1.0, (float)10, (float)0.0};
    weights[1] = {(float)1.0, (float)8, (float)0.0};
    weights[2] = {(float)1.0, (float)6, (float)0.0};
    weights[3] = {(float)1.0, (float)6, (float)0.0};
    weights[4] = {(float)1.0, (float)6, (float)5.0};
}

void MiniMaxPlayer3::setTurnNumber(int turn){
    turn_number = turn;
}

int MiniMaxPlayer3::getTurnNumber(){
    return turn_number;
}

int MiniMaxPlayer3::getGamePhase(int turn){
    for(int i = 0 ; i < MiniMaxPlayer3::game_phase; i++){
        if( turn > game_phase_boarder[i] && turn <= game_phase_boarder[i+1]){
            return i;
        }
    }

    return 0;//Warning 処理、ここに来ることがない
}

bool MiniMaxPlayer3::endGameSearchNode(const Board board,Side side){
    addEndSearchNodeCount();

    if(board.checkGameEnd()){
        if(board.getWinner() == getOwnState(getSide())){
            return true;
        }else{
            return false;
        }
    }

    std::vector<CellPosition> opponent_legal_moves = board.getAllLegalMoves(getOpponentSide(getSide()));
    std::vector<CellPosition> legal_moves = board.getAllLegalMoves(getSide());

    bool canWin = true;
    if(side == getSide()){
        if((int)legal_moves.size() == 0){
            canWin = canWin && endGameSearchNode(board,getOpponentSide(getSide()));
        }
        for(CellPosition move : legal_moves){
            Board new_board = updateBoard(board, move, getSide());
            canWin = canWin && endGameSearchNode(new_board,getOpponentSide(getSide()));
        }
    }else{
        if((int)opponent_legal_moves.size() == 0){
            canWin = canWin && endGameSearchNode(board,getSide());
        }
        for(CellPosition move : opponent_legal_moves){
            Board new_board = updateBoard(board, move, getOpponentSide(getSide()));
            canWin = canWin && endGameSearchNode(new_board,getSide());
        }
    }
    return canWin;
}

bool MiniMaxPlayer3::endGameSearch(const Board board, CellPosition& best_move)
{
    std::cout << "In endGameSearch(), Turn :: " << getTurnNumber() << "\n"; 
    std::vector<CellPosition> legal_moves = board.getAllLegalMoves(getSide());
    for(CellPosition move : legal_moves){
        if(endGameSearchNode(updateBoard(board,move,getSide()),getOpponentSide(getSide()))){
            best_move = move;
            std::cout << "Found Win Route\n";
            return true;
        }
    }
    return false;
}

int MiniMaxPlayer3::getSearchTimeMiniSec()
{
    std::chrono::system_clock::time_point end_time = std::chrono::system_clock::now();
    return (int)std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
}

void MiniMaxPlayer3::setStartTime()
{
    start_time = std::chrono::system_clock::now();
}

void MiniMaxPlayer3::setEndSearchNodeCount(int count){
    end_search_node_count = count;
}

int MiniMaxPlayer3::getEndSearchNodeCount(){
    return end_search_node_count;
}

void MiniMaxPlayer3::addEndSearchNodeCount(){
    end_search_node_count++;
}

void MiniMaxPlayer3::setMaxDepth(){
    if(getTurnNumber() > 0 && getTurnNumber() <= 7){
        max_depth = 9;
    }else if(getTurnNumber() > 7 && getTurnNumber() <=36){
        max_depth = 7;
    }else if(getTurnNumber() > 36){
        max_depth = 7;
    }
}

} // namespace reversi
