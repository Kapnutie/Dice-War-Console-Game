/* 
 * File:   main.cpp
 * Author: Jacob Davenport
 * Purpose: Project 1: Dice Wars Game
 * Created on October 23, 2018, 3:40 PM
 */

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

//<editor-fold defaultstate="collapsed" desc="Function Prototypes">
//
void printBoard(string, bool, bool, bool, bool, bool, bool, int, int, int, int, int, int);
void attack(bool, bool, bool, bool, bool, bool); //Attack with Prompt
void defend(char, bool, bool, bool, bool, bool, bool); //Defend with Prompt
bool valAtt(char, bool, bool, bool, bool, bool, bool); //Validate Attack
bool valDef(char, char, bool, bool, bool, bool, bool, bool); //Validate Defense
bool winner(char, char, int, int, int, int, int, int, bool, bool, bool, bool, bool, bool); //Decides the winner of each attack
int attDCnt(char, int, int, int, int, int, int);
int roll(int);
int adjCnt(bool, bool, bool, bool, bool, bool, bool);
void fillAttackArray(char *, bool, bool, bool, bool, bool, bool, int);
void fillDefenseArray(char, bool, bool, bool, bool, bool, bool, char *);
//
//</editor-fold>

int main()
{
    //<editor-fold defaultstate="collapsed" desc="Variable Declarations">
    //==========================================================================

    string plyrOne;//Player's Name
    string gmeData;//For use with reading text from files
    ofstream gameDat;
    ifstream gameInf;
    gameDat.open("GameData.txt");
    gameInf.open("GameData.txt");
    float seedval = time(0); //Value for use with srand()
    const short maxDice = 8; //Maximum amount of Dice that can be on a territory
    char attWith, defWith; //Represent the Territories that are attacking and defending each other
    bool validA, validD; //Holds the response of the check to see if the attacker and defender terr. are valid choices
    bool yRed, yBlue, yYellow, yGreen, yPurple, yWhite; //Territories owned by player
    int dRed, dBlue, dYellow, dGreen, dPurple, dWhite; //Dice on each territory
    int attD; //Dice count of attacking territory
    unsigned short random; //Random number used to distribute dice after each attack
    bool turn = true; //Initially set "your turn" to true
    int adjcnts; //Holds the amount of adjacent territories a player has
    int turnCnt = 0; //Turn count
    bool gameovr = false; //Game Over if adjcnts ever equals 6 (Meaning all territories are owned by either player)
    bool attWon; //Holds the response of winner() checks if the attack won
    static int SIZE = 6; //The amount of territories, for use with CPU arrays
    char cpuVldA[SIZE]; //Holds the valid attacking territories the CPU can use
    char cpuVldD[SIZE]; //Holds the valid defending territories the CPU can use
    int choices; //Used to test the CPU arrays, checking to see if there are any valid choices to make

    //Iniitalize to set the territory ownership and dice values
    yRed = true;
    yBlue = false;
    yYellow = true;
    yGreen = false;
    yPurple = true;
    yWhite = false;

    //Random Seed
    srand(static_cast<int>(seedval));

    //Initialize with random dice count between 1 and MaxDice
    cout<<rand() %6;
    cout<<rand() %6;
    cout<<rand() %6;
    cout<<rand() %6;
    dRed = rand() % maxDice + 1;
    dBlue = rand() % maxDice + 1;
    dYellow = rand() % maxDice + 1;
    dGreen = rand() % maxDice + 1;
    dPurple = rand() % maxDice + 1;
    dWhite = rand() % maxDice + 1;

    //==========================================================================
    //</editor-fold>

    //Get the player's name
    cout << "\nEnter your name: ";
    while (!(getline(cin, plyrOne) || cin.get() != '\n'))
    {
        cin.clear();
        cin.ignore(256, '\n');
        cout << "Invalid input!\n";
    }
    gameDat << plyrOne << " it took you..." << endl; //Input into txt file, for later use

    //<editor-fold desc="Main Game Loop">
    //==========================================================================

    do
    {
        turn = true; //Set turn as users. (Human player starts first)

        //Print Board/Map
        printBoard(plyrOne, yRed, yWhite, yGreen, yYellow, yPurple, yBlue, dRed, dWhite, dGreen, dYellow, dPurple, dBlue);
        turnCnt++; //Increment Turn Count

        //<editor-fold defaultstate="collapsed" desc="Player's Turn">
        //

        //<editor-fold defaultstate="collapsed" desc="User Input Prompts">
        //======================================================================

        //Attack prompt
        attack(yRed, yWhite, yGreen, yYellow, yPurple, yBlue);

        //Get attacking territory
        do
        {
            //Validating user input @ valAtt()
            while (!(cin >> attWith) || (toupper(attWith) != 'R' && toupper(attWith) != 'W' && toupper(attWith) != 'G' && toupper(attWith) != 'Y' && toupper(attWith) != 'P' && toupper(attWith) != 'B') || cin.get() != '\n')
            {
                //Redisplay Board/Prompt, with proper error message
                printBoard(plyrOne, yRed, yWhite, yGreen, yYellow, yPurple, yBlue, dRed, dWhite, dGreen, dYellow, dPurple, dBlue);
                cout << "Invalid entry. Enter the first letter of the color you want to attack with!\n";
                attack(yRed, yWhite, yGreen, yYellow, yPurple, yBlue); //Redisplay attack prompt
                cin.clear();
                cin.ignore(256, '\n');
            }
            validA = valAtt(toupper(attWith), yRed, yWhite, yGreen, yYellow, yPurple, yBlue);
            if (validA == false)
            {
                //Redisplay Board/Prompt, with proper error message
                printBoard(plyrOne, yRed, yWhite, yGreen, yYellow, yPurple, yBlue, dRed, dWhite, dGreen, dYellow, dPurple, dBlue);
                cout << "You can only attack with territories you own and which are adjacent to enemy territories!\n";
                attack(yRed, yWhite, yGreen, yYellow, yPurple, yBlue); //Redisplay
            }
        } while (validA == false);
        attWith = toupper(attWith);

        //Which territory is defending? prompt.. Also reprint board and remind user what they chose to attack with.. In case they forgot :P, wow im so nice! xD
        printBoard(plyrOne, yRed, yWhite, yGreen, yYellow, yPurple, yBlue, dRed, dWhite, dGreen, dYellow, dPurple, dBlue);
        
        cout << "Remember, you're attacking with: ";
        switch (attWith)
        {
            case 'R':
                cout << "Red\n";
                break;
            case 'W':
                cout << "White\n";
                break;
            case 'G':
                cout << "Green\n";
                break;
            case 'Y':
                cout << "Yellow\n";
                break;
            case 'P':
                cout << "Purple\n";
                break;
            case 'B':
                cout << "Blue\n";
                break;
        }
        
        //Choose Defending Territory Prompt
        defend(attWith, yRed, yWhite, yGreen, yYellow, yPurple, yBlue);

        //Get defending territory
        do
        {
            while (!(cin >> defWith) || (toupper(defWith) != 'R' && toupper(defWith) != 'W' && toupper(defWith) != 'G' && toupper(defWith) != 'Y' && toupper(defWith) != 'P' && toupper(defWith) != 'B') || cin.get() != '\n')
            {
                //Redisplay Board/Prompt, with proper error message
                printBoard(plyrOne, yRed, yWhite, yGreen, yYellow, yPurple, yBlue, dRed, dWhite, dGreen, dYellow, dPurple, dBlue);
                cout << "Invalid entry. Enter the 1st letter of the territory you want to attack!\n";
                defend(attWith, yRed, yWhite, yGreen, yYellow, yPurple, yBlue); //Redisplay
                cin.clear();
                cin.ignore(256, '\n');
            }
            validD = valDef(attWith, toupper(defWith), yRed, yWhite, yGreen, yYellow, yPurple, yBlue);
            if (validD == false)
            {
                //Redisplay Board/Prompt, with proper error message
                printBoard(plyrOne, yRed, yWhite, yGreen, yYellow, yPurple, yBlue, dRed, dWhite, dGreen, dYellow, dPurple, dBlue); //Redisplay
                cout << "You can only attack adjacent enemy territories!\n";
                defend(attWith, yRed, yWhite, yGreen, yYellow, yPurple, yBlue); //Redisplay
            }
        } while (validD == false);
        defWith = toupper(defWith);

        //======================================================================
        //</editor-fold>

        //<editor-fold defaultstate="collapsed" desc="Roll Dice">
        //======================================================================

        //Roll each player's dice, check if attack won

        //Get attacking territory's dice count
        attD = attDCnt(attWith, dRed, dWhite, dGreen, dYellow, dPurple, dBlue);
        
        //Check if the attack won
        attWon = winner(attWith, defWith, dRed, dWhite, dGreen, dYellow, dPurple, dBlue, yRed, yWhite, yGreen, yYellow, yPurple, yBlue);

        //======================================================================
        //</editor-fold>

        //<editor-fold defaultstate="collapsed" desc="If Attack Won">
        //======================================================================

        if (attWon == true)
        {
            (turn == true) ? cout << "Your Attack Won!\n" : cout << "CPU Attack Won!\n";

            //Attacking territory loses all but 1 dice
            switch (attWith)
            {
                case 'R':
                    dRed = 1;
                    break;
                case 'W':
                    dWhite = 1;
                    break;
                case 'G':
                    dGreen = 1;
                    break;
                case 'Y':
                    dYellow = 1;
                    break;
                case 'P':
                    dPurple = 1;
                    break;
                case 'B':
                    dBlue = 1;
                    break;
            }

            //Defending territory gains all dice but 1 used in the attack
            //If attack dice == 1, then defending territory is set to 1, not 0.
            switch (defWith)
            {
                case 'R':
                    (attD - 1 == 0) ? dRed = 1 : dRed = (attD - 1);
                    yRed = !yRed; //territory control is reversed if defender lost, hence "!yTerritory" 
                    break;
                case 'W':
                    (attD - 1 == 0) ? dWhite = 1 : dWhite = (attD - 1);
                    yWhite = !yWhite;
                    break;
                case 'G':
                    (attD - 1 == 0) ? dGreen = 1 : dGreen = (attD - 1);
                    yGreen = !yGreen;
                    break;
                case 'Y':
                    (attD - 1 == 0) ? dYellow = 1 : dYellow = (attD - 1);
                    yYellow = !yYellow;
                    break;
                case 'P':
                    (attD - 1 == 0) ? dPurple = 1 : dPurple = (attD - 1);
                    yPurple = !yPurple;
                    break;
                case 'B':
                    (attD - 1 == 0) ? dBlue = 1 : dBlue = (attD - 1);
                    yBlue = !yBlue;
                    break;
            }


        }

            //======================================================================
            //</editor-fold>

        //<editor-fold defaultstate="collapsed" desc="If Attack Failed">
            //======================================================================

        else//(if attack lost)
        {
            //attacking territory loses all but 1 dice
            switch (attWith)
            {
                case 'R':
                    dRed = 1;
                    break;
                case 'W':
                    dWhite = 1;
                    break;
                case 'G':
                    dGreen = 1;
                    break;
                case 'Y':
                    dYellow = 1;
                    break;
                case 'P':
                    dPurple = 1;
                    break;
                case 'B':
                    dBlue = 1;
                    break;
            }

            (turn == true) ? cout << "Your Attack Failed!\n" : cout << "CPU Attack Failed!\n"; //Output the results
        }

        //======================================================================
        //</editor-fold>

        //<editor-fold defaultstate="collapsed" desc="Distribute Dice">
        //======================================================================

        //Get the count of adjacent territories of the current player
        adjcnts = adjCnt(turn, yRed, yWhite, yGreen, yYellow, yPurple, yBlue);
        if (adjcnts == 6)gameovr = true; //Check if the game is over
        cout << "====================\n";
        cout << "Distributing Dice...\n";
        cout << "====================\n";

        //Distribute that many dice randomly across the territories they own
        while (adjcnts != 0)
        {
            random = rand() % 6 + 1;
            //<editor-fold defaultstate="collapsed" desc="test cases">
            //

            if (random == 1 && yRed == true && dRed <= maxDice - 1)
            {
                dRed++;
                cout << "\nDice on Red + 1";
                adjcnts--;
            }
            else if (random == 2 && yWhite == true && dWhite <= maxDice - 1)
            {
                dWhite++;
                cout << "\nDice on White + 1";
                adjcnts--;
            }
            else if (random == 3 && yGreen == true && dGreen <= maxDice - 1)
            {
                dGreen++;
                cout << "\nDice on Green + 1";
                adjcnts--;
            }
            else if (random == 4 && yYellow == true && dYellow <= maxDice - 1)
            {
                dYellow++;
                cout << "\nDice on Yellow + 1";
                adjcnts--;
            }
            else if (random == 5 && yPurple == true && dPurple <= maxDice - 1)
            {
                dPurple++;
                cout << "\nDice on Purple + 1";
                adjcnts--;
            }
            else if (random == 6 && yBlue == true && dBlue <= maxDice - 1)
            {
                dBlue++;
                cout << "\nDice on Blue + 1";
                adjcnts--;
            }
            //
            //</editor-fold>
        }
        cout << endl;

        //======================================================================
        //        </editor-fold>

        //
        //</editor-fold>

        //<editor-fold defaultstate="collapsed" desc="CPU's Turn">
        //

        //CPU's turn
        cout << "\n\n=========================\n";
        cout << setw(21) << "COMPUTERS TURN!\n";
        cout << "=========================\n";
        turn = false; //As seen above, turn = false represents the CPU turn is true!

        //Fill CPU's Valid attacking territory array
        memset(cpuVldA, '\0', SIZE); //Set all values to null in the cpuVldA array every loop
        fillAttackArray(cpuVldA, yRed, yWhite, yGreen, yYellow, yPurple, yBlue, SIZE);

        //Check to avoid infinite loop
        choices = 0;
        for (int i = 0; i < SIZE; i++)
        {
            if (cpuVldA[i] == '\0')choices++; //Every time encounters null, increments
        }
        attWith = '\0'; //set attWith to null each loop
        while (attWith == '\0' && choices != SIZE)//ifchoices == SIZE, no valid options inside of the array
        {
            random = rand() % 6;
            attWith = cpuVldA[random];
        }

        //Displays what the CPU chose to attack with
        switch (attWith)
        {

            case 'R':
                cout << "CPU attacked with Red\n";
                break;
            case 'G':
                cout << "CPU attacked with Green\n";
                break;
            case 'W':
                cout << "CPU attacked with White\n";
                break;
            case 'Y':
                cout << "CPU attacked with Yellow\n";
                break;
            case 'P':
                cout << "CPU attacked with Purple\n";
                break;
            case 'B':
                cout << "CPU attacked with Blue\n";
                break;
            default:
                cout << "CPU Had no valid attack option!\n";
                break;
        }

        //Check to see if there are any valid territories the CPU can attack
        if (attWith != '\0')//If CPU was able to pick an attacking territory.. continue..
        {
            //First clears the entire valid defending array to give it a "Clean slate" of sorts
            memset(cpuVldD, '\0', SIZE);

            //Fill valid defending territory array for the CPU
            fillDefenseArray(attWith, yRed, yWhite, yGreen, yYellow, yPurple, yBlue, cpuVldD);

            choices = 0;
            for (int i = 0; i < SIZE; i++)
            {
                if (cpuVldD[i] == '\0')choices++;
            }

            defWith = '\0';
            while (defWith == '\0' && choices != SIZE)//If choices == SIZE, then there are no valid options
            {
                random = rand() % 6;
                defWith = cpuVldD[random];
            }

            //Display what the player defended with against the CPU's attack
            if (defWith != '\0')//If there are no valid territories for the CPU to attack, skips the switch statement
            {
                switch (defWith)
                {
                    case 'R':
                        cout << "You defended with Red\n";
                        break;
                    case 'G':
                        cout << "You defended with Green\n";
                        break;
                    case 'W':
                        cout << "You defended with White\n";
                        break;
                    case 'Y':
                        cout << "You defended with Yellow\n";
                        break;
                    case 'P':
                        cout << "You defended with Purple\n";
                        break;
                    case 'B':
                        cout << "You defended with Blue\n";
                        break;
                }
            }

            //Get Attacking Territories Dice Count
            attD = attDCnt(attWith, dRed, dWhite, dGreen, dYellow, dPurple, dBlue);

            //See if the attack wins
            attWon = winner(attWith, defWith, dRed, dWhite, dGreen, dYellow, dPurple, dBlue, yRed, yWhite, yGreen, yYellow, yPurple, yBlue);

            //If attack Won
            if (attWon == true)
            {
                (turn == true) ? cout << "Your Attack Won!\n" : cout << "CPU Attack Won!\n";//I know

                //Attacking territory loses all but 1 dice
                switch (attWith)
                {
                    case 'R':
                        dRed = 1;
                        break;
                    case 'W':
                        dWhite = 1;
                        break;
                    case 'G':
                        dGreen = 1;
                        break;
                    case 'Y':
                        dYellow = 1;
                        break;
                    case 'P':
                        dPurple = 1;
                        break;
                    case 'B':
                        dBlue = 1;
                        break;
                }

                //Defending territory gains all dice but 1 used in the attack
                //If attack dice == 1, then defending territory is set to 1, not 0.
                //Whoever lost the defense territory control is reversed, hence "!yTerritory" 
                switch (defWith)
                {
                    case 'R':
                        (attD - 1 == 0) ? dRed = 1 : dRed = (attD - 1);
                        yRed = !yRed;
                        break;
                    case 'W':
                        (attD - 1 == 0) ? dWhite = 1 : dWhite = (attD - 1);
                        yWhite = !yWhite;
                        break;
                    case 'G':
                        (attD - 1 == 0) ? dGreen = 1 : dGreen = (attD - 1);
                        yGreen = !yGreen;
                        break;
                    case 'Y':
                        (attD - 1 == 0) ? dYellow = 1 : dYellow = (attD - 1);
                        yYellow = !yYellow;
                        break;
                    case 'P':
                        (attD - 1 == 0) ? dPurple = 1 : dPurple = (attD - 1);
                        yPurple = !yPurple;
                        break;
                    case 'B':
                        (attD - 1 == 0) ? dBlue = 1 : dBlue = (attD - 1);
                        yBlue = !yBlue;
                        break;
                }


            }

                //If attack lost
            else
            {
                //attacking territory loses all but 1 dice
                switch (attWith)
                {
                    case 'R':
                        dRed = 1;
                        break;
                    case 'W':
                        dWhite = 1;
                        break;
                    case 'G':
                        dGreen = 1;
                        break;
                    case 'Y':
                        dYellow = 1;
                        break;
                    case 'P':
                        dPurple = 1;
                        break;
                    case 'B':
                        dBlue = 1;
                        break;
                }

                (turn == true) ? cout << "Your Attack Failed!\n" : cout << "CPU Attack Failed!\n"; //Output the results of attack
            }
        }

        //<editor-fold defaultstate="collapsed" desc="Distribute Dice">
        //======================================================================

        //Get the count of adjacent territories of the current player
        adjcnts = adjCnt(turn, yRed, yWhite, yGreen, yYellow, yPurple, yBlue);
        if (adjcnts == 6)gameovr = true; //Check if the game is actually over
        cout << "====================\n";
        cout << "Distributing Dice...\n";
        cout << "====================\n";
        
        //Distribute that many dice randomly across the territories they own
        while (adjcnts != 0)
        {
            random = rand() % 6 + 1;
            //<editor-fold defaultstate="collapsed" desc="test cases">
            //

            if (random == 1 && yRed == false && dRed <= maxDice - 1)
            {
                dRed++;
                cout << "\nDice on Red + 1";
                adjcnts--;
            }
            else if (random == 2 && yWhite == false && dWhite <= maxDice - 1)
            {
                dWhite++;
                cout << "\nDice on White + 1";
                adjcnts--;
            }
            else if (random == 3 && yGreen == false && dGreen <= maxDice - 1)
            {
                dGreen++;
                cout << "\nDice on Green + 1";
                adjcnts--;
            }
            else if (random == 4 && yYellow == false && dYellow <= maxDice - 1)
            {
                dYellow++;
                cout << "\nDice on Yellow + 1";
                adjcnts--;
            }
            else if (random == 5 && yPurple == false && dPurple <= maxDice - 1)
            {
                dPurple++;
                cout << "\nDice on Purple + 1";
                adjcnts--;
            }
            else if (random == 6 && yBlue == false && dBlue <= maxDice - 1)
            {
                dBlue++;
                cout << "\nDice on Blue + 1";
                adjcnts--;
            }
            //
            //</editor-fold>
        }
        cout << endl;

        //======================================================================
        //        </editor-fold>

        //
        //</editor-fold>

        cout << "\n\nPress Enter To Continue\n";
        cin.ignore();

    } while (gameovr == false);

    //==========================================================================
    //</editor-fold>

    cout << setfill('=');
    cout << setw(30) << "\n";
    cout << setfill(' ') << setw(18) << "Game Over\n" << setfill('=');
    cout << setw(30) << "\n";

    printBoard(plyrOne, yRed, yWhite, yGreen, yYellow, yPurple, yBlue, dRed, dWhite, dGreen, dYellow, dPurple, dBlue);

    gameDat << turnCnt << " turns to finish the game" << endl;

    while (getline(gameInf, gmeData))
    {
        cout << gmeData << endl;
    }
    
    cout<<"\nIt appears you... ";
    (yRed == true)?cout<<"won the game!\n":cout<<"lost the game!\n";

    gameInf.close();
    gameDat.close();
    return 0;
}

