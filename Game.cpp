// CSCI 1300 Fall 2022
// Author: Brock Hoos
// Recitation: 107 - Raegan Rychecky
// Project 3 - Game.cpp

#include "Player.h"
#include "Party.h"
#include "Challenge.h"
#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>


using namespace std;

// default contructor
Game::Game()
{
    // use default constructors from all classes to reset game for a new game
    p = Party();
    player = Player();
    c = Challenge();
    m.resetMap();
}

/**
 * @Algorithm removes random party member for when monster captures them
 * 1. Get random number
 * 2. Get party member name at random index
 * 3. Change party member name at random index to blank
 * 4. Change weapon to blank at random index
 * 5. Change party member count to 1 less
 * Return: name of party member (string)
 */
string Game:: removePartyMember()
{
    // get random number
    srand(time(0));
    int random_index;
    random_index = (rand() % p.getNumParty()) + 1;
    random_index = random_index - 1;
    // get party members name at index before changing it 
    string name = p.getPartyNamesAt(random_index);

    // change party member name to blank
    p.setPartyNamesAt("", random_index);
    // change weapon for party member to blank
    p.setWeaponsAt((random_index + 1), "");
    // lose one party member in count
    p.setNumParty(p.getNumParty() - 1);


    // return party members name to tell user who was removed
    return name;

}


/**
 * Algorithm removes party member at specific index if member dies of hunger
 * 1. Use input of index to change party member name to blank
 * 2. Change weapon party member had to blank
 * 3. Change party member count to one less
 * Parameters: index (int)
 */
void Game:: removePartyMemberAt(int index)
{
    // change party member to blank
    p.setPartyNamesAt("", index);
    // change weapon fro party member to blank
    p.setWeaponsAt(index+1, "");
    // lose one party member in count
    p.setNumParty(p.getNumParty() - 1);
}

/**
 * Algorithm gets monster based on num of rooms cleared, allows party to fight or surrender if they have weapons, and calculates if they win or not based on formula
 * 1. Get level needed for monster
 * 2. Get monster name at level
 * 3. Check if party has at least one weapon
 * If yes, party can pick to fight of surrender
 * If no, party has to surrender and lose one party member at random
 * 4. Allow user to fight or surrender
 * 5. Check if input is valid
 * If user chooses to fight, calculate if they won or not based on formual. (greater to or equal to 0 = win)
 * 6. If user wins, add 10*level of gold and 5*level of ingredients and calc if monster drops key
 * 7. If user loses, lose quater of all gold and up to 30 ingredients
 * 8. If user chooses to surrender, lose one random party member
 */
void Game:: monsterFight()
{
    // get level of monster needed
    int level = player.getNumRooms() + 1;
    // get monster name
    string monster = c.getMonstersAtLevel(level);
    // print out attacking message
    cout << "Watch out! " << monster << " is attacking!" << endl;

    // check if party has at least one weapon to fight monster
    if(p.getNumWeapons() == 0)
    {
        // print out no weapon message
        cout << "You and you're party have no weapons! You must surrender now." << endl;
        // remove random party member
        string name = removePartyMember();
        // print out which party member was captured
        cout << name << " has been captured by " << monster << endl;
    }
    // party chooses what to do
    else
    {
        // print out options
        cout << "Select one:" << endl;
        cout << "1. Fight" << endl;
        cout << "2. Surrender" << endl;
        // get user choice
        int fight_choice;
        cin >> fight_choice;

        // check if choice is valid
        while(!(fight_choice == 1 || fight_choice == 2))
        {
            cout << "Invalid input. Enter either 1 or 2" << endl;
            cin >> fight_choice;
        }

        // choose to fight
        if(fight_choice == 1)
        {
            // print fight message
            cout << "You and your party will fight now!" << endl;
            int outcome_1;
            // get num of weapons
            outcome_1 = (p.getNumWeapons());
            int r1;
            // get random number 1-6
            srand(time(0));
            r1 = (rand() % 6) + 1;
            // calculate first part of formula
            outcome_1 = outcome_1 * r1;
            for(int i = 0; i < p.getNumWeapons(); i ++)
            {
                // find if party has special weapons
                string weapon;
                weapon = p.getWeaponAt(i);
                // add 1 to first part of formula
                if(weapon == "(+1) Mythril Rapier")
                {
                    outcome_1 = outcome_1 + 1;
                }
                // add 2 to formula
                else if(weapon == "(+2) Flaming Battle-Axe")
                {
                    outcome_1 = outcome_1 + 2;
                }
                // add 3 to formula
                else if(weapon == "(+3) Vorpal Longsword")
                {
                    outcome_1 = outcome_1 + 3;
                }
            }

            // calc second part of formula
            int r2;
            int outcome_2;
            r2 = (rand() % 6) + 1;
            // random number multiplied by monster level, then divided by how much armor party has
            outcome_2 = (r2 * level) / p.getArmor();
            
            // subtract both parts of formula
            int final_outcome;
            final_outcome = outcome_1 - outcome_2;

            // if outcome is greater than or equal to 0
            if(final_outcome >= 0)
            {
                // print out win message
                cout << "Your party defeated " << monster << endl;
                cout << "Your party has gained " << (10 * level) << " gold" << endl;
                // add to gold supply
                p.setNumGold(p.getNumGold() + (10*level));
                cout << "Your party has gained " << (5 * level) << " ingredients" << endl;
                // add to ingredients
                p.setNumIngredients((5*level));

                // get random num to see if monster will drop key or not
                srand(time(0));
                // 10% chance of key
                if(((rand() % 10) + 1) == 1)
                {
                    cout << monster << " dropped a key!" << endl;
                    player.setNumKeys(player.getNumKeys() + 1);
                }
                // 90% no key
                else
                {
                    cout << monster << " did not drop a key" << endl;
                }
            }
            // party loses to monster (outcome is less than 0)
            else
            {
                // print loss staement
                cout << monster << " defeated your party" << endl;
                int new_gold;
                // losee quater of all gold
                new_gold = p.getNumGold() / 4;
                p.setNumGold(new_gold);
                cout << "Your party has " << new_gold << " gold now" << endl;
                // lose random amount of ingredients up to 30
                int new_ingredients;
                new_ingredients = (rand() % 30) + 1;
                // if amount of ingredients loss is greater than ingredients party has, set to 0
                if(p.getNumIngredients() <= new_ingredients)
                {
                    p.setNumIngredients(0);
                }
                // subtract ingredients loss from ingredients
                else
                {
                    p.setNumIngredients(p.getNumIngredients() - new_ingredients);
                }
                cout << "Your party has " << p.getNumIngredients() << " ingredients now" << endl;
            }

        }
        // user chooses to surrender
        else if(fight_choice == 2)
        {
            // print out surrender statement
            cout << "You have surrendered." << endl;
            // remove random party member
            string name = removePartyMember();
            cout << name << " has been captured by " << monster;
        }
    }
}


/**
 * Algorithm is used for final boss fight against the sorcerer, calculates if user wins the game or not 
 * 1. Check if user has weapons, if no, remove one random partt member
 * 2. Battle sorcerer
 * 3. Calculate fighting formula, same as regular monster fight
 * 4. If party defeats sorcerer, rooms cleared are now 5
 * 5. If they lose, they lose a key, get a misfortune and 50% chance to lose 5 fullness for each party member
 */
void Game::finalMonsterFight()
{
    // print final battle
    cout << "You have reached the final battle!" << endl;
    cout << "Prepare to Battle the sorcerer" << endl;
    
    // check if user has weapons
    int level = 6;
    // no weapons
    if(p.getNumWeapons() == 0)
        {
        cout << "You and you're party have no weapons! You must surrender now." << endl;
        string name = removePartyMember();
        cout << name << " has been captured by the sorcerer" << endl;
        }
        // has weapons
        else
        {
            
                cout << "You and your party will fight now!" << endl;
                int outcome_1;
                // calc first part of formula
                outcome_1 = (p.getNumWeapons());
                int r1;
                srand(time(0));
                r1 = (rand() % 6) + 1;
                outcome_1 = outcome_1 * r1;
                for(int i = 0; i < p.getNumWeapons(); i ++)
                {
                    string weapon;
                    weapon = p.getWeaponAt(i);
                    if(weapon == "(+1) Mythril Rapier")
                    {
                        outcome_1 = outcome_1 + 1;
                    }
                    else if(weapon == "(+2) Flaming Battle-Axe")
                    {
                        outcome_1 = outcome_1 + 2;
                    }
                    else if(weapon == "(+3) Vorpal Longsword")
                    {
                        outcome_1 = outcome_1 + 3;
                    }
                }

            int r2;
            // calc second part of formula
            int outcome_2;
            r2 = (rand() % 6) + 1;
            outcome_2 = (r2 * level) / p.getArmor();
            
            int final_outcome;
            final_outcome = outcome_1 - outcome_2;

            // if outcome is greater than or equal to 0
            if(final_outcome >= 0)
            {
                cout << "Your party defeated the sorcerer" << endl;
                player.setNumRooms();
                m.removeRoom(m.getPlayerRow(), m.getPlayerCol());          
            }
            // outcome is less than 0
            else
            {
                cout <<  "The sorcerer defeated your party" << endl;
                // lose key
                if(player.getNumKeys() > 0)
                {
                    player.setNumKeys(player.getNumKeys() - 1);
                }
                // random misfortune
                getMisfortune();
                
            }

        }
    // 50% chance to lose 5 fullness
    srand(time(0));
    int lose_fullness;
    for(int i = 0; i < p.getNumParty(); i++)
    {
        lose_fullness = (rand() % 10) + 1;
        if(lose_fullness <= 5)
        {
            player.setFullness(player.getFullness(i) - 5, i);
        }
    }

}

/**
 * Algorithm is used for party when fighting a monster to clear a room 
 * 1. Get random monster using getMonsterAtLevel function in Challenge class
 * 2. Check if party has weapons. If no, they must surrender
 * 3. If they have weapons, they fight the monster
 * 4. Calculate final outcome of battle from battle formula
 * 5. If party wins, num of rooms is +1 and lose key, removes room from map and removes monster. 40% chance of misfortune
 * 6. If party loses. Lose key and 60%b chance fro misfortune
 * 7. 50% chance to lose fulness for each party member
 */
