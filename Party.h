// CSCI 1300 Fall 2022
// Author: Brock Hoos
// Recitation: 107 - Raegan Rychecky
// Project 3 - Party.h

#ifndef PARTY_H
#define PARTY_H

#include <iostream>
#include <vector>

using namespace std;

class Party
{
    public:
    Party();

    // setters
    void setPlayerName(string player_name);
    void setPartyNamesAt(string name, int index);
    void setNumGold(int num_gold);
    void setNumIngredients(int num_ingredients);
    void setWeaponsAt(int index, string weapon);
    void setCookwareAt(int amount, int index);
    void setArmor(int num_armor);
    void setTreasureAt(int index);
    void setNumParty(int num);


    // getters
    string getPlayerName() const;
    string getPartyNamesAt(int index) const;
    int getNumGold() const;
    int getNumIngredients() const;
    string getWeaponAt(int index);
    int getNumWeapons() const;
    int getCookwareAt(int index) const;
    int getArmor() const;
    int getTreasureAt(int index) const;
    int getNumParty() const;
    

    // other
    int hasWeapon(int index);

    private:
    // data members
    string player_name_;
    string party_names_[4];
    int num_gold_;
    int num_ingredients_;
    string weapons_[5] = {"","","","",""};
    int cookware_[3] = {0,0,0};
    int armor_;
    int treasure_[5] = {0,0,0,0,0};
    int num_party_members;  
    

};

#endif