//<editor-fold defaultstate="collapsed" desc="Functions">
//

//Attack prompt, also tells you if your choice of attack is possible
void attack(bool red, bool white, bool green, bool yellow, bool purple, bool blue)
{
    cout << "\nEnter the territory you want to attack with: ";
    (red == true && (white == false || green == false)) ? printf("%c[1;30;42m\nR. Red    \n", 27) : printf("%c[1;30;41m\nR. Red    \n", 27);
    (white == true && (red == false || green == false || yellow == false || purple == false)) ? printf("%c[1;30;42mW. White  \n", 27) : printf("%c[1;30;41mW. White  \n", 27);
    (green == true && (red == false || white == false || yellow == false)) ? printf("%c[1;30;42mG. Green  \n", 27) : printf("%c[1;30;41mG. Green  \n", 27);
    (yellow == true && (green == false || white == false || purple == false || blue == false)) ? printf("%c[1;30;42mY. Yellow \n", 27) : printf("%c[1;30;41mY. Yellow \n", 27);
    (purple == true && (white == false || yellow == false || blue == false)) ? printf("%c[1;30;42mP. Purple \n", 27) : printf("%c[1;30;41mP. Purple \n", 27);
    (blue == true && (yellow == false || purple == false)) ? printf("%c[1;30;42mB. Blue   \n", 27) : printf("%c[1;30;41mB. Blue   \n", 27);
    printf("%c[0m", 27);
}