void Game::monsterFightRoom()
{
        // get random monster
        int level = player.getNumRooms() + 1;
        string monster = c.getMonstersAtLevel(level);
        cout << "Watch out! " << monster << " is attacking!" << endl;

        // check if party has weapon
        if(p.getNumWeapons() == 0)
        {
            cout << "You and you're party have no weapons! You must surrender now." << endl;
            string name = removePartyMember();
            cout << name << " has been captured by " << monster;
        }
        // fight monster
        else
        {
            
                cout << "You and your party will fight now!" << endl;
                // calc battle outcome
                int outcome_1;
                outcome_1 = (p.getNumWeapons());
                int r1;
                srand(time(0));
                r1 = (rand() % 6) + 1;
                outcome_1 = outcome_1 * r1;
                for(int i = 0; i < p.getNumWeapons(); i ++)
                {
                    string weapon;
                    weapon = p.getWeaponAt(i);
                    if(weapon == "(+1) Mythril Rapier")
                    {
                        outcome_1 = outcome_1 + 1;
                    }
                    else if(weapon == "(+2) Flaming Battle-Axe")
                    {
                        outcome_1 = outcome_1 + 2;
                    }
                    else if(weapon == "(+3) Vorpal Longsword")
                    {
                        outcome_1 = outcome_1 + 3;
                    }
                }

            int r2;
            int outcome_2;
            r2 = (rand() % 6) + 1;
            outcome_2 = (r2 * level) / p.getArmor();
            
            int final_outcome;
            final_outcome = outcome_1 - outcome_2;

            // if outcome is greater than or equal to 0 (win)
            if(final_outcome >= 0)
            {
                cout << "Your party defeated " << monster << endl;
                // add to num of rooms
                player.setNumRooms();
                // lose key if they have 1
                if(player.getNumKeys() > 0)
                {
                    player.setNumKeys(player.getNumKeys() - 1);
                }
                // remove room from map
                m.removeRoom(m.getPlayerRow(), m.getPlayerCol());
                // misfortune 40%
                srand(time(0));
                int misfortune_ = (rand() % 10 )+1;
                if(misfortune_ <= 4)
                {
                    getMisfortune();
                }
            }
            // if outcome is less than 0 (loss)
            else
            {
                cout << monster << " defeated your party" << endl;
                // lose key if they entered room with key
                if(player.getNumKeys() > 0)
                {
                    player.setNumKeys(player.getNumKeys() - 1);
                }
                 // misfortune 60%
                 srand(time(0));
                int misfortune_ = (rand() % 10 )+1;
                if(misfortune_ <= 6)
                {
                    getMisfortune();
                }
            }

        }
    // 50% chance to lose 1 fullness
    srand(time(0));
    int lose_fullness;
    for(int i = 0; i < p.getNumParty(); i++)
    {
        lose_fullness = (rand() % 10) + 1;
        if(lose_fullness <= 5)
        {
            player.setFullness(player.getFullness(i) - 1, i);
        }
    }
}

/**
 * Algorithm prints updated rooms cleared, num of keys, and anger of sorcerer
 * 1. Use getter function in player class to stayt updated on party status
 */
void Game::printStatus()
{
    cout << "+-------------+" << endl;
    cout << "| STATUS      |" << endl;
    cout << "+-------------+" << endl;
    cout << "| Rooms Cleared: " << player.getNumRooms() << " | Keys: " << player.getNumKeys() << " | Anger Level: " << player.getAnger() << endl;
}

/**
 * Algorithm prints out party updated inventory after each turn using getters in party class
 * 1. Get num gold and print it out
 * 2. Get Num ingredients
 * 3. Get cookware at each index 
 * 4. Get weapons for each player
 * 5. Get num of armor
 * 6. Get treasure party has
 */
void Game::printInventory()
{
    cout << "+-------------+" << endl;
    cout << "| INVENTORY   |" << endl;
    cout << "+-------------+" << endl;
    cout << "| Gold        | " << p.getNumGold() << endl;
    cout << "| Ingredients | " << p.getNumIngredients() << " kg" << endl;
    cout << "| Cookware    | P: "  << p.getCookwareAt(0) << " | F: " << p.getCookwareAt(1) << " | C: " << p.getCookwareAt(2) << endl;
    cout << "| Weapons     | " << p.getPlayerName() << " " << p.getWeaponAt(0) << "  " << p.getPartyNamesAt(0) << " " << p.getWeaponAt(1) <<  "  " << p.getPartyNamesAt(1) << " " << p.getWeaponAt(2) << "  " << p.getPartyNamesAt(2) << " " << p.getWeaponAt(3) << "  " << p.getPartyNamesAt(3) << " " << p.getWeaponAt(4) << endl;
    cout << "| Armor       | " << p.getArmor() << endl;
    cout << "| Treasures   | R: " << p.getTreasureAt(0) << " | N: " << p.getTreasureAt(1) << " | B: " << p.getTreasureAt(2) << " | C: " << p.getTreasureAt(3) << " | G: " << p.getTreasureAt(4) << endl;
}

/**
 * Algorithm prints out party members name and there fullness after each turn using party and player getter functions
 * 1. Get player name and fullness
 * 2. Get party member 1 name and fullness
 * 3. Get party member 2 name and fullness
 * 4. Get party member 3 name and fullness
 * 5. Get party member 4 name and fullness
 */
void Game::partyStatus()
{
    cout << "+-------------+" << endl;
    cout << "| PARTY       |" << endl;
    cout << "+-------------+" << endl;
    cout << "| " << p.getPlayerName() << " | " << player.getFullness(0) << endl;
    cout << "| " << p.getPartyNamesAt(0) << " | " << player.getFullness(1) << endl;
    cout << "| " << p.getPartyNamesAt(1) << " | " << player.getFullness(2) << endl;
    cout << "| " << p.getPartyNamesAt(2) << " | " << player.getFullness(3) << endl;
    cout << "| " << p.getPartyNamesAt(3) << " | " << player.getFullness(4) << endl;
    cout << endl;
}


/**
 * Algortihm prints out menu for merchant
 * 1. Print menu
 */
void Game::printMerchantMenu()
{       
    // print menu
    cout << "Welcome to my shop! Buy as little or as much as you want, but remember you can't come back once you leave." << endl;
    cout << endl;
    cout << "Choose one of the following:" << endl;
    cout << "1. Ingredients: To make food, you have to cook raw ingredients." << endl;
    cout << "2. Cookware: You will need something to cook those ingredients." << endl;
    cout << "3. Weapons: It's dangerous to go alone, take this!" << endl;
    cout << "4. Armor: If you want to survive monster attacks, you will need some armor." << endl;
    cout << "5. Sell treasures: If you find anything shiny, I would be happy to take it off your hands." << endl;
    cout << "6. Leave: Make sure you get everything you need, I'm leaving after this sale!" << endl;
    cout << endl;
    cout << "Choose an option:" << endl;
}


/**
 * Algorithm is used to start the gaem and get player name as well as all party member names
 * 1. Get player name from user
 * 2. Get each party member name
 * 3. Store into array to save player and party member names
 * 4. Print welcome statement
 */
void Game::playerStart()
{
    // get player name
    cout << "What is your name?" << endl;
    string player_name;
    cin >> player_name;
    p.setPlayerName(player_name);

    // get party names
    string party_member1;
    string party_member2;
    string party_member3;
    string party_member4;

        // store party names
        cout << "What is one party member name?" << endl;
        cin >> party_member1;
        p.setPartyNamesAt(party_member1, 0);

        cout << "What is the second party member name?" << endl;
        cin >> party_member2;
        p.setPartyNamesAt(party_member2, 1);

        cout << "What is the third party member name?" << endl;
        cin >> party_member3;
        p.setPartyNamesAt(party_member3, 2);

        cout << "What is the final party member name?" << endl;
        cin >> party_member4;
        p.setPartyNamesAt(party_member4, 3);
    

    // welcome statement
    cout << endl;
    cout << "Welcome " << p.getPlayerName() << " and your party of " << p.getPartyNamesAt(0) << ", " << p.getPartyNamesAt(1) << ", " << p.getPartyNamesAt(2) << ", and " << p.getPartyNamesAt(3) << endl;
    cout << endl;
}

/**
 * Algorithm chackes to see if player wants to continue playing
 * 1. Ask user if they want to continue
 * If no, return -1 signifying they dont want to play 
 * If yes, return 1 signifying they still want to play
 * Return: 1 for yes or -1 for no
 */
int Game::ready()
{
    // ask question
    char ready;
    cout << "Are you ready to begin? Y/N" << endl;
    cin >> ready;

    // yes they want to continue
    if(ready == 'Y' || ready == 'y')
    {
        cout << endl;
        printInventory();
        cout << endl;
    }
    // no they dont want to continue
    else 
    {
        cout << "Goodbye." << endl;
        return -1;
    }

    return 1;
}

/**
 * Algorithm is an interactive menu for buying things from NPCs, it is used at the satrt of the game and when player wants to buy from NPC. The price is updated based on rooms cleared
 * 1. Print out menu and get user input
 * 2. Loop until choice is invalid
 * If choice is 1, allow user to buy ingredients in multiples of 5 as long as they have enough gold
 * If choice is 2, allow user to buy 3 types of cookware as long as they have enough gold
 * If choice is 3, allow user to buy up to 5 weapons and give them to certain party memebers as long as they have enough gold
 * If choice is 4, allow user to buy armor as long as they have enough gold
 * If choice is 5, allow user to sell the treasures they have
 */
