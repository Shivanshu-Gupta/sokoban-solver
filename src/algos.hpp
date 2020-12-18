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
	int max_reached_size = 0;
	int num_nodes_explored = 0;
	// TODO: Define other relevant heuristics (required to gauge performance measures) here
};

// Declare functions for each algorithm
Solution depthLimitedSearch(SokobanNode* node, int limit, bool graphSearch);
Solution depthFirstSearch(SokobanNode* node, bool graphSearch);
Solution iterativeDeepeningSearch(SokobanNode* node, bool graphSearch);
Solution breadthFirstSearch(SokobanNode* node, bool graphSearch);
Solution uniformCostSearch(SokobanNode* node, bool graphSearch);
Solution aStarSearch(SokobanNode* node, bool graphSearch);
Solution greedyBFS(SokobanNode* node, bool graphSearch);

// Declare Helper functions
bool is_cycle(SokobanNode* node);
bool same_state(SokobanNode* node, SokobanNode* parent);
// TODO: Hash, A* Heuristics and other helper functions, as needed, to be added

// Define functions for each algorithm
// TODO: Should update all functions with relevant values of Algorithm & Solution charecteristics

Solution depthLimitedSearch(SokobanNode* node, int limit = -1, bool graphSearch = false) {
	// TODO: Destruct pointers
	Solution solution;
    stack<SokobanNode*> frontier;
    set< size_t > reached;
    // cout<<" "<<node<<"; Calling from root node : Y/N : " << node->startNode<<endl;
    frontier.push(node);
    if( graphSearch ) {
        reached.insert(node->state->hashKey);
    }
    SokobanNode* current;
    while(!frontier.empty()) {
        current = frontier.top();
    	// current->state->outputBoard(cout);
        if(solution.max_frontier_size < frontier.size())
            solution.max_frontier_size = frontier.size();
        frontier.pop();
        solution.num_nodes_explored+=1;
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
	        for(auto child_node : current->getChildrenNode()) {
                if (graphSearch) {
                    if (reached.find(child_node->state->hashKey) == reached.end()) {
                        reached.insert(child_node->state->hashKey);
                    }
                    else {
                        continue;
                    }
                }
	            frontier.push(child_node);
	        }
        }
    }
    solution.max_reached_size = reached.size();
    return solution;
}

Solution depthFirstSearch(SokobanNode* node, bool graphSearch = false) {
    return (depthLimitedSearch(node,-1, graphSearch));
}