//Attacking prompt, also tells you if the defender is a valid target
void defend(char attckr, bool red, bool white, bool green, bool yellow, bool purple, bool blue)
{
    bool r, w, g, y, p, b;
    cout << "\nEnter the first letter of the territory you want to attack\n";

    (red == false && (attckr == 'G' || attckr == 'W')) ? r = true : r = false;
    (green == false && (attckr == 'R' || attckr == 'W' || attckr == 'Y')) ? g = true : g = false;
    (white == false && (attckr == 'R' || attckr == 'G' || attckr == 'Y' || attckr == 'P')) ? w = true : w = false;
    (yellow == false && (attckr == 'G' || attckr == 'W' || attckr == 'P' || attckr == 'B')) ? y = true : y = false;
    (purple == false && (attckr == 'W' || attckr == 'Y' || attckr == 'B')) ? p = true : p = false;
    (blue == false && (attckr == 'Y' || attckr == 'P')) ? b = true : b = false;


    if (red == true)r = false;
    if (white == true)w = false;
    if (green == true)g = false;
    if (yellow == true)y = false; //base cases, if you own it, you cant attack it
    if (purple == true)p = false;
    if (blue == true)b = false;



    (r == true) ? printf("%c[1;30;42mR. Red    \n", 27) : printf("%c[1;30;41mR. Red    \n", 27);
    (w == true) ? printf("%c[1;30;42mW. White  \n", 27) : printf("%c[1;30;41mW. White  \n", 27);
    (g == true) ? printf("%c[1;30;42mG. Green  \n", 27) : printf("%c[1;30;41mG. Green  \n", 27);
    (y == true) ? printf("%c[1;30;42mY. Yellow \n", 27) : printf("%c[1;30;41mY. Yellow \n", 27);
    (p == true) ? printf("%c[1;30;42mP. Purple \n", 27) : printf("%c[1;30;41mP. Purple \n", 27);
    (b == true) ? printf("%c[1;30;42mB. Blue   \n", 27) : printf("%c[1;30;41mB. Blue   \n", 27);
    printf("%c[0m", 27);
}

