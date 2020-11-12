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
    cout << "Goal State Entered" << endl;
    bool y = true;

    for(int x = 0; x < b->n_rows; x++) {
	    cout << "X: " << x << endl;
	for(auto i : b->goal_adj[x]){    
	cout << "goal_adj: " << i << endl;
	}
	for(auto i: box_adj[x]){
	cout <<	"box_adj: " << i << endl;
	}	
        if(b->goal_adj[x] != box_adj[x]) y = false;
    }
    return y;
}

optional<SokobanState> SokobanState::doMove(Move move) {
    SokobanBoard*& b = board;
    const Coord &diff = moves[move];
    Coord nextPos = pos + diff;
    Coord next2Pos = pos + 2 * diff;
    //cout << "Checkpoint 6" << endl;
    //nextPos.print();
    //cout << "Rows: " << b->n_rows << "Columns: " << b->n_cols << endl;
    //cout << " " << inBounds(nextPos) << endl;
    //cout << " " << isWall(nextPos) << endl;

    if(!inBounds(nextPos) || isWall(nextPos)) return nullopt;
        //cout << "Checkpoint 7" << endl;
    if(isBox(nextPos)) {    // there's a box in the next position
        // wall or another box after the box => can't move
	outputBoard(cout);
	cout << "   " << isWall(nextPos) << endl;
	cout << "   " << isBox(nextPos) << endl;
	cout << "   " << isWall(next2Pos) << endl;
	cout << "   " << isBox(next2Pos) << endl;
	
        if(isWall(next2Pos) || isBox(next2Pos)) return nullopt;

	cout << "Inside if loop" << endl;
        SokobanState newState(*this);
        newState.pos = nextPos;
        newState.box_adj[nextPos.x].erase(nextPos.y);
        newState.box_adj[next2Pos.x].insert(next2Pos.y);
        return newState;
    }
    
    //cout << "Checkpoint 8" << endl;
    SokobanState newState(*this);
    newState.pos = nextPos;
    //nextPos.print();
    return newState;
}

optional<SokobanNode> SokobanNode::doMove(Move move) {
    auto newState = state->doMove(move); 
    if(newState.has_value()) {
        auto *newNodeState = new SokobanState(newState.value());
        SokobanNode* newNode = new SokobanNode();
        newNode->startState = false;
        newNode->state = newNodeState;
        newNode->parentNode = this;
        newNode->parentMove = move;
        newNode->depth = depth + 1;
        // update pathcost also
        newNode->pathCost = pathCost + 1;
	//cout << "Path Cost: " << newNode->pathCost << endl;
	//cout << "Depth: " << newNode->depth << "Parent Node Depth: " << newNode->parentNode->depth << endl;

        return *newNode;
    }
    return nullopt;
}

vector<SokobanNode> SokobanNode::getChildrenNode() {
    vector<SokobanNode> children;
    for(Move m: {D, R, L, U}) {
	//cout << move_names[m] << endl;    
        auto childNode = this->doMove(m);
        if (childNode.has_value()) {
	    //cout << move_names[m] << endl;
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
	current.state->outputBoard(cout);
        frontier.pop();
        if((current.state)->isGoalState()) {
	    goalFound = true;
            return current;
        }
        if(limit >=0 && current.depth > limit) {
            cutoff = true;
            continue;
        }
        // else {should check for cycles}
        for(auto childnode : current.getChildrenNode()) {
            frontier.push(childnode);
        }
    }
    if(cutoff)
        return goalNode;
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
    cout << "Checkpoint 1" << endl;	
    queue<SokobanNode> frontier;
    if((state)->isGoalState()){
	 cout << "Checkpoint 2" << endl;   
    	return (*this);
    }
    else
    	frontier.push(*this);
    	
    while(!frontier.empty()){
    	cout << "Checkpoint 3: " << frontier.size() << endl;
    	SokobanNode current_node = frontier.front();
	current_node.state->pos.print();
	//if(current_node.startState != true)
	    //cout << "     " << current_node.depth << " " << current_node.parentNode->depth << endl;
    	frontier.pop();
    	
         	
    	for(auto child_node : current_node.getChildrenNode()){
		child_node.state->pos.print();
	}
    	for(auto child_node : current_node.getChildrenNode()){
          // cout << "Checkpoint 4: " << frontier.size() << " " << child_node.depth << endl;
    	    if((child_node.state)->isGoalState()){
		cout << "Checkpoint 5" << endl;
    	        return child_node;
	    }
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
