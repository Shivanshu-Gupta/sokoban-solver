//
// Created by Shivanshu Gupta on 31/10/20.
//

#ifndef SOKOBAN_SOLVER_SOKOBAN_H
#define SOKOBAN_SOLVER_SOKOBAN_H

#include <iostream>
#include <vector>
#include <unordered_set>
#include <set>
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
    vector<set<int>> goal_adj;
    vector<unordered_set<int>> wall_adj;
};

struct SokobanState {
    Coord pos;
    SokobanBoard* board;
    vector<set<int>> box_adj;
    size_t hashKey = 0;

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

    optional<SokobanState*> doMove(Move move);
    inline bool isValidMove(Move move) { return doMove(move).has_value(); };
    bool isGoalState();
    void setHashKey();
    double computeHeuristicPerBox(Coord box_pos); //Returns minimum of the distances between the box and every goal location
    double computeHeuristic(); //Calculates the heuristic value for this node

    // Input
    void loadInputFile(const string &inputPath);
    void loadBoardFile(const string &inputPath);
    static pair<int, int> readCoords(ifstream &fin);
    static vector<Coord> readCoordsArray(ifstream &fin);

    friend ostream &operator<< (ostream &output, const SokobanState &s);
    friend bool operator== (const SokobanState& lhs, const SokobanState& rhs) {
        return lhs.pos == rhs.pos && lhs.box_adj == rhs.box_adj;
    }
};

struct SokobanNode {
    SokobanState* state = NULL;
    SokobanNode* parentNode = NULL;         // parent node of current node
    Move parentMove;                        // parentMove parent has taken to reach here
    int depth = 0;                          // depth of current node
    double pathCost = 0.0;                  // path cost from start node to this node
    double heuristicCost;                   // heuristic value of this node
    double functionCost; 		      // function value of this node
    bool usePathCost = true;                   // states if pathCost is to be used for priority queue sorting
    bool useHeuristicCost = false;             // states if heuristicCost is to be used for priority queue sorting
    bool startNode = false;                 // root Node
    optional<SokobanNode*> doMove(Move move);// returns Node reached via move, if move is possible
    vector<SokobanNode*> getChildrenNode();  // list of all successor nodes

    ~SokobanNode() {
        delete state;
    }
};

struct Compare {
    bool operator()(SokobanNode* a, SokobanNode* b) {
        return (a->functionCost > b->functionCost);
    }
};

// Generic hash_combine function
template <class T>
inline void hash_combine(std::size_t& seed, const T& v) {
    hash<T> hasher;
    //cout << "Seed: " << seed << endl;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
    //cout << "Hasher: " << seed << " " << hasher(v) << endl;
}

// Specialise std::hash for SokobanState
namespace std {
    template<>
    struct hash<SokobanState> {
        size_t operator()(SokobanState const& s) const noexcept {
            size_t h = 0;
            hash_combine(h, s.pos.x);
            hash_combine(h, s.pos.y);
            hash_combine(h, ':');
            for(const set<int>& ys: s.box_adj) {
                for(const int& y: ys) hash_combine(h, y);
                hash_combine(h, ':');
            }
            return h;
        }
    };
};

#endif //SOKOBAN_SOLVER_SOKOBAN_H
