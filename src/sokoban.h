    //
// Created by Shivanshu Gupta on 31/10/20.
//

#ifndef SOKOBAN_SOLVER_SOKOBAN_H
#define SOKOBAN_SOLVER_SOKOBAN_H

#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <tuple>
#include <utility>
#include <fstream>
#include <optional>
#include "coord.hpp"

using namespace std;

enum Move {
    U, D, L, R
};

struct SokobanBoard {
    int n_rows, n_cols;
    vector<string> board;
    vector<unordered_set<int>> goal_adj;
    vector<unordered_set<int>> wall_adj;
};

struct SokobanState {
    Coord pos;
    SokobanBoard* board;
    vector<unordered_set<int>> box_adj;

    inline bool isWall(Coord p) { return board->board[p.x][p.y] == '#'; }
    inline bool isGoal(Coord p) {
        return board->board[p.x][p.y] == '.'
            || board->board[p.x][p.y] == '*'
            || board->board[p.x][p.y] == '+';
    }
    inline bool isBox(Coord p) { return box_adj[p.x].contains(p.y); }
    inline bool isPos(Coord p) { return pos == p; }

    inline bool inBounds(Coord p) {
        return 0 <= p.x && p.x < board->n_rows
            && 0 <= p.y && p.y < board->n_cols;
    }

    optional<SokobanState> doMove(Move move);
    inline bool isValidMove(Move move) { return doMove(move).has_value(); };

    void loadInputFile(const string &inputPath);
    void loadBoardFile(const string &inputPath);
    void outputBoard(ostream &out);
    void outputBoardToFile(string outputPath) {
        ofstream fout(outputPath);
        outputBoard(fout);
    }
    bool isGoalState();
    static pair<int, int> readCoords(ifstream &fin);
    static vector<Coord> readCoordsArray(ifstream &fin);
};

struct SokobanNode {
    SokobanState* state = NULL;
    SokobanNode* parentNode = NULL;     // parent node of current node
    Move parentMove;                          // parentMove parent has taken to reach here
    double pathCost = 0;                    // path cost from start node to this node
    int depth = 0;                          // depth of current node
    bool cutoff = true;
    optional<SokobanNode> doMove(Move move);
    vector<SokobanNode> getChildrenNode();
    // Define methods for each algorithm
    optional<SokobanNode> depthLimitedSearch(int limit);
    optional<SokobanNode> depthFirstSearch();
    optional<SokobanNode> iterativeDeepeningSearch();
    optional<SokobanNode> breadthFirstSearch();
    optional<SokobanNode> uniformCostSearch();
};

struct Compare
{
    bool operator()(const SokobanNode& a, const SokobanNode& b)
    {
        return (a.pathCost > b.pathCost);
    }
};
    
#endif //SOKOBAN_SOLVER_SOKOBAN_H
