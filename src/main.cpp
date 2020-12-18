//
// Created by Shivanshu Gupta on 31/10/20.
//

#include <unordered_set>
#include <string>

#include "sokoban.h"
#include "algos.hpp"

extern unordered_map<Move, string> move_names;
bool DEBUG = false;

void hashingExample() {
    cout << "========================HashingExample========================" << endl;
    SokobanState state;
    state.loadBoardFile("../samples/input03.txt");
    cout << state;

    hash<SokobanState> hasher;
    size_t hash = hasher(state);
    cout << hash << endl;

    unordered_set<SokobanState> reached;
    reached.insert(state);
    for(const auto& s: reached) cout << s;
    cout << "===============================================================" << endl;
    cout << endl;
}

int main(int argc, char** argv) {
    //hashingExample();

    string input_file = argv[1];
    string algo = argc >= 3 ? argv[2] : "Astar";
    string variant = argc >= 4 ? argv[3] : "graph";
    if(DEBUG) cout << "Algorithm: " << algo << ' ' << variant << endl;
    SokobanState state;
    state.loadBoardFile(input_file);
    if(DEBUG) cout << state;

    // Initialize the Problem Node
    SokobanNode* initialNode = new SokobanNode();
    initialNode->state = &state;
    initialNode->startNode = true;

    Solution nextNode;
    if(variant == "tree") {
        // Tree search algos
        if(algo == "DFS") nextNode = depthFirstSearch(initialNode, false);
        else if(algo == "IDS") nextNode = iterativeDeepeningSearch(initialNode, false);
        else if(algo == "BFS") nextNode = breadthFirstSearch(initialNode, false);
        else if(algo == "UCS") nextNode = uniformCostSearch(initialNode, false);
        else if(algo == "Astar") nextNode = aStarSearch(initialNode, false);
        else if(algo == "GBFS") nextNode = greedyBFS(initialNode, false);
        else cout << "invalid algo. should be one of DFS, IDS, BFS, UCS, Astar or GBFS" << endl;
    } else if(variant == "graph") {
        // Graph search algos
        if(algo == "DFS") nextNode = depthFirstSearch(initialNode, true);
        else if(algo == "IDS") nextNode = iterativeDeepeningSearch(initialNode, true);
        else if(algo == "BFS") nextNode = breadthFirstSearch(initialNode, true);
        else if(algo == "UCS") nextNode = uniformCostSearch(initialNode, true);
        else if(algo == "Astar") nextNode = aStarSearch(initialNode, true);
        else if(algo == "GBFS") nextNode = greedyBFS(initialNode, true);
        else cout << "invalid algo. should be one of DFS, IDS, BFS, UCS, Astar or GBFS" << endl;
    }

    //Print the solution
    if (nextNode.goalFound) {
        SokobanNode* current = nextNode.goalNode;
        vector<Move> pathToGoal;
        while(!(current->startNode)) {
            pathToGoal.push_back(current->parentMove);
            current = current->parentNode;
        }
        reverse(pathToGoal.begin(),pathToGoal.end());
        cout << pathToGoal.size() << " ";
        for(Move m : pathToGoal)
            cout << move_names[m] << " ";
        cout << endl;
    }
    else { // No solution found
        cout << "The algorithm couldn't find the solution" << endl;
    }

    //Print algorithm characteristics
    if(DEBUG) {
        cout << "max_frontier_size = " << nextNode.max_frontier_size << endl;
        cout << "max_reached_size = " << nextNode.max_reached_size << endl;
        cout << "num_nodes_explored = " << nextNode.num_nodes_explored << endl;
    }
}