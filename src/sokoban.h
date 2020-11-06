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
};

struct SokobanState {
    SokobanBoard* board;
    vector<unordered_set<int>> box_adj;
    Coord pos;

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
    static pair<int, int> readCoords(ifstream &fin);
    static vector<Coord> readCoordsArray(ifstream &fin);
};

struct SokobanNode {
    SokobanState* state;

    optional<SokobanNode> doMove(Move move);
//    vector<SokobanNode> getChildren();
};


#endif //SOKOBAN_SOLVER_SOKOBAN_H
