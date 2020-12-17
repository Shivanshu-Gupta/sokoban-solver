//
// Created by Shivanshu Gupta on 31/10/20.
//

#include <unordered_set>

#include "sokoban.h"
#include "algos.hpp"

extern unordered_map<Move, string> move_names;

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
    SokobanState state;
    // state.loadInputFile("../samples/sokoban00.txt");
    state.loadBoardFile("../samples/test_cases/n6/n6b1_1.txt");
    //state.loadBoardFile("../samples/input03.txt");
    // state.loadBoardFile(input_file);
    cout << state;

    // Initialize the Problem Node
    SokobanNode* initialNode = new SokobanNode();
    initialNode->state = &state;
    initialNode->startNode = true;
    
    

    // Start Algorithms here...

    // Tree search algos
    // auto nextNode = depthLimitedSearch(initialNode, 10, false);
    // auto nextNode = depthFirstSearch(initialNode, false);
    // auto nextNode = iterativeDeepeningSearch(initialNode, false);
    // auto nextNode = breadthFirstSearch(initialNode, false);
    // auto nextNode = uniformCostSearch(initialNode, false);
    //   auto nextNode = aStarSearch(initialNode, false); 
    // auto nextNode = greedyBFS(initialNode, false);

    // Graph search algos
    // auto nextNode = depthLimitedSearch(initialNode, 10, true);
    // auto nextNode = depthFirstSearch(initialNode, true);
    // auto nextNode = iterativeDeepeningSearch(initialNode, true);
    // auto nextNode = breadthFirstSearch(initialNode, true);
    // auto nextNode = uniformCostSearch(initialNode, true);
       auto nextNode = aStarSearch(initialNode, true);
    //   auto nextNode = greedyBFS(initialNode, true); 

    //Print the solution
    if (nextNode.goalFound) {
        SokobanNode* current = nextNode.goalNode;
        cout << "Outputting path: ";
        vector<Move> pathToGoal;
        while(!(current->startNode)) {
            pathToGoal.push_back(current->parentMove);
            current = current->parentNode;
        }
        reverse(pathToGoal.begin(),pathToGoal.end());
        cout << pathToGoal.size() << " ";
        for(Move m : pathToGoal)
            cout << move_names[m] << ", ";
        cout<<"done... Terminating... \n";
    }
    else { // No solution found
        cout << "The algorithm couldn't find the solution\n";
    }

    //Print algorithm characteristics
    cout << "max_frontier_size = " << nextNode.max_frontier_size << endl;
    cout << "max_reached_size = " << nextNode.max_reached_size << endl;
    cout << "num_nodes_explored = " << nextNode.num_nodes_explored << endl;  
}



    /*
    {     
        // check if movenames and outputBoard functions work
        for(Move m: {U, D, L, R}) {
            auto nextState = state.doMove(m);
            if (nextState.has_value()) {
                cout << move_names[m] << ':' << endl;
                nextState->outputBoard(cout);
            }
        }
        cout << endl;
    }
    */

    /*
    // Check individual Functions : optional, doMove() and pos.print()
    for(Move m: {U, D, L, R}) {
        auto nextNode = initialNode.doMove(m);
        if (nextNode.has_value()) {
            auto node = nextNode.value();
            cout << move_names[node.parentMove] << ':' << endl;
            cout << node.depth << ':' << endl;
            //----------------------------------
            // The following lines are not working properly
            // Need to check the doMove function for SokobanNode
            node.state->pos.print();
            node.state->outputBoard(cout);
            // cout << ((nextNode.value()).state)->pos.x << ':' << endl;
            //----------------------------------
        }
    }
    */