Solution iterativeDeepeningSearch(SokobanNode* node, bool graphSearch = false) {
    int limit=0;
    Solution result;
    do {
        result = depthLimitedSearch(node, limit, graphSearch);
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
Solution breadthFirstSearch(SokobanNode* node, bool graphSearch = false) {
    Solution solution;
    queue<SokobanNode*> frontier;
    set< size_t > reached;
    if(node->state->isGoalState()){
    	solution.goalFound = true;
        solution.goalNode = node;
        solution.max_reached_size = reached.size();
        return solution;
    }
	frontier.push(node);
    if( graphSearch ) {
        reached.insert(node->state->hashKey);
    }
	SokobanNode* current;
    while(!frontier.empty()) {
    	current = frontier.front();
        if(solution.max_frontier_size < frontier.size())
            solution.max_frontier_size = frontier.size();
        frontier.pop();
        solution.num_nodes_explored+=1;
        for(auto child_node : current->getChildrenNode()){
            if((child_node->state)->isGoalState()) {
		    	solution.goalFound = true;
		        solution.goalNode = child_node;
                solution.max_reached_size = reached.size();
		        return solution;
        	}
            else {
                if (graphSearch) {
                    if (reached.find(child_node->state->hashKey) == reached.end()) {
                        reached.insert(child_node->state->hashKey);
                    }
                    else {
                        continue;
                    }
                }
                frontier.push(child_node);
            }
        }
    }
    solution.max_reached_size = reached.size();
    return solution;
}

Solution uniformCostSearch(SokobanNode* node, bool graphSearch = false) {
	Solution solution;
    priority_queue < SokobanNode*, vector<SokobanNode*>, Compare> frontier;
    map< size_t, SokobanNode*> reached;
    frontier.push(node);
    if( graphSearch ) {
        reached[node->state->hashKey] = node;
    }
    while(!frontier.empty()){
        SokobanNode* current = frontier.top();
        if(solution.max_frontier_size < frontier.size())
        solution.max_frontier_size = frontier.size();
        frontier.pop();
        solution.num_nodes_explored+=1;

        if((current->state)->isGoalState()) {
        	solution.goalFound = true;
            solution.goalNode = current;
            solution.max_reached_size = reached.size();
            return solution;
        }

        for(auto child_node : current->getChildrenNode()){
            if (graphSearch) {
                if (reached.find(child_node->state->hashKey) == reached.end()) {
                    reached[child_node->state->hashKey] = child_node;
                }
                else if (child_node->pathCost < reached[child_node->state->hashKey]->pathCost) {
                    reached[child_node->state->hashKey] = child_node;
                }
                else {
                    continue;
                }
            }
            frontier.push(child_node);
        }
    }
    solution.max_reached_size = reached.size();
    return solution;
}

Solution aStarSearch(SokobanNode* node, bool graphSearch = false) {
    Solution solution;
    priority_queue < SokobanNode*, vector<SokobanNode*>, Compare> frontier;
    map< size_t, SokobanNode*> reached;
    //map< size_t, SokobanNode*> reached_f;                    //Current Frontier Nodes
    frontier.push(node);
    node->heuristicCost = node->state->computeHeuristic();
    node->functionCost = node->pathCost + node->heuristicCost;
    node->useHeuristicCost = true;

    if( graphSearch ) {
        reached[node->state->hashKey] = node;
    }
    while(!frontier.empty()){
        SokobanNode* current = frontier.top();
        if(solution.max_frontier_size < frontier.size())
            solution.max_frontier_size = frontier.size();
        frontier.pop();
        solution.num_nodes_explored+=1;

        if((current->state)->isGoalState()) {
        	solution.goalFound = true;
            solution.goalNode = current;
            solution.max_reached_size = reached.size();
            return solution;
        }

        for(auto child_node : current->getChildrenNode()){
            if (graphSearch) {
                if (reached.find(child_node->state->hashKey) == reached.end()) {
                    reached[child_node->state->hashKey] = child_node;
                }
                else if (child_node->pathCost < reached[child_node->state->hashKey]->pathCost) {
                    reached[child_node->state->hashKey] = child_node;
                }
                else {
                    continue;
                }
            }
            frontier.push(child_node);
        }
    }
    solution.max_reached_size = reached.size();
    return solution;
}

Solution greedyBFS(SokobanNode* node, bool graphSearch = false) {
    Solution solution;
    priority_queue < SokobanNode*, vector<SokobanNode*>, Compare> frontier;
    map< size_t, SokobanNode*> reached;
    //map< size_t, SokobanNode*> reached_f;                    //Current Frontier Nodes
    frontier.push(node);
    node->heuristicCost = node->state->computeHeuristic();
    node->functionCost = node->heuristicCost;
    node->usePathCost = false;
    node->useHeuristicCost = true;

    if( graphSearch ) {
        reached[node->state->hashKey] = node;
    }
    while(!frontier.empty()){
        SokobanNode* current = frontier.top();
        if(solution.max_frontier_size < frontier.size())
            solution.max_frontier_size = frontier.size();
        frontier.pop();
        solution.num_nodes_explored+=1;

        if((current->state)->isGoalState()) {
        	solution.goalFound = true;
            solution.goalNode = current;
            solution.max_reached_size = reached.size();
            return solution;
        }

        for(auto child_node : current->getChildrenNode()){
            if (graphSearch) {
                if (reached.find(child_node->state->hashKey) == reached.end()) {
                    reached[child_node->state->hashKey] = child_node;
                }
                else if (child_node->pathCost < reached[child_node->state->hashKey]->pathCost) {
                    reached[child_node->state->hashKey] = child_node;
                }
                else {
                    continue;
                }
            }
            frontier.push(child_node);
        }
    }
    solution.max_reached_size = reached.size();
    return solution;
}

bool is_cycle(SokobanNode* node)
{
	if(node->startNode) return false;
	SokobanNode* parent = node;
	do {
		parent = parent->parentNode;
        // if(same_state(node,parent)){
		if((*(node->state)) == (*(parent->state))){
			return true;
		}
	} while(!parent->startNode);
	return false;
}

// bool same_state(SokobanNode* node, SokobanNode* parent)
// {
// 	// TODO: update this function once hash is defined
// 	if(node->state->pos != parent->state->pos) return false;
// 	vector<set<int>> nodeBoxes = node->state->box_adj, parentBoxes = parent->state->box_adj;
//     for(int x = 0; x < node->state->board->n_rows; x++) {
//         if(nodeBoxes[x] != parentBoxes[x]) return false;
//     }
// 	return true;
// }
