//
// Created by Shivanshu Gupta on 31/10/20.
//

#include "sokoban.h"

extern unordered_map<Move, string> move_names;

int main() {
    SokobanState state;
    // state.loadInputFile("../samples/sokoban00.txt");
     state.loadInputFile("../samples/sokoban01.txt");
    // state.outputBoard(cout);
    // cout << endl;
    // state.pos.print();

    // state.loadBoardFile("../samples/input00.txt");
//    state.loadBoardFile("../samples/input01.txt");
    // state.outputBoard(cout);
    // cout << endl;

    // for(Move m: {U, D, L, R}) {
    //     auto nextState = state.doMove(m);
    //     if (nextState.has_value()) {
    //         cout << move_names[m] << ':' << endl;
    //         nextState->outputBoard(cout);
    //     }
    // }
    // cout << endl;

    // Initialize the Problem Node
    SokobanNode initialNode;
    initialNode.state = &state;

    // Check individual Functions
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

    // Start Algorithms here...

    // DFS,DLS,IDS
    // Once previous errors are removed need to check these 
    // DFS will break down as the is_cycle() function is not written yet
    // DLS and IDS should work better, couldn't check
    // auto nextNode = initialNode->depthLimitedSearch(0);
    // auto nextNode = initialNode->depthFirstSearch();
    // auto nextNode = initialNode->iterativeDeepeningSearch();
    
    // Print the solution
    // if (nextNode.has_value()) {
    //     SokobanNode* sNode = (nextNode.value());
    //     (sNode->state)->outputBoard(cout);
    //     vector<Move> pathToGoal;
    //     do {
    //         pathToGoal.push_back(sNode->parentMove);
    //         sNode = sNode->parentNode;
    //     } while(sNode != NULL);
    //     reverse(pathToGoal.begin(),pathToGoal.end());
    //     for(Move m : pathToGoal)
    //     cout << move_names[m] << ':' << endl;
    // }
    // else { // No solution found
    //     cout << "The algorithm couldn't find the solution\n";
    // }
}

