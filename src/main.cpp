//
// Created by Shivanshu Gupta on 31/10/20.
//

#include "sokoban.h"
#include "algos.hpp"

extern unordered_map<Move, string> move_names;

int main() {
    SokobanState state;
    // state.loadInputFile("../samples/sokoban00.txt");
    // state.outputBoard(cout);
    // state.pos.print();

    // state.loadBoardFile("../samples/input00.txt");
    state.loadBoardFile("../samples/input03.txt");
    state.outputBoard(cout);


    // Initialize the Problem Node
    SokobanNode* initialNode = new SokobanNode();
    initialNode->state = &state;
    initialNode->startNode = true;

    // Start Algorithms here...
    // auto nextNode = depthLimitedSearch(initialNode, 2);
    // auto nextNode = depthFirstSearch(initialNode);
    // auto nextNode = iterativeDeepeningSearch(initialNode);    
    // auto nextNode = breadthFirstSearch(initialNode);
    auto nextNode = uniformCostSearch(initialNode);   

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
        for(Move m : pathToGoal)
            cout << move_names[m] << ", ";
        cout<<"done... Terminating... \n";
    }
    else { // No solution found
        cout << "The algorithm couldn't find the solution\n";
    }
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