// CSCI 1300 Fall 2022
// Author: Brock Hoos
// Recitation: 107 - Raegan Rychecky
// Project 3 - Player.cpp

#include "Player.h"
#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;

// default constructor
Player::Player()
{
    // sets fullness for each party member and player to 50
    for(int i = 0; i < 5; i++)
    {
        fullness_[i] = 50;
    }
    // set sorcerer anger to 0
    sorcerer_anger_ = 0;
    // set num of rooms cleared to 0
    num_rooms_ = 0;
    // set num of keys to 0
    num_keys_ = 0;
}

//getters
int Player::getFullness(int index) const
{
    return fullness_[index];
}

int Player::getAnger() const
{
    return sorcerer_anger_;
}


int Player::getNumRooms() const
{
    return num_rooms_;
}

int Player::getNumKeys() const
{
    return num_keys_;
}

// setters
void Player::setFullness(int fullness, int index) // set fullness to specific number for specific party member or player
{
    if(fullness > 50)
    {
        fullness_[index] = 5;
    }
    else
    {   
        fullness_[index] = fullness;
    }
}

void Player::setAnger() // add 1 to sorcerer anger
{
    sorcerer_anger_ ++;
}


void Player::setNumRooms() // add 1 num of rooms cleared
{
    num_rooms_ ++;
}
void Player::setNumKeys(int keys) // set num of keys to specific amount of keys
{
    num_keys_ = keys;
}