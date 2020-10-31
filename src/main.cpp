//
// Created by Shivanshu Gupta on 31/10/20.
//

#include "sokoban.h"

extern unordered_map<Move, string> move_names;

int main() {
    SokobanState state;
    state.loadInputFile("../samples/sokoban00.txt");
    state.outputBoard(cout);
    cout << endl;

    state.loadBoardFile("../samples/input01.txt");
    state.outputBoard(cout);
    for(Move m: {U, D, L, R}) {
        if(state.isValidMove(m)) cout << move_names[m] << ' ';
    }
    cout << endl;
}