//Valid Attacking territory? Checks if you own the point, and if there are surrounding points owned by CPU
bool valAtt(char attckr, bool red, bool white, bool green, bool yellow, bool purple, bool blue)
{
    bool valid;
    if (attckr == 'R')valid = ((red == true) && (white == false || green == false));
    else if (attckr == 'W')valid = ((white == true) && (red == false || green == false || yellow == false || purple == false));
    else if (attckr == 'G')valid = ((green == true) && (red == false || white == false || yellow == false || purple == false));
    else if (attckr == 'Y')valid = ((yellow == true) && (green == false || white == false || purple == false || blue == false));
    else if (attckr == 'P')valid = ((purple == true) && (white == false || yellow == false || blue == false));
    else if (attckr == 'B')valid = ((blue == true) && (yellow == false || purple == false));
    return valid;
}

//Valid Defender territory? Checks if the defending territory is valid.
bool valDef(char attckr, char dfnder, bool red, bool white, bool green, bool yellow, bool purple, bool blue)
{
    bool valid;
    if (attckr == 'R' && (dfnder == 'W' || dfnder == 'G'))
    {
        if (dfnder == 'W' && white == false)valid = true;
        else if (dfnder == 'G' && green == false)valid = true;
        else return false;
    }
    else if (attckr == 'W' && (dfnder == 'R' || dfnder == 'G' || dfnder == 'Y' || dfnder == 'P'))
    {
        if (dfnder == 'R' && red == false)valid = true;
        else if (dfnder == 'G' && green == false)valid = true;
        else if (dfnder == 'Y' && yellow == false)valid = true;
        else if (dfnder == 'P' && purple == false)valid = true;
        else valid = false;
    }
    else if (attckr == 'G' && (dfnder == 'R' || dfnder == 'W' || dfnder == 'Y'))
    {
        if (dfnder == 'R' && red == false)valid = true;
        else if (dfnder == 'W' && white == false)valid = true;
        else if (dfnder == 'Y' && yellow == false)valid = true;
        else valid = false;
    }
    else if (attckr == 'Y' && (dfnder == 'G' || dfnder == 'W' || dfnder == 'P' || dfnder == 'B'))
    {
        if (dfnder == 'G' && green == false)valid = true;
        else if (dfnder == 'W' && white == false)valid = true;
        else if (dfnder == 'P' && purple == false)valid = true;
        else if (dfnder == 'B' && blue == false)valid = true;
        else valid = false;
    }
    else if (attckr == 'P' && (dfnder == 'W' || dfnder == 'Y' || dfnder == 'B'))
    {
        if (dfnder == 'W' && white == false)valid = true;
        else if (dfnder == 'Y' && yellow == false)valid = true;
        else if (dfnder == 'B' && blue == false)valid = true;
        else valid = false;
    }
    else if (attckr == 'B' && (dfnder == 'Y' || dfnder == 'P'))
    {
        if (dfnder == 'P' && purple == false)valid = true;
        else if (dfnder == 'Y' && yellow == false)valid = true;
        else valid = false;
    }
    if (valid == true)
    {
        cout << "===================";
        cout << "\nAttacking...\n";
        cout << "===================\n";
    }
    return valid;
}

