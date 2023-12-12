// CSCI 1300 Fall 2022
// Author: Brock Hoos
// Recitation: 107 - Raegan Rychecky
// Project 3 - Player.h

#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>

using namespace std;

class Player
{
    public:
    // default constructor
    Player();

    //getters
    int getFullness(int index) const;
    int getAnger() const;
    int getNumRooms() const;
    int getNumKeys() const;


    //setters
    void setFullness(int fullness, int index);
    void setAnger();
    void setNumRooms();
    void setNumKeys(int keys);



    private:
    // data members
    int fullness_[5];
    int sorcerer_anger_;
    int num_rooms_;
    int num_keys_;


};

#endif