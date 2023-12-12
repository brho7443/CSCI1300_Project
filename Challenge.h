// CSCI 1300 Fall 2022
// Author: Brock Hoos
// Recitation: 107 - Raegan Rychecky
// Project 3 - Challenge.h

#ifndef CHALLENGE_H
#define CHALLENGE_H

#include <iostream>
#include <vector>

using namespace std;

class Challenge
{
    public:
    // default constructor
    Challenge();

    // setters
    void setMonsters(string file_name);
    void setNumMonsters(int num);
    void setRiddle(string file_name);
   
    //getters (only one because get riddle and get monster is in other function section)
    int getNumMonsters() const;

    //other
    string getMonstersAtLevel(int level);
    void removeMonster(string monster_name);
    string getRiddle();
    bool riddleAnswer(string riddle, string answer);
   

    private:
    // data members
    string monsters_[21];
    int num_monsters_;
    string riddles_[10];
};

#endif