void Game::meetMerchant()
{
    // print menu and get user input
    printMerchantMenu();
    int merchant_choice;
    cin >> merchant_choice;
    // while merchant choice is valid
    while(merchant_choice < 6 && merchant_choice > 0)
    {
        // buy ingredients
        if(merchant_choice ==1)
        {
            // ask how many they want to buy in multiples of 5
            cout << "How many kg of ingredients do you need [" << 1 * (1 + (player.getNumRooms() * 0.25)) << " Gold/kg]? (Enter a positive mulitple of 5)" << endl;
            int num_ingredients;
            cin >> num_ingredients;

            // if ingredients not in multiples of 5 ask again
            while((num_ingredients % 5) != 0 || num_ingredients < 0)
            {
                cout << "Please enter of multiple of 5 or 0 to cancel." << endl;
                cin >> num_ingredients;
            }

            // check to make sure they want to buy this many ingredients for this price
            cout << "You want to buy " << num_ingredients << " kg of ingredients for " << num_ingredients * (1 + (player.getNumRooms() * 0.25)) << " Gold? (Y/N)" << endl;
            char answer;
            cin >> answer;

            // if they confirm the purchase
            if(answer == 'Y' || answer == 'y')
            {
                // get price
                int total = num_ingredients * (1 + (player.getNumRooms() * 0.25));
                // if price is more than the gold they currently have
                while(total > p.getNumGold())
                {
                    // get new number of ingredients
                    cout << "Sorry you dont have enough gold. How much do you want? (Enter a multiple of 5 or 0 to cancel)." << endl;
                    cin >> num_ingredients;
                    total = num_ingredients  * (1 + (player.getNumRooms() * 0.25));
                }

                // change num gold 
                p.setNumGold(p.getNumGold() - total);
                // change num of ingredients
                p.setNumIngredients(num_ingredients);

                cout << endl;
                cout << "Thanks for your business. What else can I get for you?" << endl;
            }

        }
        // buying cookware
        else if(merchant_choice == 2)
        {
            // cookware menu
            cout << "I have a several types of cookware, which one would you like?" << endl;    
            cout << "Each type has a different probability of breaking when used, marked with (XX%)." << endl;
            cout << endl;
            cout << "Choose one of the following:" << endl;
            cout << "1. (25%) Ceramic Pot [" << 2  * (1 + (player.getNumRooms() * 0.25)) << " Gold]" << endl;
            cout << "2. (10%) Frying Pan [" << 10 * (1 + (player.getNumRooms() * 0.25)) << " Gold]" << endl;
            cout << "3. ( 2%) Cauldron [" << 20 * (1 + (player.getNumRooms() * 0.25)) << " Gold]" << endl;
            cout << "4. Cancel" << endl;
            int cookware_choice;
            cin >> cookware_choice;
            // if they didnt cancel sale
            if(cookware_choice != 4)
            {
                // ceramic pot
                if(cookware_choice ==1)
                {
                    // get quantity
                    cout << "How many would you like?" << endl;
                    int quantity;
                    cin >> quantity;
                    
                    // if quantity is negative
                    while(quantity < 0)
                    {
                        cout << "Please enter positive integer" << endl;
                        cin >> quantity;
                    }

                // confirm price with user
                cout << "You want to buy (" << quantity << ") Ceramic Pot(s) for " << quantity * (2  * (1 + (player.getNumRooms() * 0.25))) << " Gold? Y/N" << endl;
                    char answer;
                    cin >> answer;

                    // confirm sale
                    if(answer == 'y' || answer == 'Y')
                    {
                        int total = quantity * (2 * (1 + (player.getNumRooms() * 0.25)));

                        // if total is more than gold they have
                        while(total > p.getNumGold())
                        {
                            cout << "Sorry you dont have enough gold. How many do you want? (Enter postive integer or 0 to cancel)." << endl;
                            cin >> quantity;
                            total = quantity * (2 * (1 + (player.getNumRooms() * 0.25)));
                        }
                        
                        // set gold 
                        p.setNumGold(p.getNumGold() - total);
                        // set cookware
                        p.setCookwareAt(quantity, 0);
                    

                        cout << "Thanks for your business. What else can I get for you?" << endl;
                    }
                
                }
                // frying pan
                else if(cookware_choice == 2)
                {
                    // get quantity
                    cout << "How many would you like?" << endl;
                    int quantity;
                    cin >> quantity;
                    
                    // if quantity is negative
                    while(quantity < 0)
                    {
                        cout << "Please enter positive integer" << endl;
                        cin >> quantity;
                    }

                    // confirm purchase
                    cout << "You want to buy (" << quantity << ") Frying Pan(s) for " << quantity* (10 * (1 + (player.getNumRooms() * 0.25))) << " Gold? Y/N" << endl;
                    char answer;
                    cin >> answer;
                    if(answer == 'y' || answer == 'Y')
                    {
                        int total = quantity * (10 * (1 + (player.getNumRooms() * 0.25)));

                        // check if user has enough gold
                        while(total > p.getNumGold())
                        {
                            cout << "Sorry you dont have enough gold. How many do you want? (Enter postive integer or 0 to cancel)." << endl;
                            cin >> quantity;
                            total = quantity * (10 * (1 + (player.getNumRooms() * 0.25)));
                        }
                        
                        // set gold
                        p.setNumGold(p.getNumGold() - total);
                        // set frying pans
                        p.setCookwareAt(quantity, 1);
                    

                        cout << "Thanks for your business. What else can I get for you?" << endl;
                    }
                    
                }
                // for caludron
                else if(cookware_choice==3)
                {
                    // get quantity
                    cout << "How many would you like?" << endl;
                    int quantity;
                    cin >> quantity;
                    
                    // check quantity is positive
                    while(quantity < 0)
                    {
                        cout << "Please enter positive integer" << endl;
                        cin >> quantity;
                    }

                    // confirm purchase
                    cout << "You want to buy (" << quantity << ") Cauldron(s) for " << quantity*(20 * (1 + (player.getNumRooms() * 0.25))) << " Gold? Y/N" << endl;
                    char answer;
                    cin >> answer;

                    if(answer == 'y' || answer == 'Y')
                    {
                        int total = quantity * (20 * (1 + (player.getNumRooms() * 0.25)));
                        // check they have enough gold
                        while(total > p.getNumGold())
                        {
                            cout << "Sorry you dont have enough gold. How many do you want? (Enter postive integer or 0 to cancel)." << endl;
                            cin >> quantity;
                            total = quantity * (20 * (1 + (player.getNumRooms() * 0.25)));
                        }
                        
                        // set gold
                        p.setNumGold(p.getNumGold() - total);
                        // set cauldrons
                        p.setCookwareAt(quantity, 2);
                        cout << "Thanks for your business. What else can I get for you?" << endl;
                    }
                    
                }
            }

            // invalid chocie of cookware
            while(cookware_choice > 4 || cookware_choice <=0)
            {
                cout << "Please enter valid input" << endl;
                cin >> cookware_choice;
            }
        }
        // buying weapons
        else if(merchant_choice==3)
        {
            // print out weapon menu
            cout << "I have a plentiful collection of weapons to choose from, what would you like?" << endl;
            cout << "Note that some of them provide you a special bonus in combat, marked by a (+X)." << endl;
            cout << endl;
            cout << "Choose one of the following:" << endl;
            cout << "1. Stone Club [" << 2 * (1 + (player.getNumRooms() * 0.25)) << " Gold]" << endl;
            cout << "2. Iron Spear [" << 2 * (1 + (player.getNumRooms() * 0.25)) << " Gold]" << endl;
            cout << "3. (+1) Mythril Rapier [" << 5 * (1 + (player.getNumRooms() * 0.25)) << " Gold]" << endl;
            cout << "4. (+2) Flaming Battle-Axe [" << 15 * (1 + (player.getNumRooms() * 0.25)) << " Gold]" << endl;
            cout << "5. (+3) Vorpal Longsword [" << 50 * (1 + (player.getNumRooms() * 0.25)) << " Gold]" << endl;
            cout << "6. Cancel" << endl;
            int weapon_choice;
            cin >> weapon_choice;

            // stone club
            if(weapon_choice == 1)
            {
                // get quantity
                 cout << "How many would you like?" << endl;
                    int quantity;
                    cin >> quantity;
                    
                    // if quantity is negative
                    while(quantity < 0)
                    {
                        cout << "Please enter positive integer" << endl;
                        cin >> quantity;
                    }

                    // make sure they dont buy more than 5 weapons
                    while((p.getNumWeapons() + quantity) > p.getNumParty() + 1)
                        {
                            cout << "Sorry, you can only have 5 weapons max and one for each party member. Try a different quantity or 0 to cancel" << endl;
                            cin >> quantity; 
                            
                        }

                // confirm purchase
                cout << "You want to buy (" << quantity << ") Stone club(s) for " << quantity*(2 * (1 + (player.getNumRooms() * 0.25))) << " Gold? Y/N" << endl;
                    char answer;
                    cin >> answer;
                    if(answer == 'y' || answer == 'Y')
                    {

                        int total = quantity * (2* (1 + (player.getNumRooms() * 0.25)));

                        // check they have enough gold
                        while(total > p.getNumGold())
                        {
                            cout << "Sorry you dont have enough gold. How many do you want? (Enter postive integer or 0 to cancel)." << endl;
                            cin >> quantity;
                            total = quantity * (2 * (1 + (player.getNumRooms() * 0.25)));
                        }

                        // giving weapons to specific people
                         for(int i = 0; i < quantity; i++)
                        {
                            // index for weapons
                            cout << "Who would you like to give the weapon(s) too? (0 for yourself, 1, for first party memeber, 2 for second party member, etc)" << endl;
                            int user_choice_party_member;
                            cin >> user_choice_party_member;
                            
                            // check fro valid input
                            while(user_choice_party_member > 4 || user_choice_party_member < 0)
                            {
                                cout << "Enter valid option" << endl;
                                cin >> user_choice_party_member;
                            }
                            
                            // check if player doesnt have weapon
                            while(p.hasWeapon(user_choice_party_member) == 1)
                            {
                                cout << "This player already has a weapon. Select another one" << endl;
                                cin >> user_choice_party_member;
                            }

                            // make sure player is alive
                            while(p.getPartyNamesAt(user_choice_party_member - 1) == "")
                            {
                                cout << "This player has died. Select anotehr player to give it too." << endl;
                                cin >> user_choice_party_member;
                            }

                            // set weapon for player
                            p.setWeaponsAt(user_choice_party_member, "Stone Club");
                            
                            // if they give it to themselves
                            if(user_choice_party_member == 0)
                            {
                                cout << "You have given " << p.getPlayerName() << " a Stone Club" << endl;
                                cout << endl;
                            }
                            // weapon to party member
                            else
                            {
                                cout << "You have given " << p.getPartyNamesAt(user_choice_party_member - 1) << " a Stone Club" << endl;
                                cout << endl;
                            }
                        }
                        
                        // set gold
                        p.setNumGold(p.getNumGold() - total);
                        
                        cout << "Thanks for your business. What else can I get for you?" << endl;
                    }
            }
            // iron spear
            else if(weapon_choice == 2)
            {
                // get quantity
                 cout << "How many would you like?" << endl;
                    int quantity;
                    cin >> quantity;
                    
                    // check if quantity is positive
                    while(quantity < 0)
                    {
                        cout << "Please enter positive integer" << endl;
                        cin >> quantity;
                    }

                    // check they dont have 5 weapons
                    while((p.getNumWeapons() + quantity) > p.getNumParty() + 1)
                        {
                            cout << "Sorry, you can only have 5 weapons max. Try a different quantity or 0 to cancel" << endl;
                            cin >> quantity; 
                            
                        }

                // confirm purchase
                cout << "You want to buy (" << quantity << ") Iron Spear(s) for " << quantity*(2 * (1 + (player.getNumRooms() * 0.25))) << " Gold? Y/N" << endl;
                    char answer;
                    cin >> answer;
                    if(answer == 'y' || answer == 'Y')
                    {

                        int total = quantity * (2 * (1 + (player.getNumRooms() * 0.25)));
                        
                        // check if they have enough gold
                        while(total > p.getNumGold())
                        {
                            cout << "Sorry you dont have enough gold. How many do you want? (Enter postive integer or 0 to cancel)." << endl;
                            cin >> quantity;
                            total = quantity * (2 * (1 + (player.getNumRooms() * 0.25)));
                        }
                        
                        // give weapons to players
                         for(int i = 0; i < quantity; i++)
                        {
                            cout << "Who would you like to give the weapon(s) too? (0 for yourself, 1, for first party memeber, 2 for second party member, etc)" << endl;
                            int user_choice_party_member;
                            cin >> user_choice_party_member;
                            
                            // invalid input
                            while(user_choice_party_member > 4 || user_choice_party_member < 0)
                            {
                                cout << "Enter valid option" << endl;
                                cin >> user_choice_party_member;
                            }

                            // party member is alive
                            while(p.getPartyNamesAt(user_choice_party_member - 1) == "")
                            {
                                cout << "This player has died. Select anotehr player to give it too." << endl;
                                cin >> user_choice_party_member;
                            }

                            // check if party member already has weapon
                            while(p.hasWeapon(user_choice_party_member) == 1)
                            {
                                cout << "This player already has a weapon. Select another one" << endl;
                                cin >> user_choice_party_member;
                            }

                            // give player weapon
                            p.setWeaponsAt(user_choice_party_member, "Iron Spear");

                            // if weapon was given to player
                            if(user_choice_party_member == 0)
                            {
                                cout << "You have given " << p.getPlayerName() << " an Iron Spear" << endl;
                                cout << endl;
                            }
                            // if given to party member
                            else
                            {
                                cout << "You have given " << p.getPartyNamesAt(user_choice_party_member - 1) << " an Iron Spear" << endl;
                                cout << endl;
                            }

                        }
                        
                        // set gold
                        p.setNumGold(p.getNumGold() - total);

                         cout << "Thanks for your business. What else can I get for you?" << endl;
                    }
            }
            // mythril rapier
            else if(weapon_choice == 3)
            {
                // get quantity
                 cout << "How many would you like?" << endl;
                    int quantity;
                    cin >> quantity;
                    
                    // check if valid
                    while(quantity < 0)
                    {
                        cout << "Please enter positive integer" << endl;
                        cin >> quantity;
                    }

                    // check under 5 weapons
                    while((p.getNumWeapons() + quantity) > (p.getNumParty() + 1))
                        {
                            cout << "Sorry, you can only have 5 weapons max. Try a different quantity or 0 to cancel" << endl;
                            cin >> quantity; 
                            
                        }

                // confirm purchase
                cout << "You want to buy (" << quantity << ") (+1) Mythril Rapier(s) for " << quantity*(5 * (1 + (player.getNumRooms() * 0.25))) << " Gold? Y/N" << endl;
                    char answer;
                    cin >> answer;
                    if(answer == 'y' || answer == 'Y')
                    {

                        int total = quantity * (5 * (1 + (player.getNumRooms() * 0.25)));

                        // check if they have enough gold
                        while(total > p.getNumGold())
                        {
                            cout << "Sorry you dont have enough gold. How many do you want? (Enter postive integer or 0 to cancel)." << endl;
                            cin >> quantity;
                            total = quantity * (5 * (1 + (player.getNumRooms() * 0.25)));
                        }

                        // give weapons to specific players
                         for(int i = 0; i < quantity; i++)
                        {
                            // get index
                            cout << "Who would you like to give the weapon(s) too? (0 for yourself, 1, for first party memeber, 2 for second party member, etc)" << endl;
                            int user_choice_party_member;
                            cin >> user_choice_party_member;

                            // invalid input
                            while(user_choice_party_member > 4 || user_choice_party_member < 0)
                            {
                                cout << "Enter valid option" << endl;
                                cin >> user_choice_party_member;
                            }
                            
                            // check if party member is alive
                            while(p.getPartyNamesAt(user_choice_party_member - 1) == "")
                            {
                                cout << "This player has died. Select anotehr player to give it too." << endl;
                                cin >> user_choice_party_member;
                            }

                            // check player doesnt have weapon
                            while(p.hasWeapon(user_choice_party_member) == 1)
                            {
                                cout << "This player already has a weapon. Select another one" << endl;
                                cin >> user_choice_party_member;
                            }

                            // give weapon to player
                            p.setWeaponsAt(user_choice_party_member, "(+1) Mythril Rapier");
                            
                            // for player themself
                            if(user_choice_party_member == 0)
                            {
                                cout << "You have given " << p.getPlayerName() << " a (+1) Mythril Rapier" << endl;
                                cout << endl;
                            }
                            // for party member
                            else
                            {
                                cout << "You have given " << p.getPartyNamesAt(user_choice_party_member - 1) << " a (+1) Mythril Rapier" << endl;
                                cout << endl;
                            }



                        }
                      
                        // set gold
                        p.setNumGold(p.getNumGold() - total);
                        
                        cout << "Thanks for your business. What else can I get for you?" << endl;
                    }
            }   
            // Flaming battle-axe 
            else if(weapon_choice == 4)
            {
                // get quantity
                cout << "How many would you like?" << endl;
                    int quantity;
                    cin >> quantity;
                    
                    // invalid quantity
                    while(quantity < 0)
                    {
                        cout << "Please enter positive integer" << endl;
                        cin >> quantity;
                    }

                    // over 5 weapons
                    while((p.getNumWeapons() + quantity) > (p.getNumParty() + 1))
                        {
                            cout << "Sorry, you can only have 5 weapons max. Try a different quantity or 0 to cancel" << endl;
                            cin >> quantity; 
                            
                        }

                // confirm purchase
                cout << "You want to buy (" << quantity << ") (+2) Flaming Battle-Axe(s) for " << quantity*(15* (1 + (player.getNumRooms() * 0.25))) << " Gold? Y/N" << endl;
                    char answer;
                    cin >> answer;
                    if(answer == 'y' || answer == 'Y')
                    {

                        int total = quantity * (15 * (1 + (player.getNumRooms() * 0.25)));

                        // check they have enough gold
                        while(total > p.getNumGold())
                        {
                            cout << "Sorry you dont have enough gold. How many do you want? (Enter postive integer or 0 to cancel)." << endl;
                            cin >> quantity;
                            total = quantity * (15 * (1 + (player.getNumRooms() * 0.25)));
                        }

                        // give weapons to specific players
                         for(int i = 0; i < quantity; i++)
                        {
                            // get index
                            cout << "Who would you like to give the weapon(s) too? (0 for yourself, 1, for first party memeber, 2 for second party member, etc)" << endl;
                            int user_choice_party_member;
                            cin >> user_choice_party_member;

                            // invalid input
                            while(user_choice_party_member > 4 || user_choice_party_member < 0)
                            {
                                cout << "Enter valid option" << endl;
                                cin >> user_choice_party_member;
                            }
                            
                            // check player is alive
                            while(p.getPartyNamesAt(user_choice_party_member - 1) == "")
                            {
                                cout << "This player has died. Select anotehr player to give it too." << endl;
                                cin >> user_choice_party_member;
                            }

                            // player doesnt already have weapon
                            while(p.hasWeapon(user_choice_party_member) == 1)
                            {
                                cout << "This player already has a weapon. Select another one" << endl;
                                cin >> user_choice_party_member;
                            }

                            // set weapon
                            p.setWeaponsAt(user_choice_party_member, "(+2) Flaming Battle-Axe");
                            
                            // for player
                            if(user_choice_party_member == 0)
                            {
                                cout << "You have given " << p.getPlayerName() << " a (+2) Flaming Battle-Axe" << endl;
                                cout << endl;
                            }
                            // for party member
                            else
                            {
                                cout << "You have given " << p.getPartyNamesAt(user_choice_party_member - 1) << " a (+2) Flaming Battle-Axe" << endl;
                                cout << endl;
                            }

                        }
                        
                        // set gold
                        p.setNumGold(p.getNumGold() - total);
                        
                        cout << "Thanks for your business. What else can I get for you?" << endl;
                    }
            }
            // vorpal longsword
            else if(weapon_choice == 5)
            {
                // get quantity
                cout << "How many would you like?" << endl;
                    int quantity;
                    cin >> quantity;
                    
                    // invalid quantity
                    while(quantity < 0)
                    {
                        cout << "Please enter positive integer" << endl;
                        cin >> quantity;
                    }

                    // max of 5 weapons
                    while((p.getNumWeapons() + quantity) > (p.getNumParty() + 1))
                        {
                            cout << "Sorry, you can only have 5 weapons max. Try a different quantity or 0 to cancel" << endl;
                            cin >> quantity; 
                            
                        }

                // confirm purchase
                cout << "You want to buy (" << quantity << ") (+3) Vorpal Longsword(s) for " << quantity*(50* (1 + (player.getNumRooms() * 0.25))) << " Gold? Y/N" << endl;
                    char answer;
                    cin >> answer;
                    if(answer == 'y' || answer == 'Y')
                    {

                        int total = quantity * (50 * (1 + (player.getNumRooms() * 0.25)));

                        // check they have enough gold
                        while(total > p.getNumGold())
                        {
                            cout << "Sorry you dont have enough gold. How many do you want? (Enter postive integer or 0 to cancel)." << endl;
                            cin >> quantity;
                            total = quantity * (50 * (1 + (player.getNumRooms() * 0.25)));
                        }

                        // give weapons to specific people
                         for(int i = 0; i < quantity; i++)
                        {
                            cout << "Who would you like to give the weapon(s) too? (0 for yourself, 1, for first party member, 2 for second party member, etc)" << endl;
                            int user_choice_party_member;
                            cin >> user_choice_party_member;

                            // inavlid person
                            while(user_choice_party_member > 4 || user_choice_party_member < 0)
                            {
                                cout << "Enter valid option" << endl;
                                cin >> user_choice_party_member;
                            }

                            // check member is alive
                            while(p.getPartyNamesAt(user_choice_party_member - 1) == "")
                            {
                                cout << "This player has died. Select anotehr player to give it too." << endl;
                                cin >> user_choice_party_member;
                            }

                            // check player doesnt have weapon
                            while(p.hasWeapon(user_choice_party_member) == 1)
                            {
                                cout << "This player already has a weapon. Select another one" << endl;
                                cin >> user_choice_party_member;
                            }

                            // set weapons
                            p.setWeaponsAt(user_choice_party_member, "(+3) Vorpal Longsword");
                            
                            // for player
                            if(user_choice_party_member == 0)
                            {
                                cout << "You have given " << p.getPlayerName() << " a (+3) Vorpal Longsword" << endl;
                                cout << endl;
                            }
                            // for member
                            else
                            {
                                cout << "You have given " << p.getPartyNamesAt(user_choice_party_member - 1) << " a (+3) Vorpal Longsword" << endl;
                                cout << endl;
                            }

                        }
                        
                        // set gold
                        p.setNumGold(p.getNumGold() - total);
                        
                        cout << "Thanks for your business. What else can I get for you?" << endl;
                    }
            }
            // invalid weapon choice
            while(weapon_choice < 0 || weapon_choice > 6)
            {
                cout << "Please enter valid choice" << endl;
                cin >> weapon_choice;
            }
        }
        // buy armor 
        else if(merchant_choice==4)
        {   
            // get quantity
             cout << "How many suits of armor can I get you? (Enter a positive integer, or 0 to cancel)" << endl;
             int num_armor;
             cin >> num_armor;

            // invalid quantity
             while(num_armor < 0)
             {
                cout << "Please enter a postive integer or 0 to cancel" << endl;
                cin >> num_armor;
             }

            // check they have enough gold
             while(num_armor * (5*((player.getNumRooms()*0.25)+1)) > p.getNumGold())
             {
                cout << "Sorry you do not have enough gold. Try another amount of armor or 0 to cancel" << endl;
                cin >> num_armor;
             }
            
            // confirm purchase
             cout << "You want to buy " << num_armor << " suit(s) of armor for " << (num_armor * (((player.getNumRooms()*0.25)+1) *5)) << " gold? (Y/N)" << endl;
             char armor_answer;
             cin >> armor_answer;

             if(armor_answer == 'y' || armor_answer == 'Y')
             {
                // set num of armor
                p.setArmor(num_armor);
                // set gold
                p.setNumGold(p.getNumGold() - (num_armor * (5*((player.getNumRooms()*0.25)+1))));
             }

        }
        // sell treasure
        else if(merchant_choice ==5)
        {
            // print treasure menu
             cout << "You have chosen to sell treasure" << endl;
             cout << "What tresure would you like to sell?" << endl;
             cout << "1. Silver Ring" << endl;
             cout << "2. Ruby Necklace" << endl;
             cout << "3. Emerald Ring" << endl;
             cout << "4. Diamond Circlet" << endl;
             cout << "5. Gem-Encrusted Diamond" << endl;
             cout << "6. Cancel" << endl;
             int treasure_option;
             cin >> treasure_option;

            // inalvid input
             while(treasure_option > 6 || treasure_option < 1)
             {
                cout << "Invalid input" << endl;
                cin >> treasure_option;
             }

            // make sure they have treasure
             while(treasure_option != 6 && p.getTreasureAt(treasure_option-1) < 1)
             {
                cout << "You dont have any of this treasure try another or enter 6 to cancel" << endl;
                cin >> treasure_option;
             }

            // valid input and they have treasure
             if(treasure_option < 6 && p.getTreasureAt(treasure_option-1) > 0)
             {
                    // silver ring
                    if(treasure_option == 1)
                    {
                        cout << "You have sold " << p.getTreasureAt(0) << " silver ring(s) for " << (p.getTreasureAt(0) * 5) << endl;
                        // set gold
                        p.setNumGold(p.getNumGold() + (p.getTreasureAt(0) * 5));
                    }
                    // ruby necklace
                    else if(treasure_option == 2)
                    {
                        cout << "You have sold " << p.getTreasureAt(1) << " Ruby Necklace(s) for " << (p.getTreasureAt(1) * 10) << endl;
                        // set gold
                        p.setNumGold(p.getNumGold() + (p.getTreasureAt(1) * 10));
                    }
                    // emerald ring
                    else if(treasure_option == 3)
                    {
                        cout << "You have sold " << p.getTreasureAt(2) << " Emerald Ring(s) for " << (p.getTreasureAt(2) * 15) << endl;
                        // set gold
                        p.setNumGold(p.getNumGold() + (p.getTreasureAt(2) * 15));
                    }
                    // diamond circlet
                    else if(treasure_option == 4)
                    {
                        cout << "You have sold " << p.getTreasureAt(3) << " Diamond Circlet(s) for " << (p.getTreasureAt(3) * 20) << endl;
                        // set gold
                        p.setNumGold(p.getNumGold() + (p.getTreasureAt(3) * 20));
                    }
                    // gem-encrusted goblet
                    else if(treasure_option == 5)
                    {
                        cout << "You have sold " << p.getTreasureAt(4) << " Gem-Encrusted Goblet(s) for " << (p.getTreasureAt(4) * 25) << endl;
                        // set gold
                        p.setNumGold(p.getNumGold() + (p.getTreasureAt(4) * 25));
                    }
             }
        }
        // invalid choice for merchant menu
        else 
        {
            cout << "Please enter a valid option" << endl;
            cin >> merchant_choice;
        }
    // after each purchase display updated inventory
    printInventory();
    printMerchantMenu();
    cin >> merchant_choice;
    }    
    

    // user wants to leave merchant
    if(merchant_choice == 6)
    {
        cout << "Best of luck to you all!" << endl;
    }
    // invalid merchant choice
    while(merchant_choice > 6 || merchant_choice < 0)
    {
        cout << "Please enter valid option" << endl;
        cin >> merchant_choice;
    }
}

