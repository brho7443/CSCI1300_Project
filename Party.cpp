// CSCI 1300 Fall 2022
// Author: Brock Hoos
// Recitation: 107 - Raegan Rychecky
// Project 3 - Party.cpp

#include "Party.h"
#include <iostream>

using namespace std;

// default constructor
Party::Party()
{
    num_gold_ = 100; // set num of gold for party to 100 at start
    player_name_ = ""; // player name is reset each game
    num_ingredients_ = 0; // num of ingredients set to 0
    armor_ = 0; // armor is set ot 0
    num_party_members = 4; // num of party members is 4
}

// setters
void Party::setPlayerName(string player_name) // set player_name for game and to write to file at end
{
    player_name_ = player_name;
}

void Party::setPartyNamesAt(string name, int index) // party names are set at each index
{
    party_names_[index] = name;
}

void Party::setNumGold(int num_gold) // set num of gold to input
{
    num_gold_ = num_gold;
}

void Party::setNumIngredients(int num_ingredients) // set num ingredients to number 
{
    num_ingredients_ = num_ingredients_ + num_ingredients;
}

void Party::setWeaponsAt(int index, string weapon) // set weapon at specific index for person who holds it
{
    weapons_[index] = weapon;
}

void Party::setCookwareAt(int amount, int index) // set cookware at specific index to a number 
{
    cookware_[index] = cookware_[index] + amount;
}

void Party::setArmor(int num_armor) // set armor to number inputted
{
    armor_ = armor_ + num_armor;

}

void Party::setTreasureAt(int index) // add to treasure at specific index
{
    treasure_[index] ++;
}

void Party::setNumParty(int num) // set num of party to number inputted
{
    num_party_members = num;
}


// getters
string Party::getPlayerName() const
{
    return player_name_;
}

string Party::getPartyNamesAt(int index) const
{
    return party_names_[index];
}

int Party::getNumGold() const
{
    return num_gold_;
}

int Party::getNumIngredients() const
{
    return num_ingredients_;
}

string Party::getWeaponAt(int index)
{
    return weapons_[index];
}

int Party::getNumWeapons() const
{
   int num_weapons = 0;
    for(int i = 0; i < 5; i ++)
    {
        if(weapons_[i].length() > 0)
        {
            num_weapons ++;
        }
    }
    return num_weapons;
}

int Party::getCookwareAt(int index) const
{
    return cookware_[index];
}

int Party::getArmor() const
{
    return armor_;
}

int Party::getTreasureAt(int index) const
{
    return treasure_[index];
}

int Party::getNumParty() const
{
    return num_party_members;
}


// other 

/**
 * Algorithm returns number 1 if persona t index has a weapon or 0 if they do not have a weapon
 * 1. Check if a string is stored at index meaning there is a weapon
 * If yes, return 1
 * If no, return 0
 * Parameters: index (int)
 * Return: Either 1 or 0 depending on if person at index has a weapon or not
 */
int Party::hasWeapon(int index)
{
    if(weapons_[index].length() > 0)
    {
        return 1;
    }
    else 
    {
        return 0;
    }
}