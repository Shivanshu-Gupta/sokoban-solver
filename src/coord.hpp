//
// Created by Shivanshu Gupta on 31/10/20.
//

#ifndef SOKOBAN_SOLVER_COORD_HPP
#define SOKOBAN_SOLVER_COORD_HPP

#include <iostream>
#include <vector>
#include <map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <fstream>

using namespace std;

struct Coord {
    int x, y;

    Coord(): x(0), y(0) {}
    Coord(const int &x, const int &y) : x(x), y(y) {}
    void print ()
    {
        cout<< "("<< x << ", " << y << ")" << endl;
    }
    // void output(ostream &out) {
    //     out<< "("<< x << ", " << y << ")" << endl;
    // }
    // void outputToFile(string outputPath) {
    //     ofstream fout(outputPath);
    //     output(fout);
    // }
    explicit Coord(const pair<int, int> &p) {
        x = p.first;
        y = p.second;
    }

    inline Coord &operator= (const pair<int, int> &other) {
        tie(x, y) = other;
        return *this;
    }

    Coord operator- () const { return {-x, -y}; }
    Coord operator+ () const { return {x, y}; }

    Coord operator+ (const Coord& other) const { return {x + other.x, y + other.y}; }
    Coord operator- (const Coord& other) const { return {x - other.x, y - other.y}; }

    Coord& operator+= (const Coord& rhs) { x += rhs.x; y += rhs.y; return *this;}
    Coord& operator-= (const Coord& rhs) { x -= rhs.x; y -= rhs.y; return *this;}

    Coord& operator+= (const pair<int, int>& rhs) { x += rhs.first; y += rhs.second; return *this;}
    Coord& operator-= (const pair<int, int>& rhs) { x -= rhs.first; y -= rhs.second; return *this;}

    Coord& operator+= (const int& val) { x += val; y += val; return *this; }
    Coord& operator-= (const int& val) { x -= val; y -= val; return *this; }
    Coord& operator*= (const int& val) { x *= val; y *= val; return *this; }
    Coord& operator/= (const int& val) { x /= val; y /= val; return *this; }

    friend Coord operator+ (const int& val, Coord rhs) { return rhs += val; }
    friend Coord operator+ (Coord lhs, const int& val) { return lhs += val; }

    friend Coord operator- (const int& val, Coord rhs) { return rhs -= val; }
    friend Coord operator- (Coord lhs, const int& val) { return lhs -= val; }

    friend Coord operator* (const int& val, Coord rhs) { return rhs *= val; }
    friend Coord operator* (Coord lhs, const int& val) { return lhs *= val; }

    friend Coord operator/ (const int& val, Coord rhs) { return rhs /= val; }
    friend Coord operator/ (Coord lhs, const int& val) { return lhs /= val; }

    friend bool operator== (const Coord& lhs, const Coord& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
    friend bool operator!= (const Coord& lhs, const Coord& rhs) { return lhs.x != rhs.x || lhs.y != rhs.y; }
};

#endif //SOKOBAN_SOLVER_COORD_HPP