/**
 * Algorithm uses display map function in map class to display map in driver file
 */
void Game::printMap()
{
    m.displayMap();
}

/**
 * Algorithm uses addRoom function in map class to add 5 rooms
 * 
 */
void Game::addRooms()
{
    // only add 5 rooms
    while(m.getRoomCount() < 5)
    {
        m.addRoom();
    }
}

/**
 * Algorithm uses addNPC function in map class to add 5 NPCs
 */
void Game::addNPCs()
{
    // only add 5 npcs
    while(m.getNPCCount() < 5)
    {
        m.addNPC();
    }
}

/**
 * Algorithm allows user to pick which option they want to do on a free space, and returns true or false depending if user wants to quit or not
 * 1. Print menu
 * If user chooses 1, allow them to move any cardinal direction using a, s, w, or d keys and make sure its on map using functions from map class
 * If user chooses 2, explore space if it is unexplored, then 10% chance fro key, 20% for treasure, 20% for monster and 50% for nothing, with each member having 50% chance to lose 1 fullness
 * If user chooses 3, spawn random monster of correct level and use fightMonster() function and 40% chance for misfortune
 * If user chooses 4, allow them to cook food in multiples of 5 and each player gets 1 fullness for every 5 ingredients
 * If user chooses 5, return false and end game
 * Return: True for user quit or false for user to continue 
 */