bool winner(char attckr, char dfnder, int dRed, int dWhite, int dGreen, int dYellow, int dPurple, int dBlue, bool yRed, bool yWhite, bool yGreen, bool yYellow, bool yPurple, bool yBlue)
{
    bool attWon; //Did the attacker win?
    int attRoll; //Attacker's dice roll
    int defRoll; //Defender's dice roll
    int defDcnt; //Defender's dice count
    int attDcnt; //Attacker's dice count

    //<editor-fold defaultstate="collapsed" desc="Set Attacker and Defender's Dice Count">
    if ('R' == attckr)attDcnt = dRed;
    if ('W' == attckr)attDcnt = dWhite;
    if ('G' == attckr)attDcnt = dGreen;
    if ('Y' == attckr)attDcnt = dYellow;
    if ('P' == attckr)attDcnt = dPurple;
    if ('B' == attckr)attDcnt = dBlue;

    if ('R' == dfnder)defDcnt = dRed;
    if ('W' == dfnder)defDcnt = dWhite;
    if ('G' == dfnder)defDcnt = dGreen;
    if ('Y' == dfnder)defDcnt = dYellow;
    if ('P' == dfnder)defDcnt = dPurple;
    if ('B' == dfnder)defDcnt = dBlue;
    //</editor-fold>

    //Calculate the roll's
    attRoll = roll(attDcnt);
    defRoll = roll(defDcnt);
    cout << "Attacker Rolled: " << attRoll << endl;
    cout << "Defender Rolled: " << defRoll << endl;

    if (attRoll == defRoll)attWon = false; //Defender wins if tie
    else if (isgreater(attRoll, defRoll))attWon = true;
    else attWon = false;
    return attWon;
}

