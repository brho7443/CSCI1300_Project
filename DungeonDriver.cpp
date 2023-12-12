// CSCI 1300 Fall 2022
// Author: Brock Hoos
// Recitation: 107 - Raegan Rychecky
// Project 3 - DungeonDriver.cpp

#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>

using namespace std;

int main()
{
    // default constructor for Game class
    Game g = Game();

    // player start function in game class (asks user for their name and party member names)
    g.playerStart();

    // asks player if they are ready for the game
    if(g.ready() == -1) // if they enter NO then game will quit
    {
        return 0;
    }

    // first meeting with merchant before dungeon entered
    g.meetMerchant();

    // printing status and inventory of party
    g.printStatus(); 
    g.printInventory();
    g.partyStatus();
    cout << endl;
    cout << endl; 

    // creating map with 5 rooms and 5 npcs
    g.addRooms();
    g.addNPCs();
    // print map to user
    g.printMap();

    // making sure quit is false, if true ever game will end
    bool quit;
    quit = false;

    // game continues if (rooms cleared is not 5 and player isnt on exit) or (party members is greater than 0) or (sorcerer anger is not 100) or (fullness of player does not equal 0)
    while((g.numRooms() < 5 && g.isDungeonExit() == false) || g.numParty() > 0 || g.anger() < 100 || quit == false || g.fullness(0) < 0)
    {
        // before each turn check fullness of each player
        for(int i = 0; i < g.numParty() + 1; i++)
        {
            int fullness = g.fullness(i);
            // if fullness of any player is below 20 print out statement
            if(fullness < 20)
            {
                cout << "Check your fullness of your party!!" << endl;
            }
            // if fullness reaches 0
            else if(fullness == 0)
            {
                // removes party member at index of 0
                g.removePartyMemberAt(i-1);
            }
        }

        // check which space player is on
        // if it is a free space
        if(g.isFreeSpace() == true)
        {
            quit = g.emptySpaceChoice();
            if(quit == true)
            {
                break;
            }
        }
        // if it is a room
        else if(g.isRoomLocation() == true)
        {
            quit = g.roomChoice();
            if(quit == true)
            {
                break;
            }
        }
        // if it is a NPC
        else if(g.isNPCLocation() == true)
        {
            quit = g.npcChoice();
            if(quit == true)
            {
                break;
            }
        }
        // if it is exit
        else if(g.isDungeonExit() == true)
        {
            // print out message saying they have not cleared enough rooms yet
            cout << "You have not cleared all the rooms. Move onto a new space now" << endl;
            // this function is in Game class, forces player to move off it 
            g.exitChoice();
        }
    }

    // if player has no party members
    if(g.numParty() == 0)
    {
        // print out that all their party members died and their final stats
        cout << "All other party members have died! GAME OVER" << endl;
        cout << "FINAL STATS" << endl;
        cout << "-------------" << endl;
        g.printStatus();
        g.printInventory();
        g.partyStatus();
    }
    // if sorcerer anger reaches 100
    else if(g.anger() == 100)
    {
        // print out the sorcerer destroyed dungeon and final stats
        cout << "The sorcerer destroyed the dungeon! GAME OVER" << endl;
        cout << "FINAL STATS" << endl;
        cout << "-------------" << endl;
        g.printStatus();
        g.printInventory();
        g.partyStatus();
    }
    // if player's fullness equals 0
    else if(g.fullness(0) == 0)
    {
        // print out dying of hunger message and final stats
        cout << "You have died of starvation!" << endl;
        cout << "FINAL STATS" << endl;
        cout << "-------------" << endl;
        g.printStatus();
        g.printInventory();
        g.partyStatus();

    }
    // if user manually quits
    else if(quit == true)
    {
        // print out final stats
        cout << "FINAL STATS" << endl;
        cout << "-------------" << endl;
        g.printStatus();
        g.printInventory();
        g.partyStatus();
    }
    // if user passes dungeon (5 rooms cleared and reached exit)
    else if(g.numRooms() == 5 && g.isDungeonExit() == true)
    {
        // print out congragulation message and final stats
        cout << "Hooray! You defeated the sorcer! Great job!" << endl;
        cout << "FINAL STATS" << endl;
        cout << "-------------" << endl;
        g.printStatus();
        g.printInventory();
        g.partyStatus();
    }

    // read final stats to a file
    ofstream fout;
    fout.open("results.txt");
    fout << g.playerName() << ": " << g.finalScore() << endl; //finalScore() function in Game class calculates a final score based on multiple factor
    fout << endl;

    return 0;
}
     



    