bool Game::emptySpaceChoice()
{
    cout << "What would you like to do?" << endl;
    cout << "Select one:" << endl;
    cout << "1. Move" << endl;
    cout << "2. Investigate" << endl;
    cout << "3. Pick a Fight" << endl;
    cout << "4. Cook and Eat" << endl;
    cout << "5. Give up" << endl;
    int choice;
    cin >> choice;

    // invalid input
    while(choice > 5 || choice < 1)
    {
        cout << "Please enter valid input between 1 and 5" << endl;
        cin >> choice;
    }

    // move
    if(choice == 1)
    {
        cout << "You have chosen to MOVE. Please enter either a,s,d, or w to move a certain direction." << endl;
        char move;
        cin >> move;

        // check valid key to move
        while(!(move == 'a' || move == 's' || move == 'w' || move == 'd' ))
        {
            cout << "Enter a valid key for direction, with no capital letters" << endl;
            cin >> move;
        }

        // check move will be on map
        while((m.isOnMap(m.getPlayerRow(), m.getPlayerCol() - 1) == false && move == 'a') || (m.isOnMap(m.getPlayerRow() + 1, m.getPlayerCol()) == false && move == 's') || (m.isOnMap(m.getPlayerRow() - 1, m.getPlayerCol()) == false && move == 'w') || (m.isOnMap(m.getPlayerRow(), m.getPlayerCol() + 1) == false && move == 'd'))
        {
                cout << "This is not a valid move on the map. Please go another direction" << endl;
                cin >> move;
        }

        // move left 1 space
        if(move == 'a')
        {
            m.setPlayerPosition(m.getPlayerRow(), m.getPlayerCol() - 1);
            
        }
        // move down 1 space
        else if(move == 's')
        {
            m.setPlayerPosition(m.getPlayerRow() + 1, m.getPlayerCol());
            
        }
        // move up one space
        else if(move == 'w')
        {

            m.setPlayerPosition(m.getPlayerRow() - 1, m.getPlayerCol());

        }
        // move right 1 space
        else if (move == 'd')
        {
            
            m.setPlayerPosition(m.getPlayerRow(), m.getPlayerCol() + 1);
    
        }
        
        // 50% chance for fullness -1
        int random_num;

        for(int i = 0; i < p.getNumParty(); i++)
        {
            random_num = 0;
            random_num = (rand()%10) + 1;

            if(random_num == 1 || random_num == 2)
            {
                player.setFullness((player.getFullness(i) - 1), i);
            }
        }
        
        // add 1 to sorcer anger
        player.setAnger();
        cout << endl;
        cout << "-----------------------------------------" << endl;
        cout << endl;
        // print status of party and new map
        printStatus();
        printInventory();
        partyStatus();
        printMap();

        return false;
    }
    // if space is already explored
    else if(choice == 2 && m.isExplored(m.getPlayerRow(), m.getPlayerCol()) == true)
    {
        cout<< "You have already explored this space. Pick another option" << endl;
        cin >> choice;
    }
    // space is unexplored
    else if(choice == 2 && m.isExplored(m.getPlayerRow(), m.getPlayerCol()) == false)
    {
        cout << "You have chosen to INVESTIGATE" << endl;
        srand(time(0));
        int random = (rand()%10) + 1;
        
        // 10% for key
        if(random == 1)
        {
            cout << "You have found a key!" << endl;
            // add key
            player.setNumKeys(player.getNumKeys() + 1);
            
            // 50% fullness -1
            for(int i = 0; i < p.getNumParty(); i++)
            {
                int random_num = 0;
                srand(time(0));
                random_num = (rand()%10) + 1;

                if(random_num == 1 || random_num == 2 || random_num == 3 || random_num == 4 || random_num == 5)
                {
                    player.setFullness((player.getFullness(i) - 1), i);
                }
            }
        }
        // 20% chance for treasure
        else if(random == 2 || random == 3)
        {
            cout << "You have found treasure. You can sell treasure to merchants for extra gold!" << endl;
            // get num of rooms
            int num_rooms = player.getNumRooms();
            // for 0 rooms
            if(num_rooms == 0)
            {
                cout << "1 Silver Ring has been added to your inventory" << endl;
                p.setTreasureAt(0);
            }
            // for 1 room
            else if(num_rooms == 1)
            {
                cout << "1 Silver Ring has been added to your inventory" << endl;
                p.setTreasureAt(0);
            }
            // for 2 rooms
            else if(num_rooms == 2)
            {
                cout << "1 Ruby Necklace has been added to your inventory" << endl;
                p.setTreasureAt(1);
            }
            // for 3 rooms
            else if(num_rooms == 3)
            {
                cout << "1 Emerald Ring has been added to your inventory" << endl;
                p.setTreasureAt(2);
            }
            // for 4 rooms
            else if(num_rooms == 4)
            {
                cout << "1 Diamond Circlet has been added to your inventory" << endl;
                p.setTreasureAt(3);
            }
            // for 5 rooms
            else if(num_rooms >= 5)
            {
                cout << "1 Gem-Encrusted Goblet has been added to your inventory" << endl;
                p.setTreasureAt(4);
            }

            // 50% chance fullness -1
            for(int i = 0; i < p.getNumParty() + 1; i++)
            {
                int random_num = 0;
                srand(time(0));
                random_num = (rand()%10) + 1;

                if(random_num == 1 || random_num == 2 || random_num == 3 || random_num == 4 || random_num == 5)
                {
                    player.setFullness((player.getFullness(i) - 1), i);
                }
            }
        }
        // 20% chance for monster
        else if(random == 4 || random == 5)
        {
            cout << "You have found a monster in the darkness. Prepare to fight!" << endl;
            // function for fighting
            monsterFight();
        }
        // 50% for nothing
        else
        {
            cout << "You found NOTHING!" << endl;
        }

        // 40% chance of misfortune
        srand(time(0));
        int misfortune_ = (rand() % 10 )+1;

        if(misfortune_ <= 4)
        {
            // function to get random misfortune
            getMisfortune();
        }

        cout << endl;
        cout << "-----------------------------------------" << endl;
        cout << endl;
        // print status and map
        printStatus();
        printInventory();
        partyStatus();
        // change space to explored
        m.exploreSpace(m.getPlayerRow(), m.getPlayerCol());
        printMap();
        return false;

    }
    // fight monster
    else if(choice == 3)
    {
        cout << "You have choosen to FIGHT! Prepare for war!" << endl;
        // use function to get random monster and fight
        monsterFight();

        // 40% chance of misfortune
        srand(time(0));
        int misfortune_ = (rand() % 10 )+1;

        if(misfortune_ <= 4)
        {
            getMisfortune();
        }
        // print status and map
        printStatus();
        printInventory();
        partyStatus();
        printMap();
        return false;
    }
    // cook and eat
    else if(choice == 4)
    {
        // get cookware to use
        cout << "You have chosen to COOK and EAT." << endl;
        cout<< "Which cookware would you like to use? ('P' for cermaic pot, 'F' for frying pan, 'C' for cauldron,m or 'Q' to quit)" << endl;
        char cookware_use;
        cin >> cookware_use;

        // invalid cookware
        while(!(cookware_use == 'P' || cookware_use == 'F' || cookware_use == 'C' || cookware_use == 'Q'))
        {
            cout << "Please enter valid input. Make sure to capitalize your choice as well" << endl;
            cin >> cookware_use;
        }
        // ceramic pot
        if(cookware_use == 'P')
        {

            int num_cookware = p.getCookwareAt(0);

            // if no cermaic pots
            if(num_cookware == 0)
            {
                cout << "You dont have this cookware item. Select another cookware item or 'Q' to quit" << endl;
                cin >> cookware_use;
            }

            // get amoutn of ingredients
            cout << "How many ingredients would you like to cook? (Enter multiple of 5 or 0 to cancel out)." << endl;
            int num_ingredients_cook;
            cin >> num_ingredients_cook;

            // check multiple of 5
            while((num_ingredients_cook % 5) != 0)
            {
                cout << "Please enter a multiple of 5 or 0 to cancel" << endl;
                cin >> num_ingredients_cook;
            }

            // check they have enough ingredients
            while(num_ingredients_cook > p.getNumIngredients())
            {
                cout << "You dont have enough ingredients. Enter a lower amount please" << endl;
                cin >> num_ingredients_cook;
            }

            
            // cook ingredients with cookware
            cout << "Cooking " << num_ingredients_cook << " ingredients using a " << "ceramic pot..." << endl;

            // 25% chance pot breaks
            srand(time(0));
            int random_num_cook = (rand() % 100) + 1;
            if(random_num_cook <= 25)
            {
                cout << "The cook FAILED!" << endl;
                cout << "You have lost your cermaic pot and your ingredients." << endl;
                // lose ingredients and pot
                p.setNumIngredients(p.getNumIngredients() - num_ingredients_cook);
                if(p.getCookwareAt(0) > 1)
                {
                    p.setCookwareAt(p.getCookwareAt(0)-1, 0);
                }

            }
            // success
            else
            {
                cout << "The cook was a SUCCESS!" << endl;
                cout << "Each player in you party and your fullness has increased by " << (num_ingredients_cook / 5) << endl;
                // lose ingredients
                p.setNumIngredients(p.getNumIngredients() - num_ingredients_cook);
                // for each member add 1 fullness for every 5 ingredients
                for(int i = 0; i < p.getNumParty() + 1; i++)
                {
                    if(player.getFullness(i) + (num_ingredients_cook / 5) >= 50)
                    {
                        player.setFullness(50, i);
                    }
                    else
                    {
                    player.setFullness(player.getFullness(i) + (num_ingredients_cook / 5), i);
                    }

                }
            }
        }
        // frying pan
        else if(cookware_use == 'F')
        {
            int num_cookware = p.getCookwareAt(1);
            // valid cookware amount
            while(num_cookware == 0)
            {
                cout << "You dont have this cookware item. Select another cookware item or 'Q' to quit" << endl;
                cin >> cookware_use;
            }
            
            // amount of ingredients
            cout << "How many ingredients would you like to cook? (Enter multiple of 5)." << endl;
            int num_ingredients_cook;
            cin >> num_ingredients_cook;

            // multiple of 5
            while((num_ingredients_cook % 5) != 0)
            {
                cout << "Please enter a multiple of 5 or 0 to cancel" << endl;
                cin >> num_ingredients_cook;
            }

            // enough ingredients
            while(num_ingredients_cook > p.getNumIngredients())
            {
                cout << "You dont have enough ingredients. Enter a lower amount please" << endl;
                cin >> num_ingredients_cook;
            }

            // quit cooking
            if(num_ingredients_cook == 0)
            {
                return false;
            }

            cout << "Cooking " << num_ingredients_cook << " ingredients using a " << "frying pan..." << endl;

            // 10% chance breaking
            srand(time(0));
            int random_num_cook = (rand() % 100) + 1;
            if(random_num_cook <= 10)
            {
                cout << "The cook FAILED!" << endl;
                cout << "You have lost your frying pan and your ingredients." << endl;
                // lose ingredients and cookware
                p.setNumIngredients(p.getNumIngredients() - num_ingredients_cook);
                if(p.getCookwareAt(1) > 1)
                {
                    p.setCookwareAt(p.getCookwareAt(1)-1, 0);
                }
                
            }
            // success
            else
            {
                cout << "The cook was a SUCCESS!" << endl;
                cout << "Each player in you party and your fullness has increased by " << (num_ingredients_cook / 5) << endl;
                // lose ingredients and give each member 1 fullness every 5 ingredients
                p.setNumIngredients(p.getNumIngredients() - num_ingredients_cook);
                for(int i = 0; i < p.getNumParty(); i++)
                {
                    if(player.getFullness(i) + (num_ingredients_cook / 5) >= 50)
                    {
                        player.setFullness(50, i);
                    }
                    else
                    {
                        player.setFullness(player.getFullness(i) + (num_ingredients_cook / 5), i);
                    }

                }
                
            }
            
        }
        // cauldron
        else if(cookware_use == 'C')
        {

            int num_cookware = p.getCookwareAt(2);
            // check if they have cookware
            if(num_cookware == 0)
            {
                cout << "You dont have this cookware item. Select another cookware item or 'Q' to quit" << endl;
                cin >> cookware_use;
            }

            // get amount of ingredients
            cout << "How many ingredients would you like to cook? (Enter multiple of 5)." << endl;
            int num_ingredients_cook;
            cin >> num_ingredients_cook;

            // multiple of 5 for ingredients
            while((num_ingredients_cook % 5) != 0)
            {
                cout << "Please enter a multiple of 5 or 0 to cancel" << endl;
                cin >> num_ingredients_cook;
            }

            // have enough ingredients
            while(num_ingredients_cook > p.getNumIngredients())
            {
                cout << "You dont have enough ingredients. Enter a lower amount please" << endl;
                cin >> num_ingredients_cook;
            }


            cout << "Cooking " << num_ingredients_cook << " ingredients using a " << "cauldron..." << endl;

            // 2% chance of breaking
            srand(time(0));
            int random_num_cook = (rand() % 100) + 1;
            // fail
            if(random_num_cook <= 2)
            {
                cout << "The cook FAILED!" << endl;
                cout << "You have lost your cauldron and your ingredients." << endl;
                // lose ingredients and cookware
                p.setNumIngredients(p.getNumIngredients() - num_ingredients_cook);
                if(p.getCookwareAt(2) > 1)
                {
                    p.setCookwareAt(p.getCookwareAt(2)-1, 0);
                }
               
            }
            // success
            else
            {
                cout << "The cook was a SUCCESS!" << endl;
                cout << "Each player in you party and yourselfs fullness has increased by " << (num_ingredients_cook / 5) << endl;
                // lose ingredients add 1 fullness every 5 ingredients
                p.setNumIngredients(p.getNumIngredients() - num_ingredients_cook);
                for(int i = 0; i < p.getNumParty(); i++)
                {
                    if(player.getFullness(i) + (num_ingredients_cook / 5) >= 50)
                    {
                        player.setFullness(50, i);
                    }
                    else
                    {
                        player.setFullness(player.getFullness(i) + (num_ingredients_cook / 5), i);
                    }

                }
                
            }
            
        }
        // cancel cook and eat
        else if(cookware_use == 'Q')
        {
            cout << "Cancelled" << endl;
        }
        
        // 40% chance of misfortune
        srand(time(0));
        int misfortune_ = (rand() % 10 )+1;

        if(misfortune_ <= 4)
        {
            getMisfortune();
        }

        cout << endl;
        cout << "-----------------------------------------" << endl;
        cout << endl;
        // print out new inventory and map
        printStatus();
        printInventory();
        partyStatus();
        printMap();
        return false;
    }
    // quit game
    else if(choice == 5)
    {
        cout << "We're deeply saddened you could not defeat the sorcerer" << endl;
        // return true for quit
        return true;
    }

    return false;
}