//returns sum of int dCount dice rolls
int roll(int dCount)
{
    int random;
    int roll = 0;
    for (int i = 0; i < dCount; i++)
    {
        random = rand() % 6 + 1;
        roll += random;
    }

    return roll;
}

//Returns Dice Count of the attacking territory
int attDCnt(char attWith, int dRed, int dWhite, int dGreen, int dYellow, int dPurple, int dBlue)
{
    if (attWith == 'R')return dRed;
    else if (attWith == 'W')return dWhite;
    else if (attWith == 'G')return dGreen;
    else if (attWith == 'Y')return dYellow;
    else if (attWith == 'P')return dPurple;
    else if (attWith == 'B')return dBlue;

    return 1;
}

//Returns the amount of adjacent territories for the given player
int adjCnt(bool turn, bool yRed, bool yWhite, bool yGreen, bool yYellow, bool yPurple, bool yBlue)
{
    int acount = 0;
    if (turn == true)//Your turn
    {
        //If you own an adjacent territory ++ the adjacent count
        if (yRed == true && (yGreen == true || yWhite == true))acount++;
        if (yWhite == true && (yRed == true || yGreen == true || yYellow == true || yPurple == true))acount++;
        if (yGreen == true && (yRed == true || yWhite == true))acount++;
        if (yYellow == true && (yGreen == true || yWhite == true || yPurple == true || yBlue == true))acount++;
        if (yPurple == true && (yWhite == true || yYellow == true || yBlue == true))acount++;
        if (yBlue == true && (yYellow == true || yPurple == true))acount++;
    }

    else if (turn == false)//CPU's turn
    {
        //If the CPU owns an adjacent territory ++ the adjacent count
        if (yRed == false && (yGreen == false || yWhite == false))acount++;
        if (yWhite == false && (yRed == false || yGreen == false || yYellow == false || yPurple == false))acount++;
        if (yGreen == false && (yRed == false || yWhite == false))acount++;
        if (yYellow == false && (yGreen == false || yWhite == false || yPurple == false || yBlue == false))acount++;
        if (yPurple == false && (yWhite == false || yYellow == false || yBlue == false))acount++;
        if (yBlue == false && (yYellow == false || yPurple == false))acount++;
    }

    return acount;
}

