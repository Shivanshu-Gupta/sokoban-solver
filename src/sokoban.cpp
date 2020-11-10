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
    for(int x = 0; x < b->n_rows; x++) {
        if(b->goal_adj[x] == box_adj[x]) return false;
    }
    return true;
}

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

optional<SokobanNode> SokobanNode::doMove(Move move) {
    SokobanNode newNode;
    auto newState = state->doMove(move); 
    if(newState.has_value()) {
        auto *newNodeState = new SokobanState(newState.value());
        newNode.state = newNodeState;
        newNode.parentNode = this;
        newNode.parentMove = move;
        newNode.depth = depth + 1;
        // update pathcost also
        newNode.pathCost++;

        return newNode;
    }
    return nullopt;
}

vector<SokobanNode> SokobanNode::getChildrenNode() {
    vector<SokobanNode> children;
    for(Move m: {U, D, L, R}) {
        auto childNode = this->doMove(m);
        if (childNode.has_value()) {
            children.push_back(childNode.value());
        }
    }
    return children;
}

optional<SokobanNode> SokobanNode::depthLimitedSearch(int limit) {
    stack<SokobanNode> frontier;
    frontier.push(*this);
    SokobanNode goalNode;
    cutoff = false;
    while(!frontier.empty()) {
        SokobanNode current = frontier.top();
        frontier.pop();
        if((current.state)->isGoalState()) {
            return current;
        }
        if(limit >=0 && current.depth > limit) {
            cutoff = true;
            return goalNode;
        }
        // else {should check for cycles}
        for(auto childnode : current.getChildrenNode()) {
            frontier.push(childnode);
        }
    }
    return nullopt;
}

optional<SokobanNode> SokobanNode::depthFirstSearch() {
    return (this->depthLimitedSearch(-1));
}

optional<SokobanNode> SokobanNode::iterativeDeepeningSearch() {
    int limit=0;
    optional<SokobanNode> result;
    do {
        result = this->depthLimitedSearch(limit);
        limit++;
    } while(cutoff); // continue if cutoff or solution not found
    return result;
}

//Breadth First Search(Tree Search) [Gets Killed as no bound on termination]
// To Do: 1. Metric to detect failure states(from where no solution is possible), 
//        2. Need to implement backtracking when failure occurs.
//        3. Find best move for a given state
optional<SokobanNode> SokobanNode::breadthFirstSearch(){
    queue<SokobanNode> frontier;
    if(((*this).state)->isGoalState())
    	return (*this);
    else
    	frontier.push(*this);
    	
    while(!frontier.empty()){
    	//cout << frontier.size() << endl;
    	SokobanNode current_node = frontier.front();
    	frontier.pop();
    	    	 
    	for(auto child_node : current_node.getChildrenNode()){
           //cout << "Depth : " << child_node.depth << endl;
    	    if((child_node.state)->isGoalState())
    	        return child_node;
    	    else
    	        //To do: Check if child_node has already been encountered before using parent_node pointers
    	        //If present, return without doing anything. Else, push it on the queue. 
    	        frontier.push(child_node);
    	}
    	    
    }
    return nullopt;

}

//Uniform Cost Search
optional<SokobanNode> SokobanNode::uniformCostSearch(){
    priority_queue<SokobanNode, vector<SokobanNode>, Compare> frontier;
    frontier.push(*this);
    
    while(!frontier.empty()){
        SokobanNode current_node = frontier.top();
        frontier.pop();
        
        if((current_node.state)->isGoalState()) {
            return current_node;
        }
                
        for(auto child_node : current_node.getChildrenNode()){
            // if(child_node.state != any element.state in frontier)
            // Use Hashing to store states
            // To Do: Push Hash states to an unordered_set  
            frontier.push(child_node);        
        }
    }
    return nullopt;
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
    // b->board[pos.x][pos.y] = '@';    // Chenged this, since pos is already stored
    b->board[pos.x][pos.y] = ' ';

    for(auto &[x, y]: goals) {
        if(b->board[x][y] == ' ' || b->board[x][y] == '$' || b->board[x][y] == '@') {
            if(b->board[x][y] == ' ') b->board[x][y] = '.';
            else if(b->board[x][y] == '$') b->board[x][y] = '*';
            else if(b->board[x][y] == '@') b->board[x][y] = '+';
            b->goal_adj[x].insert(y);
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
        b->wall_adj.emplace_back();
        b->goal_adj.emplace_back();
        box_adj.emplace_back();

        int y = 0;
        for(auto &ch: b->board.back()) {
            switch(ch) {
                case '#': b->wall_adj.back().insert(y); break;
                case '$': box_adj.back().insert(y); ch = ' '; break;
                case '*': box_adj.back().insert(y); b->goal_adj.back().insert(y); ch = '.'; break;
                case '@': pos = Coord{x, y}; ch = ' '; break;
                case '+': pos = Coord{x, y}; b->goal_adj.back().insert(y); ch = '.'; break;
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
    for(string l: b->board) {
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