/**
 * Algorithm is used for user choice when they are on an NPC space in driver file
 * 1. Get user choice
 * If choice is 1, move off NPC space but display it as NPC space
 * If choice is 2, NPC ask riddle, if they get it correct they can buy stuff, if incorrect spawns random monster
 * If choice is 3, return true and end game
 * Return: True for game end or false for game continue
 */
bool Game::npcChoice()
{
    // NPC menu choice
    cout << "You are on an NPC space. What would you like to do?" << endl;
    cout << "Select one:" << endl;
    cout << "1. Move" << endl;
    cout << "2. Speak to NPC" << endl;
    cout << "3. Give up" << endl;
    int npc_choice;
    cin >> npc_choice;
    // invalid input
    while(npc_choice < 1 || npc_choice > 3)
    {
        cout << "Please enter valid input." << endl;
        cin >> npc_choice;
    }

    // move off space
    if(npc_choice == 1)
    {
            // get move
            cout << "You have chosen to MOVE. Please enter either a,s,d, or w to move a certain direction." << endl;
            char move;
            cin >> move;

        // make sure move is a,s,w, or d
        while(!(move == 'a' || move == 's' || move == 'w' || move == 'd' ))
        {
            cout << "Enter a valid key for direction, with no capital letters" << endl;
            cin >> move;
        }

        // make sure move will be on map
        while((m.isOnMap(m.getPlayerRow(), m.getPlayerCol() - 1) == false && move == 'a') || (m.isOnMap(m.getPlayerRow() + 1, m.getPlayerCol()) == false && move == 's') || (m.isOnMap(m.getPlayerRow() - 1, m.getPlayerCol()) == false && move == 'w') || (m.isOnMap(m.getPlayerRow(), m.getPlayerCol() + 1) == false && move == 'd'))
        {
            cout << "This is not a valid move on the map. Please go another direction" << endl;
            cin >> move;
        }

        // move left 1 space
        if(move == 'a')
        {
            m.setPlayerPosition(m.getPlayerRow(), m.getPlayerCol() - 1);
            
        }
        // move down 1 space
        else if(move == 's')
        {
            m.setPlayerPosition(m.getPlayerRow() + 1, m.getPlayerCol());
            
        }
        // move up 1 space
        else if(move == 'w')
        {

            m.setPlayerPosition(m.getPlayerRow() - 1, m.getPlayerCol());

        }
        // move left 1 space
        else if (move == 'd')
        {
            
            m.setPlayerPosition(m.getPlayerRow(), m.getPlayerCol() + 1);
    
        }
        
        // 20% chance fullness -1
        int random_num;
        for(int i = 0; i < p.getNumParty() + 1; i++)
        {
            random_num = 0;
            srand(time(0));
            random_num = (rand()%10) + 1;

            if(random_num == 1 || random_num == 2)
            {
                player.setFullness((player.getFullness(i) - 1), i);
            }
        }

        // reveal NPC space
        m.exploreSpace(m.getPlayerRow(), m.getPlayerCol());
        // add 1 to sorcerer anger
        player.setAnger();
        // print status and new map
        printStatus();
        printInventory();
        partyStatus();
        printMap();

        return false;

    }
    // interact with NPC
    else if(npc_choice == 2)
    {
        // get riddle
        string riddle;
        riddle = c.getRiddle();
        cout << riddle << " (have no spaces or capital letters in your answer. All answers are 1 word)" << endl;
    
        //get user answer
        string answer;
        cin >> answer;

        // check answer
        // if answer correct print menu and allow them to buy
        if(c.riddleAnswer(riddle, answer) == true)
        {
            meetMerchant();
        }
        // if answer is wrong summon monster
        else
        {
            // function to fight random monster
            monsterFight();
        }

        // change NPC space to explored
        m.removeNPC(m.getPlayerRow(), m.getPlayerCol());
        return false;
    }
    // user wants to quit
    else if(npc_choice == 3)
    {
        cout << "We're deeply saddened you could not defeat the sorcerer" << endl;
        return true;
    }

    return false;
}