//Prints the Board/Map
void printBoard(string plyrOne, bool yRed, bool yWhite, bool yGreen, bool yYellow, bool yPurple, bool yBlue, int dRed, int dWhite, int dGreen, int dYellow, int dPurple, int dBlue)
{
    //Ansii color codes as follows...
    //41-red, 42-green, 43-yellow, 44-blue, 45-purple, 47-white
    cout << "\n\nYou: " << plyrOne << endl;//5UC4 NAM3, V3Re P3R50N@L, W@W! (^(o)3(o)^)
    printf("%c[0;34;46m^ ^    ^^^   ^    ^^^     ^^ ^    ^^^       ^^ ^       ^\n", 27); // waves Background
    printf("%c[0;34;46m^^        ^^^",27);printf("%c[41m          ",27);printf("%c[0;34;46m^^^     ^^    ^^^      ^^^     ^^\n", 27);// waves Background
    printf("%c[0;34;46m^^    ^^^",27);printf("%c[41m          ",27);printf("%c[47m               ",27);printf("%c[0;34;46m^^^     ^^    ^     ^^\n", 27); // waves Background
    printf("%c[0;34;46m^^    ^^^",27);printf("%c[41m    ",27);(yRed==true)?printf("%c[1;30;41mYou    ",27):printf("%c[1;30;41mCPU    ",27);printf("%c[47m               ",27);printf("%c[0;34;46m^^     ^^    ^     ^^\n", 27); // waves Background
    printf("%c[0;34;46m^^    ^^^",27);printf("%c[41m    ",27);(yRed==true)?printf("%c[1;30;41mDice:%d   ",27, dRed):printf("%c[1;30;41mDice:%d   ",27, dRed);printf("%c[47m            ",27);printf("%c[0;34;46m^^^     ^^    ^     ^^\n", 27); // waves Background
    printf("%c[0;34;46m^^    ^^^",27);printf("%c[41m            ",27);printf("%c[47m             ",27);printf("%c[0;34;46m^^^     ^^    ^     ^^\n", 27); // waves Background
    printf("%c[0;34;46m^^     ^^^",27);printf("%c[41m              ",27);printf("%c[47m                     ",27);printf("%c[0;34;46m^^^      ^^\n", 27); // waves Background
    printf("%c[0;34;46m^^      ^^^",27);printf("%c[42m           ",27);printf("%c[47m                  ",27);printf("%c[0;34;46m^^^     ^^    ^^\n", 27); // waves Background
    printf("%c[0;34;46m^^      ^",27);printf("%c[42m             ",27);printf("%c[47m    ",27);(yWhite==true)?printf("%c[1;30;47mYou      ",27):printf("%c[1;30;47mCPU      ",27);printf("%c[0;34;46m^^^     ^ ^     ^^^  \n", 27); // waves Background
    printf("%c[0;34;46m^^      ",27);printf("%c[42m      ",27);(yGreen==true)?printf("%c[1;30;42mYou     ",27):printf("%c[1;30;42mCPU     ",27);printf("%c[47m    ",27);(yWhite==true)?printf("%c[1;30;47mDice:%d   ",27, dWhite):printf("%c[47mDice:%d   ",27, dWhite);printf("%c[0;34;46m^^^      ^     ^^^   \n", 27); // waves Background
    printf("%c[0;34;46m^^      ^",27);printf("%c[42m     ",27);(yGreen==true)?printf("%c[1;30;42mDice:%d  ",27, dGreen):printf("%c[1;30;42mDice:%d  ",27, dGreen);printf("%c[47m             ",27);printf("%c[0;34;46m^^^     ^^    ^^^    \n", 27); // waves Background
    printf("%c[0;34;46m^^      ^^^",27);printf("%c[42m           ",27);printf("%c[47m               ",27);printf("%c[0;34;46m^^^     ^^    ^^^  \n", 27); // waves Background
    printf("%c[0;34;46m^^      ^^^",27);printf("%c[42m           ",27);printf("%c[47m                      ",27);printf("%c[0;34;46m^^^     ^^  \n", 27); // waves Background
    printf("%c[0;34;46m^^             ^^^",27);printf("%c[43m       ",27);printf("%c[45m       ",27);printf("%c[0;34;46m^^^     ^^    ^^^       \n", 27); // waves Background                                                                                                                                       //Well this got a little.. out of hand...
    printf("%c[0;34;46m^^             ^^^",27);printf("%c[43m           ",27);printf("%c[45m       ",27);printf("%c[0;34;46m^^^     ^^    ^^^   \n", 27); // waves Background
    printf("%c[0;34;46m^^     ^^^      ^^^",27);printf("%c[43m        ",27);printf("%c[45m          ",27);printf("%c[0;34;46m^^^  ^^    ^^^     \n", 27); // waves Background
    printf("%c[0;34;46m^^       ^^    ^^",27);printf("%c[43m          ",27);printf("%c[45m           ",27);printf("%c[0;34;46m^   ^^    ^^^     \n", 27); // waves Background
    printf("%c[0;34;46m^^       ^^    ^^",27);printf("%c[43m  ",27);(yYellow==true)?printf("%c[1;30;43mYou    ",27):printf("%c[1;30;43mCPU    ",27);printf("%c[45m     ",27);(yPurple==true)?printf("%c[1;30;45mYou    ",27):printf("%c[1;30;45mCPU    ",27);printf("%c[0;34;46m^   ^^    ^^^     \n", 27); // waves Background
    printf("%c[0;34;46m^^       ^^    ^^",27);printf("%c[43m  ",27);(yYellow==true)?printf("%c[1;30;43mDice:%d ",27, dYellow):printf("%c[1;30;43mDice:%d ",27, dYellow);printf("%c[45m     ",27);(yPurple==true)?printf("%c[1;30;5mDice:%d  ",27, dPurple):printf("%c[1;30;45mDice:%d  ",27, dPurple);printf("%c[0;34;46m^   ^^   ^^^     \n", 27); // waves Background
    printf("%c[0;34;46m^^       ^    ^^^",27);printf("%c[43m          ",27);printf("%c[45m           ",27);printf("%c[0;34;46m    ^^    ^^^     \n", 27); // waves Background
    printf("%c[0;34;46m^^       ^^       ^^^",27);printf("%c[43m      ",27);printf("%c[45m         ",27);printf("%c[0;34;46m^^    ^^    ^^^     \n", 27); // waves Background
    printf("%c[0;34;46m^^     ^         ^^^",27);printf("%c[43m           ",27);printf("%c[45m       ",27);printf("%c[0;34;46m^^^     ^^    ^^^ \n", 27); // waves Background
    printf("%c[0;34;46m^^               ^^^",27);printf("%c[43m           ",27);printf("%c[44m           ",27);printf("%c[0;34;46m^^^     ^^   ^\n", 27); // waves Background
    printf("%c[0;34;46m^^      ^^^",27);printf("%c[44m                               ",27);printf("%c[0;34;46m^^^     ^^    \n", 27);
    printf("%c[0;34;46m^^      ^^^",27);printf("%c[44m                               ",27);printf("%c[0;34;46m^^^     ^^    \n", 27); 
    printf("%c[0;34;46m^^      ^^^",27);printf("%c[44m                   ",27);(yBlue==true)?printf("%c[1;30;44mYou                   ",27):printf("%c[1;30;44mCPU                   ",27);printf("%c[0;34;46m^^^ \n", 27);
    printf("%c[0;34;46m^^      ^^^",27);printf("%c[44m                 ",27);(yBlue==true)?printf("%c[1;30;44mDice:%d                ",27, dBlue):printf("%c[1;30;44mDice:%d                ",27, dBlue);printf("%c[0;34;46m^^^   \n", 27);
    printf("%c[0;34;46m^^      ^^^",27);printf("%c[44m                               ",27);printf("%c[0;34;46m^^^     ^^    \n", 27);
    printf("%c[0;34;46m^^      ^^^",27);printf("%c[44m                       ",27);printf("%c[0;34;46m^^^     ^^    ^^^     \n", 27);
    printf("%c[0;34;46m^^^    ^^^    ^    ^^     ^ ^^    ^^^        ^^^     ^^^\n", 27);
    printf("%c[0m", 27);//Resets
}

