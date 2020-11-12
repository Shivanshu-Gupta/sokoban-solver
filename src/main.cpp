//
// Created by Shivanshu Gupta on 31/10/20.
//

#include "sokoban.h"

extern unordered_map<Move, string> move_names;

int main() {
    SokobanState state;
    //state.loadInputFile("../samples/sokoban00.txt");
    // state.loadInputFile("../samples/sokoban02.txt");
    //state.loadInputFile("../samples/sokoban01.txt");
    //state.outputBoard(cout);
    // cout << endl;
    // state.pos.print();

    // state.loadBoardFile("../samples/input00.txt");
//    state.loadBoardFile("../samples/input01.txt");
    state.loadBoardFile("../samples/input03.txt");
    state.outputBoard(cout);
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
    initialNode.startState = true;

    // Check individual Functions
    /*for(Move m: {U, D, L, R}) {
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
    }*/

    // Start Algorithms here...

    // DFS,DLS,IDS
    // Once previous errors are removed need to check these 
    // DFS will break down as the is_cycle() function is not written yet
    // DLS and IDS should work better, couldn't check
    auto nextNode = initialNode.depthLimitedSearch(10);
    // auto nextNode = initialNode.depthFirstSearch();
    // auto nextNode = initialNode.iterativeDeepeningSearch();
    
    // Run BFS and UCS on the initial node
    //auto nextNode = initialNode.breadthFirstSearch();
    // auto nextNode = initialNode.uniformCostSearch();	  
    cout << "Checkpoint 0" << endl;

    //Print the solution
    if (nextNode.has_value()) {
	if(initialNode.cutoff && !initialNode.goalFound)
	    cout << "Cutoff" << endl;
        else	
	    cout << "Goal Found" << endl;    
       /*SokobanNode* sNode = &(nextNode.value());
        (sNode->state)->outputBoard(cout);
	cout << "Outputting path" << endl;
        vector<Move> pathToGoal;
        do {
            cout << move_names[sNode->parentMove] << endl;		
            pathToGoal.push_back(sNode->parentMove);
	    cout << "Before: " << sNode->depth << endl;
            sNode = sNode->parentNode;
            //cout << "After: " << sNode->depth << endl;
	} while(!(sNode->startState));
	cout << "While Loop Exited" << endl;
        reverse(pathToGoal.begin(),pathToGoal.end());
        for(Move m : pathToGoal)
        cout << "     " << move_names[m] << ':' << " ";*/
    }
    else { // No solution found
        cout << "The algorithm couldn't find the solution\n";
    } 
}

