// CSCI 1300 Fall 2022
// Author: Brock Hoos
// Recitation: 107 - Raegan Rychecky
// Project 3 - Challenge.cpp

#include "Challenge.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// default constructor
Challenge::Challenge()
{
    // set total num of monsters to 21
    num_monsters_ = 21;
    // read in monsters from file
    setMonsters("monsters.txt");
    // read in riddles from a file
    setRiddle("riddles.txt");

}

// setters

void Challenge::setMonsters(string file_name) // read in monster and level to array and store each line of file into seperate index of array
{
    // reading in from file
    ifstream fin(file_name);
    string line1;
    int i = 0;
    // check if file is open
    if(fin.is_open())
    {
        while(getline(fin, line1))
        {
            if(line1.length() > 0)
            {
                // store the whole line from file into array (includes level of monster)
                monsters_[i] = line1;
                i++;
                
            }  
        }
    
    }
    // close file
    fin.close();
}


void Challenge:: setNumMonsters(int num) // changes total num of monsters if they are defeated
{
    num_monsters_ = num;
}

void Challenge::setRiddle(string file_name) // read in riddles from file for NPC (includes answer too)
{
    // read file
    ifstream fin(file_name);
    string line1;
    int i = 0;
    // check if opened
    if(fin.is_open())
    {
        while(getline(fin, line1))
        {
            if(line1.length() > 0)
            {
                // store into riddle array
                riddles_[i] = line1;
                i++;
                
            }  
        }
    
    }
    // close file
    fin.close();
}



// getters

int Challenge:: getNumMonsters() const
{
    return num_monsters_;
}


// other 
/**
 * Algorithhm takes in the level of monster needed and finds a monster of that level and returns its name
 * 1. Initialize all variables
 * 2. While loop until level of random monster matches level inputted
 * 3. Get random index of monster array and get level of monster at that index
 * 4. If level found is equal to level needed get monster name and change flag variable to true to end while loop
 * Parameters: level (int)
 * Return: Monster name (string) 
 */
string Challenge::getMonstersAtLevel(int level) 
{
    // initialize all variables
    int test_level = -1;
    int random_monster_index = -1;
    string line;
    string test_line;
    string monster;
    int comma = -1;
    bool flag = false;

    // while loop until monster is found of same level
    while(flag == false)
    {
        // random index for mosnters array
        srand(time(0));
        random_monster_index = rand() % 20;
        line = monsters_[random_monster_index];
        // find comma and get level of monster at index
        comma = line.find(',');
        test_line = line[comma+1];
        test_level = stoi(test_line);

        // if the level found is the level that was needed
        if(level == test_level)
        {
            for(int i = 0; i < comma; i++)
            {
                // get monster name at same index
                monster += line[i];
            }
            // change flag to true to end while loop
            flag = true;
        }

    }

    // returns name of monster found
    return monster;
}


/**
 * Algorithm takes in monster name to remove and removes it from array
 * 1. Initialize variables
 * 2. Loop through monsters array
 * 3. Check if names are same
 * If yes, save index and break out of loop
 * If no, continue loop
 * 4. Change monster name at saved index to blank and reduce num of monsters - 1
 * Parameters: monster_name (string)
 */
void Challenge:: removeMonster(string monster_name)
{
    // initialize variables
    string line;
    int index = -1;
    // loop through monster array
    for(int i = 0; i < 21; i++)
    {
        // get name of monster at each index
        line = monsters_[i];
        line = line.substr(0, line.length() - 2);
        // check if names are the same
        if(line == monster_name)
        {
            // save index
            index = i;
            break;
        }
    }

    // change monster name to blank to remove it
    monsters_[index] = "";
    // change num of monsters to one less
    setNumMonsters(getNumMonsters() - 1);
}


/**
 * Algorithm gets random riddle to ask user when they encounter NPC
 * 1. Get random number for index
 * 2. Get riddle at this index
 * 3. Find the comma at that index and only save everything before it to not show answer
 * Parameters: none
 * Return: riddle (string)
 */
string Challenge::getRiddle()
{
    // get random num
    srand(time(0));
    int random_index = -1;
    int index = 0;
    string random_riddle = "";
    random_index = rand() % (10 - 1);

    // get riddle at index
    random_riddle = riddles_[random_index];

    // find comma and get everything before it to not show answer
    index = random_riddle.find(',');
    random_riddle = random_riddle.substr(0,index);

    // return riddle
    return random_riddle;
    
}

/**
 * Algorithm takes in riddle and the user answer and returns true correct answer or false for incorrect
 * 1. Initialize variables
 * 2. Loop through riddle array
 * 3. Get only the riddle at index wihtout the answer
 * 4. Check if riddles are the same
 * If yes, save index and break out of loop
 * If no, continue loop
 * 5. Get riddle and answer at saved index
 * 6. Only get answer (everything after comma)
 * If answer matches user answer return true
 * If answer deosn't match return false
 * Parameters: riddle (string), answer (string)
 * Return: True (if answer is correct) or False (if answer is incorrect)
 */
bool Challenge::riddleAnswer(string riddle, string answer)
{
    // initialize variables
    string test_riddle;
    string test_answer;
    int comma = -1;
    int index = -1;
    // loop through riddle array
    for(int i = 0; i < 10; i++)
    {
        // get riddle at index
        test_riddle = riddles_[i];
        // find comma nd get actual riddle with no answer
        comma = test_riddle.find(',');
        test_riddle = test_riddle.substr(0, comma);

        // if riddles match up, save index and break out of loop
        if(test_riddle == riddle)
        {
            index = i;
            break;
        }
    }

    // get whole line at index
    test_answer = riddles_[index];
    // find comma and get everyhing after for the answer
    comma = test_answer.find(',');
    test_answer = test_answer.substr(comma +1, test_answer.length() - comma);

    // if answer entered is the same as actual answer
    if(test_answer == answer)
    {
        return true;
    }
    // if answer entered doesn't match
    else 
    {
        return false;
    }
}



