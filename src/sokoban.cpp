//
// Created by Shivanshu Gupta on 31/10/20.
//

#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <cassert>
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

bool SokobanState::isGoalState() {
    SokobanBoard*& b = board;
    bool y = true;
    for(int x = 0; x < b->n_rows; x++) {
        if(b->goal_adj[x] != box_adj[x]) y = false;
    }
    return y;
}

optional<SokobanState*> SokobanState::doMove(Move move) {
    SokobanBoard*& b = board;
    const Coord &diff = moves[move];
    Coord nextPos = pos + diff;
    Coord next2Pos = pos + 2 * diff;

    if(!inBounds(nextPos) || isWall(nextPos)) return nullopt;
    if(isBox(nextPos)) {    // there's a box in the next position
        // wall or another box after the box => can't move
    
        if(isWall(next2Pos) || isBox(next2Pos)) return nullopt;
        
        SokobanState* nextState = new SokobanState(*this);
        nextState->pos = nextPos;
        nextState->box_adj[nextPos.x].erase(nextPos.y);
        nextState->box_adj[next2Pos.x].insert(next2Pos.y);
        nextState->setHashKey();
        // cout<< nextState->hashKey <<endl;
        return nextState;
    }

    SokobanState* nextState = new SokobanState(*this);
    nextState->pos = nextPos;
    nextState->setHashKey();
    // cout<< nextState->hashKey <<endl;
    return nextState;
}

optional<SokobanNode*> SokobanNode::doMove(Move move) {
    auto nextState = state->doMove(move);
    if(nextState.has_value()) {
        auto* newNode = new SokobanNode();
        newNode->startNode = false;
        newNode->state = nextState.value();
        newNode->parentNode = this;
        newNode->parentMove = move;
        newNode->depth = depth + 1;
        newNode->pathCost = pathCost + 1;
        return newNode;
    }
    return nullopt;
}

vector<SokobanNode*> SokobanNode::getChildrenNode() {
    vector<SokobanNode*> children;
    for(Move m: {D, R, L, U}) {
        auto childNode = this->doMove(m);
        if (childNode.has_value()) {
            children.push_back(childNode.value());
        }
    }
    return children;
}

void SokobanState::setHashKey() {
    hash<SokobanState> hasher;
    hashKey = hasher(*this);
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
    b->wall_adj.resize(b->n_rows);
    b->goal_adj.resize(b->n_rows);
    box_adj.resize(b->n_rows);

    for(auto &[x, y]: walls) {
        b->wall_adj[x].insert(y);
        b->board[x][y] = '#';
    }

    for(auto &[x, y]: boxes) {
        assert(b->board[x][y] == ' ');
        box_adj[x].insert(y);
        b->board[x][y] = '$';
    }

    assert(b->board[pos.x][pos.y] == ' ');
    b->board[pos.x][pos.y] = ' ';

    for(auto &[x, y]: goals) {
        if(b->board[x][y] == ' ' || b->board[x][y] == '$' || b->board[x][y] == '@') {
            if(b->board[x][y] == ' ') b->board[x][y] = '.';
            else if(b->board[x][y] == '$') b->board[x][y] = '*';
            else if(b->board[x][y] == '@') b->board[x][y] = '+';
            b->goal_adj[x].insert(y);
        } else throw "invalid board";
    }
    setHashKey();
    // cout<< hashKey <<endl;
}

void SokobanState::loadBoardFile(const string &inputPath) {
    board = new SokobanBoard;
    SokobanBoard*& b = board;
    ifstream fin(inputPath);
    string l;
    int x = 0;
    int y = 0;
    while(getline(fin, l)) {
        b->board.push_back(l);
        b->wall_adj.emplace_back();
        b->goal_adj.emplace_back();
        box_adj.emplace_back();

    y = 0;
        for(auto &ch: b->board.back()) {
            switch(ch) {
                case '#': b->wall_adj.back().insert(y); break;
                case '$': box_adj.back().insert(y); ch = ' '; break;
                case '*': box_adj.back().insert(y); b->goal_adj.back().insert(y); ch = '.'; break;
                case '@': pos = Coord{x, y}; ch = ' '; break;
                case '+': pos = Coord{x, y}; b->goal_adj.back().insert(y); ch = '.'; break;
        case '.': b->goal_adj.back().insert(y); ch = '.'; break;
        default: break;
            }
            y++;
        }
        x++;
    }
    b->n_rows = x;
    b->n_cols = y;
    setHashKey();
    // cout<< hashKey <<endl;
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

ostream &operator<< (ostream &out, const SokobanState &s) {
    SokobanBoard* b = s.board;
    int x = 0;
    for(string l: b->board) {
        for(int y: s.box_adj[x]) {
            if (l[y] == '.') l[y] = '*';
            else l[y] = '$';
        }
        if(x == s.pos.x) {
            if(l[s.pos.y] == '.') l[s.pos.y] = '+';
            else l[s.pos.y] = '@';
        }
        out << l << endl;
        x++;
    }
    return out;
}
