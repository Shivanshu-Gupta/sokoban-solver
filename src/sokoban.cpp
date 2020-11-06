//
// Created by Shivanshu Gupta on 31/10/20.
//

#include <iostream>
#include <vector>
#include <map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <fstream>
#include "coord.hpp"
#include "sokoban.h"

using namespace std;

unordered_map<Move, Coord> moves = {
        {U, {-1, 0}},
        {D, {1, 0}},
        {L, {0, -1}},
        {R, {0, 1}},
};

unordered_map<Move, string> move_names = {
        {U, "↑"},
        {D, "↓"},
        {L, "←"},
        {R, "→"},
};

optional<SokobanState> SokobanState::doMove(Move move) {
    SokobanBoard*& b = board;
    const Coord &diff = moves[move];
    Coord nextPos = pos + diff;
    Coord next2Pos = pos + 2 * diff;

    if(!inBounds(nextPos) || isWall(nextPos)) return nullopt;
    if(isBox(nextPos)) {    // there's a box in the next position
        // wall or another box after the box => can't move
        if(isWall(next2Pos) || isBox(next2Pos)) return nullopt;

        SokobanState newState(*this);
        newState.pos = nextPos;
        newState.box_adj[nextPos.x].erase(nextPos.y);
        newState.box_adj[next2Pos.x].insert(nextPos.y);
        return newState;
    }
    SokobanState newState(*this);
    newState.pos = nextPos;
    return newState;
}

void SokobanState::loadInputFile(const string &inputPath) {
    board = new SokobanBoard;
    SokobanBoard*& b = board;
    ifstream fin(inputPath);

    fin >> b->n_cols >> b->n_rows;

    auto walls = readCoordsArray(fin);
    auto boxes = readCoordsArray(fin);
    auto goals = readCoordsArray(fin);

    pos = readCoords(fin);

    b->board.resize(b->n_rows, string(b->n_cols, ' '));
    box_adj.resize(b->n_rows);

    for(auto &[x, y]: walls) {
        b->board[x][y] = '#';
    }

    for(auto &[x, y]: boxes) {
        assert(b->board[x][y] == ' ');
        box_adj[x].insert(y);
        b->board[x][y] = '$';
    }

    assert(b->board[pos.x][pos.y] == ' ');
    b->board[pos.x][pos.y] = '@';

    for(auto &[x, y]: goals) {
        if(b->board[x][y] == ' ' | b->board[x][y] == '$' | b->board[x][y] == '@') {
            if(b->board[x][y] == ' ') b->board[x][y] = '.';
            else if(b->board[x][y] == '$') b->board[x][y] = '*';
            else if(b->board[x][y] == '@') b->board[x][y] = '+';
        } else throw "invalid board";
    }
}

void SokobanState::loadBoardFile(const string &inputPath) {
    board = new SokobanBoard;
    SokobanBoard*& b = board;
    ifstream fin(inputPath);
    string l;
    int x = 0;
    while(getline(fin, l)) {
        b->board.push_back(l);
        box_adj.emplace_back();
        int y = 0;
        for(auto &ch: b->board.back()) {
            switch(ch) {
                case '$': box_adj.back().insert(y); ch = ' '; break;
                case '*': box_adj.back().insert(y); ch = '.'; break;
                case '@': pos = Coord{x, y}; ch = ' '; break;
                case '+': pos = Coord{x, y}; ch = '.'; break;
                default: break;
            }
            y++;
        }
        x++;
    }
    b->n_rows = x;
    b->n_cols = l.length();
}

void SokobanState::outputBoard(ostream &out) {
    SokobanBoard*& b = board;
    int x = 0;
    for(auto l: b->board) {
        for(int y: box_adj[x]) {
            if (l[y] == '.') l[y] = '*';
            else l[y] = '$';
        }
        if(x == pos.x) {
            if(l[pos.y] == '.') l[pos.y] = '+';
            else l[pos.y] = '@';
        }
        out << l << endl;
        x++;
    }
}

pair<int, int> SokobanState::readCoords(ifstream &fin) {
    int x, y;
    fin >> x >> y;
    return {x - 1, y - 1};
}

vector<Coord> SokobanState::readCoordsArray(ifstream &fin) {
    int n; fin >> n;
    vector<Coord> pairs(n);
    for(int i = 0; i < 2*n; i++) {
        int x; fin >> x;
        if(i%2) pairs[i/2].y = x - 1;
        else pairs[i/2].x = x - 1;
    }
    return pairs;
}