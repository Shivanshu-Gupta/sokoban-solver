//
// Created by Rohith Reddy Gangam on 13/11/20.
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
extern unordered_map<Move, string> move_names;

struct Solution {
	bool goalFound = false;
	bool cutoff = false;
	SokobanNode* goalNode;
	// Algorithm & Solution charecteristics
	int max_frontier_size = 0;
	int num_nodes_reached = 0;
	// TODO: Define other relevant heuristics (required to gauge performance measures) here
};

// Declare functions for each algorithm
Solution depthLimitedSearch(SokobanNode* node, int limit);
Solution depthFirstSearch(SokobanNode* node);
Solution iterativeDeepeningSearch(SokobanNode* node);
Solution breadthFirstSearch(SokobanNode* node);
Solution uniformCostSearch(SokobanNode* node);

// Declare Helper functions
bool is_cycle(SokobanNode* node);
bool same_state(SokobanNode* node, SokobanNode* parent);
// TODO: Hash, A* Heuristics and other helper functions, as needed, to be added

// Define functions for each algorithm
// TODO: Should update all functions with relevant values of Algorithm & Solution charecteristics 

Solution depthLimitedSearch(SokobanNode* node, int limit) {
	// TODO: Destruct pointers
	Solution solution;
    stack<SokobanNode*> frontier;
    // cout<<" "<<node<<"; Calling from root node : Y/N : " << node->startNode<<endl;
    frontier.push(node);
    SokobanNode* current;
    while(!frontier.empty()) {
        current = frontier.top();
    	// current->state->outputBoard(cout);
        frontier.pop();
        if((current->state)->isGoalState()) {
        	solution.goalFound = true;
        	solution.cutoff = false;
            solution.goalNode = current;
            break;
        }
        if(limit >=0 && current->depth > limit) {
            solution.cutoff = true;
            continue;
        }
        if (!is_cycle(current)){
	        for(auto childnode : current->getChildrenNode()) {
	            frontier.push(childnode);
	        }        	
        }
    }

    return solution;
}

Solution depthFirstSearch(SokobanNode* node) {
    return (depthLimitedSearch(node,-1));
}

Solution iterativeDeepeningSearch(SokobanNode* node) {
    int limit=0;
    Solution result;
    do {
        result = depthLimitedSearch(node, limit);
        limit++;
    } while(result.cutoff); // continue if cutoff or solution not found
    return result;
}

// Copied comments from previous place
// @Radhit, check if the current code overcomes these problems
// Breadth First Search(Tree Search) [Gets Killed as no bound on termination]
// To Do: 1. Metric to detect failure states(from where no solution is possible), 
//        2. Need to implement backtracking when failure occurs.
//        3. Find best move for a given state
Solution breadthFirstSearch(SokobanNode* node) {
	Solution solution;
    queue<SokobanNode*> frontier;
    if(node->state->isGoalState()){
    	solution.goalFound = true;
        solution.goalNode = node;
        return solution;
    }
	frontier.push(node);    
	SokobanNode* current;
    while(!frontier.empty()) {
    	current = frontier.front();
        frontier.pop();
        for(auto child_node : current->getChildrenNode()){
            if((child_node->state)->isGoalState()) {
		    	solution.goalFound = true;
		        solution.goalNode = child_node;
		        return solution;
        	}
            else
                frontier.push(child_node);
        }
    }
    return solution;
}

Solution uniformCostSearch(SokobanNode* node) {
	Solution solution;
    priority_queue < SokobanNode*, vector<SokobanNode*>, Compare> frontier;
    frontier.push(node);
    while(!frontier.empty()){
        SokobanNode* current = frontier.top();
        frontier.pop();
        
        if((current->state)->isGoalState()) {
        	solution.goalFound = true;
            solution.goalNode = current;
            return solution;
        }

        for(auto child_node : current->getChildrenNode()){
            // if(child_node.state != any element.state in frontier)
            // Use Hashing to store states
            // TODO: Push Hash states to an unordered_set  
            frontier.push(child_node);        
        }
    }
    return solution;
}

bool is_cycle(SokobanNode* node)
{
	if(node->startNode) return false;
	SokobanNode* parent = node;
	do {
		parent = parent->parentNode;
		if(same_state(node,parent)){
			return true;
		}
	} while(!parent->startNode);
	return false;
}

bool same_state(SokobanNode* node, SokobanNode* parent)
{
	// TODO: update this function once hash is defined
	if(node->state->pos != parent->state->pos) return false;
	vector<unordered_set<int>> nodeBoxes = node->state->box_adj, parentBoxes = parent->state->box_adj;
    for(int x = 0; x < node->state->board->n_rows; x++) {
        if(nodeBoxes[x] != parentBoxes[x]) return false;
    }
	return true;
}