// CSCI 1300 Fall 2022
// Author: Brock Hoos
// Recitation: 107 - Raegan Rychecky
// Project 3 - Game.h

#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Party.h"
#include "Challenge.h"
#include "Map.h"
#include <iostream>


using namespace std;

class Game
{
    
    public:
    // default constructor
    Game();

    // functions used in main driver file
    string removePartyMember();
    void removePartyMemberAt(int index);
    void monsterFight();
    void finalMonsterFight();
    void monsterFightRoom();
    void printStatus();
    void printInventory();
    void printMerchantMenu();
    void playerStart();
    int ready();
    void meetMerchant();
    void partyStatus();
    void printMap();
    void addRooms();
    void addNPCs();
    bool emptySpaceChoice();
    bool npcChoice();
    bool roomChoice();
    void exitChoice();
    bool isFreeSpace();
    bool isNPCLocation();
    bool isRoomLocation();
    bool isDungeonExit();
    int numRooms();
    int numParty();
    int anger();
    void getMisfortune();
    int fullness(int index);
    string playerName();
    int finalScore();


    private:
    // data members are other classes
    Party p;
    Player player;
    Challenge c;
    Map m;
};
#endif
