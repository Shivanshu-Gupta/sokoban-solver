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
    vector<Coord> walls, goals;
    vector<unordered_set<int>> wall_adj;
};

struct SokobanState {
    SokobanBoard* board;
    vector<Coord> boxes;
    vector<unordered_set<int>> box_adj;
    Coord pos;

    bool isValidMove(Move move);
    optional<SokobanState> doMove(Move move);

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