/**
 * Algorithm is used in driver file to display menu of choices for room space and allow user to pick an option
 * If user chooses 1, move off room space any cardinal direction
 * If user chooses 2, check if they have key and open door if they do or play game to open door without key
 * If user choose 3, return true to end game
 * Return: True for end game and False to continue game
 */
bool Game:: roomChoice()
{
    // room menu
    cout << "You are on a room space, what would you like to do?" << endl;
    cout << "Select one:" << endl;
    cout << "1. Move" << endl;
    cout << "2. Open door" << endl;
    cout << "3. Give up" << endl;
    int room_choice;
    cin >> room_choice;

    // invalid input
    while(room_choice > 3 || room_choice < 1)
    {
        cout << "Please enter valid input" << endl;
        cin >> room_choice;
    }

    // move 
    if(room_choice == 1)
    {
        // get move
        cout << "You have chosen to MOVE. Please enter either a,s,d, or w to move a certain direction." << endl;
            char move;
            cin >> move;

        // make sure move is a,s,w, or d
        while(!(move == 'a' || move == 's' || move == 'w' || move == 'd' ))
        {
            cout << "Enter a valid key for direction, with no capital letters" << endl;
            cin >> move;
        }

        // make sure move will be on map
        while((m.isOnMap(m.getPlayerRow(), m.getPlayerCol() - 1) == false && move == 'a') || (m.isOnMap(m.getPlayerRow() + 1, m.getPlayerCol()) == false && move == 's') || (m.isOnMap(m.getPlayerRow() - 1, m.getPlayerCol()) == false && move == 'w') || (m.isOnMap(m.getPlayerRow(), m.getPlayerCol() + 1) == false && move == 'd'))
        {
            cout << "This is not a valid move on the map. Please go another direction" << endl;
            cin >> move;
        }

        // move right 1 space
        if(move == 'a')
        {
            m.setPlayerPosition(m.getPlayerRow(), m.getPlayerCol() - 1);
            
        }
        // move down 1 space
        else if(move == 's')
        {
            m.setPlayerPosition(m.getPlayerRow() + 1, m.getPlayerCol());
            
        }
        // move up 1 space
        else if(move == 'w')
        {

            m.setPlayerPosition(m.getPlayerRow() - 1, m.getPlayerCol());

        }
        // move left 1 space
        else if (move == 'd')
        {
            
            m.setPlayerPosition(m.getPlayerRow(), m.getPlayerCol() + 1);
    
        }
        
        // 20% chance fullness-1
        int random_num;
        for(int i = 0; i < p.getNumParty()+ 1; i++)
        {
            random_num = 0;
            srand(time(0));
            random_num = (rand()%10) + 1;

            if(random_num == 1 || random_num == 2)
            {
                player.setFullness((player.getFullness(i) - 1), i);
            }
        }

        // add 1 to sorcer anger
        player.setAnger();

    }
    // open door
    else if(room_choice == 2)
    {
        cout << "You have chosen to OPEN THE DOOR" << endl;

        // check if party has key
        if(player.getNumKeys() < 1)
        {
            // game for no key
            cout << "You do not have a key and have fallen into a trap! You must win a game of Boulder/Parchment/Shears to open the door or risk losing a party member!" << endl;
            cout << "Select your choice:" << endl;
            cout << "1. Boulder" << endl;
            cout << "2. Parchment" << endl;
            cout << "3. Shears" << endl;
            cout << endl;
            int room_game_choice;
            cin >> room_game_choice;

            // invalid input
            while (room_game_choice < 1 || room_game_choice > 3)
            {
                cout << "Enter a valid input" << endl;
                cin >> room_game_choice;
            }
            
            // random number 1-3 for game
            int random_game_generation;
            srand(time(0));
            random_game_generation = (rand() % 3) + 1;

            // if random num and user num are equal TIE
            while(random_game_generation == room_game_choice)
            {
                cout << "Its a TIE! Enter another choice:" << endl;
                cout << "1. Boulder" << endl;
                cout << "2. Parchment" << endl;
                cout << "3. Shears" << endl;
                cout << endl;
                cin >> room_game_choice;
            }
            int lose_counter = 0;
            bool win = false;
            // while user doesnt have win or 3 losses
            while(lose_counter < 3 || win == false)
            {
                // boulder vs parchment
                if(room_game_choice == 1 && random_game_generation == 2)
                {
                    // add loss
                    lose_counter ++;
                    cout << "You have LOST to Parchement! You have " << lose_counter << " losses." << endl;
                }
                // boulder vs shears
                else if(room_game_choice == 1 && random_game_generation == 3)
                {
                    cout << "You have WON against Shears! You may now enter the room." << endl;
                    // win true
                    win = true;
                }
                // parchment vs boulder
                else if(room_game_choice == 2 && random_game_generation == 1)
                {
                    cout << "You have WON against Boulder! You may now enter the room." << endl;
                    // win true
                    win = true;
                }
                // parchment vs shears
                else if(room_game_choice == 2 && random_game_generation == 3)
                {
                    // add loss
                    lose_counter ++;
                    cout << "You have LOST to Shears! You have " << lose_counter << " losses."  << endl;
                }
                // shears vs boulder
                else if(room_game_choice == 3 && random_game_generation == 1)
                {
                    // add loss
                    lose_counter ++;
                    cout << "You have LOST to Boulder! You have " << lose_counter << " losses."  << endl;
                }
                // shears vs parchment
                else if(room_game_choice == 3 && random_game_generation == 2)
                {
                    cout << "You have WON against Parchment! You may now enter the room." << endl;
                    // win true
                    win = true;
                }

                // if user has 3 losses
                if(lose_counter == 3)
                {
                    cout << "You have 3 losses! You will now randomly lose a party member" << endl;
                    // use function to remove random party member
                    int num_party_members = p.getNumParty();
                    // if only 1 other party member end game
                    if(num_party_members == 1)
                    {
                        cout << "Unfortunately all other party members have died. You have Lost! Better luck next time." << endl;
                        p.setNumParty(0);
                    }
                    else
                    {
                        // print which party member was sacrificed
                        string name = removePartyMember();
                        cout << name << " has been sacrificed!" << endl;
                    }
                }

                // if user wins
                if(win == true)
                {
                    // open door
                    cout << "You have opened the door!" << endl;
                    // if final room
                    if(player.getNumRooms() == 4)
                    {
                        finalMonsterFight();
                    }
                    // not final room
                    else
                    {
                        monsterFightRoom();
                    }
                }
            }
        }
        // player has key
        else if(player.getNumKeys() >= 1)
        {
            // if final room
            if(player.getNumRooms() == 4)
            {
                finalMonsterFight();
            }
            // not final room
            else
            {
                monsterFightRoom();
            }
        }
    
    }
    // user chooses to give up
    else if(room_choice == 3)
    {
        cout << "We're deeply saddened you could not defeat the sorcerer" << endl;
        return true;
    }

    cout << endl;
        cout << "-----------------------------------------" << endl;
        cout << endl;
        // print statu and new map
        printStatus();
        printInventory();
        partyStatus();
        printMap();
        return false;
}