//Fills the CPU's array of options of attack
void fillAttackArray(char *cpuVldA, bool yRed, bool yWhite, bool yGreen, bool yYellow, bool yPurple, bool yBlue, int SIZE)
{
    for (int i = 0; i < SIZE; i++)
    {
        switch (i)
        {
            case 0:
                if (yRed == true)
                {
                    cpuVldA[i] = '\0'; //If you own the territory, the CPU cant attack with it
                }
                else if (yRed == false)
                {
                    if (yGreen == true || yWhite == true)
                    {
                        cpuVldA[i] = 'R';
                    }
                }
                break;
            case 1:
                if (yWhite == true)
                {
                    cpuVldA[i] = '\0'; //If you own the territory, the CPU cant attack with it
                }
                else if (yWhite == false)
                {
                    if (yRed == true || yGreen == true || yYellow == true || yPurple == true)
                    {
                        cpuVldA[i] = 'W';
                    }
                }
                break;
            case 2:
                if (yGreen == true)
                {
                    cpuVldA[i] = '\0'; //If you own the territory, the CPU cant attack with it
                }
                else if (yGreen == false)
                {
                    if (yRed == true || yWhite == true || yYellow == true)
                    {
                        cpuVldA[i] = 'G';
                    }
                }
                break;
            case 3:
                if (yYellow == true)
                {
                    cpuVldA[i] = '\0'; //If you own the territory, the CPU cant attack with it
                }
                else if (yYellow == false)
                {
                    if (yGreen == true || yWhite == true || yPurple == true || yBlue == true)
                    {
                        cpuVldA[i] = 'Y';
                    }
                }
                break;
            case 4:
                if (yPurple == true)
                {
                    cpuVldA[i] = '\0'; //If you own the territory, the CPU cant attack with it
                }
                else if (yPurple == false)
                {
                    if (yWhite == true || yYellow == true || yBlue == true)
                    {
                        cpuVldA[i] = 'P';
                    }
                }
                break;
            case 5:
                if (yBlue == true)
                {
                    cpuVldA[i] = '\0'; //If you own the territory, the CPU cant attack with it
                }
                else if (yBlue == false)
                {
                    if (yYellow == true || yPurple == true)
                    {
                        cpuVldA[i] = 'B';
                    }
                }
                break;

        }
    }
}

//Fills the CPU's array of options it can attack with after selecting an "attacking" territory
void fillDefenseArray(char attWith, bool yRed, bool yWhite, bool yGreen, bool yYellow, bool yPurple, bool yBlue, char *cpuVldD)
{
    //Fills the array with valid defending territories depending on what the CPU is attacking with
    switch (attWith)
    {
        case 'R':
            if (yWhite == true)cpuVldD[1] = 'W';
            if (yGreen == true)cpuVldD[2] = 'G';
            break;
        case 'W':
            if (yRed == true)cpuVldD[0] = 'R';
            if (yGreen == true)cpuVldD[2] = 'G';
            if (yYellow == true)cpuVldD[3] = 'Y';
            if (yPurple == true)cpuVldD[4] = 'P';
            break;
        case 'G':
            if (yRed == true)cpuVldD[0] = 'R';
            if (yWhite == true)cpuVldD[1] = 'W';
            if (yYellow == true)cpuVldD[3] = 'Y';
            break;
        case 'Y':
            if (yGreen == true)cpuVldD[2] = 'G';
            if (yWhite == true)cpuVldD[1] = 'W';
            if (yPurple == true)cpuVldD[4] = 'P';
            if (yBlue == true)cpuVldD[5] = 'B';
            break;
        case 'P':
            if (yWhite == true)cpuVldD[1] = 'W';
            if (yYellow == true)cpuVldD[3] = 'Y';
            if (yBlue == true)cpuVldD[5] = 'B';
            break;
        case 'B':
            if (yYellow == true)cpuVldD[3] = 'Y';
            if (yPurple == true)cpuVldD[4] = 'P';
            break;
    }
}

//
//</editor-fold>