/**
 * Algorithm is used in driver file for if user is on exit, but doesn't have 5 rooms cleared
 * 1. Ask user to move off exit
 * 2. Get user move and move 1 space if valid
 */
void Game:: exitChoice()
{   
    // get move
    cout << "Please enter either a,s,d, or w to move a certain direction." << endl;
    char move;
    cin >> move;

        // if not a,s,w, or d
        while(!(move == 'a' || move == 's' || move == 'w' || move == 'd' ))
        {
            cout << "Enter a valid key for direction, with no capital letters" << endl;
            cin >> move;
        }

        // check if move is on map
        while((m.isOnMap(m.getPlayerRow(), m.getPlayerCol() - 1) == false && move == 'a') || (m.isOnMap(m.getPlayerRow() + 1, m.getPlayerCol()) == false && move == 's') || (m.isOnMap(m.getPlayerRow() - 1, m.getPlayerCol()) == false && move == 'w') || (m.isOnMap(m.getPlayerRow(), m.getPlayerCol() + 1) == false && move == 'd'))
        {
            cout << "This is not a valid move on the map. Please go another direction" << endl;
            cin >> move;
        }

        // move left 1 space
        if(move == 'a')
        {
            m.setPlayerPosition(m.getPlayerRow(), m.getPlayerCol() - 1);
            
        }
        // move down 1 space
        else if(move == 's')
        {
            m.setPlayerPosition(m.getPlayerRow() + 1, m.getPlayerCol());
            
        }
        // move up 1 space
        else if(move == 'w')
        {

            m.setPlayerPosition(m.getPlayerRow() - 1, m.getPlayerCol());

        }
        // move right 1 space
        else if (move == 'd')
        {
            
            m.setPlayerPosition(m.getPlayerRow(), m.getPlayerCol() + 1);
    
        }
        
        // 20% chance for fullness-1
        int random_num;
        for(int i = 0; i < p.getNumParty()+ 1; i++)
        {
            random_num = 0;
            srand(time(0));
            random_num = (rand()%10) + 1;

            if(random_num == 1 || random_num == 2)
            {
                player.setFullness((player.getFullness(i) - 1), i);
            }
        }

        // sorcer anger + 1
        player.setAnger();

}

/**
 * Function from map class used in driver function to check what space player is on
 */
bool Game:: isFreeSpace()
{
     if (!m.isOnMap(m.getPlayerRow(), m.getPlayerCol()))
    {
        return false;
    }
    if (m.isNPCLocation(m.getPlayerRow(), m.getPlayerCol()))
    {
        return false;
    }
    if (m.isRoomLocation(m.getPlayerRow(), m.getPlayerCol()))
    {
        return false;
    }
    if (m.isDungeonExit(m.getPlayerRow(), m.getPlayerCol()))
    {
        return false;
    }
    return true;
}

/**
 * Function from map class used in driver function to check what space player is on
 */
bool Game:: isNPCLocation()
{
    return m.isNPCLocation(m.getPlayerRow(),m.getPlayerCol());
}

/**
 * Function from map class used in driver function to check what space player is on
 */
bool Game:: isRoomLocation()
{
    return m.isRoomLocation(m.getPlayerRow(), m.getPlayerCol());
}

/**
 * Function from map class used in driver function to check what space player is on
 */
bool Game:: isDungeonExit()
{
    return m.isDungeonExit(m.getPlayerRow(), m.getPlayerCol());
}

/**
 * Algorithm uses getter function from player class to get num of rooms for driver file
 * Return: rooms (int)
 */
int Game:: numRooms()
{
    int rooms;
    // use fucntion from player class
    rooms = player.getNumRooms();
    return rooms;
}

/**
 * Algorithm uses getter function from party class to check if player has at least 1 party member alive
 * Return: num of party (int)
 */
int Game:: numParty()
{
    int num_party;
    // use getter function in party class
    num_party = p.getNumParty();
    return num_party;
}

/**
 * Algorithm uses getter function in player class to get anger in driver file and make sure it is not 100
 * Return: sorcerer anger (int)
 */
int Game:: anger()
{
    // use gettr function in player class
    int anger = player.getAnger();
    return anger;
}

/**
 * Algorithm gets random number between 1 and 3 and gets misfortune based on random num
 * 1. Get random num
 * If num is 1, party loses quarter of gold
 * If num is 2, party loses quater of ingredients
 * If num is 3, each party member loses 5 fullness
 */
void Game::getMisfortune() 
{
    // get random num
    srand(time(0));
    int random = (1 + (rand() % 3));
    // num is 1
    if(random == 1)
    {
        // robbed for gold
       cout << "Your party has been robbed!" << endl;
       cout << "You lost " << (p.getNumGold()/4) << " gold" << endl;
       if(p.getNumGold() - (p.getNumGold()/4) < 0)
       {
            p.setNumGold(0);
       }
       else
       {
        p.setNumGold(p.getNumGold() - (p.getNumGold()/4));
       }
    }
    // num is 2
    else if(random == 2)
    {
        // robbed for ingredients
       cout << "Some of your ingredients have gone bad" << endl;
       cout << "You have lost " << (p.getNumIngredients() / 4) << " ingredients" << endl;
       if(p.getNumIngredients() - (p.getNumIngredients() / 4) < 0)
       {
            p.setNumIngredients(0);
       }
       else
       {
            p.setNumIngredients(p.getNumIngredients() - (p.getNumIngredients() / 4));
       }
    }
    // num is 3
    else if(random == 3)
    {
        // spell losing 5 fullness for each member of party
        cout << "The sorcerer casted a spell making each party member lose 5 fullness!" << endl;
        for(int i = 0; i < p.getNumParty() + 1; i++)
        {
            if((player.getFullness(i) - 5) <= 0)
            {
                removePartyMemberAt(i);
            }
            else
            {
                player.setFullness(player.getFullness(i)-5, i);
            }
        }
    }

}

/**
 * Algorithm uses getter function in player class to get fullness of each player in driver file to check if player dies
 * Parameters: index (int)
 * Return: fullness (int)
 */
int Game:: fullness(int index)
{
    return player.getFullness(index);
}

/**
 * Algorithm uses getter function in party class to get the player name when writing to file
 * Return: player name (string)
 */
string Game:: playerName()
{
    return p.getPlayerName();
}

/**
 * Algorithm calculates party's final score based on how many rooms they cleared, the num of monsters they killed, and if they made it out of the dungeon
 * 1. Get num of rooms cleared
 * 2. Get monster killed
 * 3. Get player name
 * 4. Add 100 pts if party made it out of dungeon
 * Return: final_socre (int) 
 */
int Game:: finalScore()
{
    int final_score;
    // get num of rooms cleared
    int rooms = player.getNumRooms();
    // amount of monsters killed
    int monsters_killed = 21 - c.getNumMonsters();
    // player name
    string player_name = p.getPlayerName();
    
    // calc final score
    final_score = (rooms*20) + (monsters_killed*3);

    // if they escaped dungeon add 100 points
    if(player.getNumRooms() == 5)
    {
        final_score += 100;
    }

    return final_score;
}
