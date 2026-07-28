// CS14 Final Project - Title: The Night Shift
// This program is a text-based suspense game
// Kevin Tseng
// May 17, 2025

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <ctime>

// Structure Declarations
/* CONCEPT INCLUDED: Structures */
/* CONCEPT INCLUDED: Different Primitive Data Types (int, bool, char, std::string) */
struct Room {
	std::string name;       // name of location: lobby, break room, security office ...
	std::string type;       // category: conference rooms, restrooms ...
	bool hasIntruder;       // true if the intruder is located here
	int timesVisited;       // how many times the player searched
	bool spot1Cleared;      // true if spot a has been searched
	bool spot2Cleared;      // true if spot b has been searched
	bool spot3Cleared;      // true if spot c has been searched
	char intruderSpot;      // intruder hide spot (a, b or c)
};

struct PlayerStat {
	int threatLevel;        // keep track of the threat level
	bool intruderCaught;    // true if intruder is caught
};

// ----------------------------------------------------------------------------------------------------------------
// Function Prototypes
/*CONCEPT INCLUDED: Functions*/

// --- Title & Intro ---
void titleAndStory();           // display Game Title and background story
void instructions();            // display the game instructions
void preGameIntro();            // tutorial before actual Game starts

// --- Setup ---
void setupBuilding(Room [], int);      // initialize each room
void placeIntruder(Room [], int);      // place the intruder in the hiding spot

// --- Main Gameplay ---
void coreGameLoop(Room [], PlayerStat &);   // actual Core Game Loop
int searchAction(Room &);                   // prompt user to enter & search the locations

// --- Display ---
void displayWarning(Room [], int, int);         // display random warning messages
void displayMap(Room [], int);                  // display the building map
void displayRoomArts(int);                      // display art for corresponding location
void displayThreatLevel(const PlayerStat &);    // display current threat level
void displayFinalSummary(Room [], int );        // display the final summary

// --- Utility ---
void sortRoomsByVisits(Room [], int);           // sort the locations by visits
int findIntruderLocation(Room [], int);         // find the "location" that the intruder is hiding
std::string findIntruderSpot(const Room &);     // find the "area" that the intruder is hiding

// --- Mini Game ---
bool diceRoll();        // dice roll (player vs intruder)
bool ticTacToe();       // tic tac toe game

void displayBoard(char [3][3]);     // display the 3x3 board for tic-tac-toe
int checkWin(char [3][3], char);    // check if player or intruder wins
int checkTie(char [3][3]);          // check if it ends in a tie

// ----------------------------------------------------------------------------------------------------------------
// Global Constant Variables for
// array size and maximum threat level
/* CONCEPT INCLUDED: Variables and Named Constants */
const int SIZE = 9;
const int maxThreat = 16;

// ----------------------------------------------------------------------------------------------------------------
int main()
{
	// variable for player's choice
	// to replay or quit the game
	int playAgain;

	// get system time and generate random number
	/* CONCEPT INCLUDED: Mathematical Library Functions (Random Numbers) */
	int seed = std::time(0);
	std::srand(seed);

	do {
		// --- GAME TITLE & INSTRUCTIONS ---

		// call up function to display game title and background story
		titleAndStory();

		// call up function to display the instructions
		instructions();


		// --- SET UP ---

		// array for the entire building (9 rooms total)
		/* CONCEPT INCLUDED: Arrays */
		Room building[SIZE];

		// initialize player with default value
		PlayerStat player = {0, false};

		// call up function to initialize each room (location)
		setupBuilding(building, SIZE);

		// call up function to place intruder in hiding spot
		placeIntruder(building, SIZE);


		// --- GAME SECTION ---

		// Intro BEFORE the game starts
		preGameIntro();

		// Core Game Loop
		coreGameLoop(building, player);


		// --- FINAL RESULTS ---

		// display summary at the end of the game
		displayFinalSummary(building, SIZE);

		int hideLocation = findIntruderLocation(building, SIZE);            // variable to store the hiding location
		std::string hideSpot = findIntruderSpot(building[hideLocation]);    // variable to store the hiding spot

		// display final report and intruder hiding spot
		std::cout << "*** INCIDENT REPORT ***" << std::endl;
		std::cout << "The intruder was hiding in: " << std::endl;
		std::cout << "Location - " << building[hideLocation].name << std::endl;
		std::cout << "Area - " << hideSpot << std::endl;


		// --- Play Again or Quit ---
		/* CONCEPT INCLUDED: Input/Output */

		std::cout << "\nPress [1] to play again, or [0] to exit." << std::endl;
		std::cout << "Choice: ";
		std::cin >> playAgain;

		/* CONCEPT INCLUDED: Input Validation */
		while(std::cin.fail() || (playAgain != 1 && playAgain != 0)) {
			std::cin.clear();
			std::cin.ignore(1000, '\n');

			std::cout << "Invalid input. Enter [1] or [0]: ";
			std::cin >> playAgain;
		}

		std::cin.ignore(1000, '\n');

	} while (playAgain == 1);

	return 0;
}

//---------------------------------------------------------------------------------------------------------------
// FUNCTIONS

// --- Title & Intro Functions ---

// Function to display game title and background story
void titleAndStory() {
	std::cout << R"(
 ________  __    __  ________        __    __  ______   ______   __    __  ________         ______   __    __  ______  ________  ________ 
|        \|  \  |  \|        \      |  \  |  \|      \ /      \ |  \  |  \|        \       /      \ |  \  |  \|      \|        \|        \
 \$$$$$$$$| $$  | $$| $$$$$$$$      | $$\ | $$ \$$$$$$|  $$$$$$\| $$  | $$ \$$$$$$$$      |  $$$$$$\| $$  | $$ \$$$$$$| $$$$$$$$ \$$$$$$$$
   | $$   | $$__| $$| $$__          | $$$\| $$  | $$  | $$ __\$$| $$__| $$   | $$         | $$___\$$| $$__| $$  | $$  | $$_____    | $$
   | $$   | $$    $$| $$  \         | $$$$\ $$  | $$  | $$|    \| $$    $$   | $$          \$$    \ | $$    $$  | $$  | $$     \   | $$
   | $$   | $$$$$$$$| $$$$$         | $$\$$ $$  | $$  | $$ \$$$$| $$$$$$$$   | $$          _\$$$$$$\| $$$$$$$$  | $$  | $$$$$$$$   | $$
   | $$   | $$  | $$| $$_____       | $$ \$$$$ _| $$_ | $$__| $$| $$  | $$   | $$         |  \__| $$| $$  | $$ _| $$_ | $$         | $$
   | $$   | $$  | $$| $$     \      | $$  \$$$|   $$ \ \$$    $$| $$  | $$   | $$          \$$    $$| $$  | $$|   $$ \| $$         | $$
    \$$    \$$   \$$ \$$$$$$$$       \$$   \$$ \$$$$$$  \$$$$$$  \$$   \$$    \$$           \$$$$$$  \$$   \$$ \$$$$$$ \$$          \$$
    )";
	std::cout << std::endl;
	// https://patorjk.com/software/taag/#p=display&f=Big+Money-se&t=the+night+shift&x=none&v=4&h=4&w=80&we=false
	std::cout << R"(
  _____       _                 _            _   _             
 |_   _|     | |               | |          | | (_)            
   | |  _ __ | |_ _ __ ___   __| |_   _  ___| |_ _  ___  _ __ 
   | | | '_ \| __| '__/ _ \ / _` | | | |/ __| __| |/ _ \| '_ \
  _| |_| | | | |_| | | (_) | (_| | |_| | (__| |_| | (_) | | | |
 |_____|_| |_|\__|_|  \___/ \__,_|\__,_|\___|\__|_|\___/|_| |_|
    )";
	std::cout << std::endl;
	// https://patorjk.com/software/taag/#p=display&f=Big&t=Introduction&x=none&v=4&h=4&w=80&we=false
	std::cout << "You are the only security guard working the night shift in an office building after hours" << std::endl;
	std::cout << "It has been a quiet shift until a call from the off-site monitoring team interrupts the silence" << std::endl;
	std::cout << "Headquarters reports that a corporate spy has entered the building to steal sensitive data " << std::endl;
	std::cout << "The intruder is hiding somewhere in the building" << std::endl;
	std::cout << "Backup is 20 minutes away..." << std::endl;
	std::cout << "\nYour mission: Search the premises and detain the intruder" << std::endl;
	std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "\nPress [Enter] to continue";
	while (std::cin.get() != '\n') {
		// forces player to press [Enter] in order to continue
	}
}

// Function to display the instructions
void instructions() {
	std::cout << R"(
  _____           _                   _   _                 
 |_   _|         | |                 | | (_)                
   | |  _ __  ___| |_ _ __ _   _  ___| |_ _  ___  _ __  ___ 
   | | | '_ \/ __| __| '__| | | |/ __| __| |/ _ \| '_ \/ __|
  _| |_| | | \__ \ |_| |  | |_| | (__| |_| | (_) | | | \__ \
 |_____|_| |_|___/\__|_|   \__,_|\___|\__|_|\___/|_| |_|___/
    )";
	//https://patorjk.com/software/taag/#p=display&f=Big&t=Instructions&x=none&v=4&h=4&w=80&we=false
	std::cout << std::endl;
	std::cout << "--- HOW TO PLAY ---" << std::endl;
	std::cout << "\n1. INVESTIGATE: Use the Map to navigate between 9 locations." << std::endl;
	std::cout << "\n2. SEARCH: Each location has 3 specific hiding spots (a, b, or c)." << std::endl;
	std::cout << "\n3. WARNINGS: Warning messages are random, some can be true while" << std::endl;
	std::cout << "   some can be misleading." << std::endl;
	std::cout << "\n4. THREAT LEVEL: Every time you search a spot and find nothing," << std::endl;
	std::cout << "   the Threat Level increases. If it reaches " << maxThreat << ", the intruder" << std::endl;
	std::cout << "   escapes and you lose." << std::endl;
	std::cout << "\n5. CAUGHT: If you find the intruder, you must win a game of " << std::endl;
	std::cout << "   Tic-Tac-Toe to successfully place them in handcuffs." << std::endl;
	std::cout << "\n6. STRATEGY: Leaving a room (d) does not increase threat level," << std::endl;
	std::cout << "   but it doesn't get you closer to the intruder!" << std::endl;
	std::cout << "\n7. TIP: Try to remember which spots have been searched already." << std::endl;
	std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "\nBefore the night shift begins, we will start with a Preliminary Search" << std::endl;
	std::cout << "to prepare you for the investigation." << std::endl;
	std::cout << "\nPress [Enter] to continue";
	while (std::cin.get() != '\n') {
		// forces player to press [Enter] in order to continue
	}
}

// Function for the tutorial before actual game starts
void preGameIntro() {
	std::cout << R"(
 ██╗    ██╗ █████╗ ██████╗ ███╗   ██╗██╗███╗   ██╗ ██████╗ 
 ██║    ██║██╔══██╗██╔══██╗████╗  ██║██║████╗  ██║██╔════╝ 
 ██║ █╗ ██║███████║██████╔╝██╔██╗ ██║██║██╔██╗ ██║██║  ███╗
 ██║███╗██║██╔══██║██╔══██╗██║╚██╗██║██║██║╚██╗██║██║   ██║
 ╚███╔███╔╝██║  ██║██║  ██║██║ ╚████║██║██║ ╚████║╚██████╔╝
  ╚══╝╚══╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝╚═╝  ╚═══╝ ╚═════╝ 
	)" ;
	// https://www.asciiart.eu/text-to-ascii-art
	// intro warning BEFORE the actual game starts
	std::cout << std::endl;
	std::cout << "⚠ A loud thudding noise came from the Reception Area!! ⚠" << std::endl;


	// guide the player to go to the Reception Area
	// https://ascii.co.uk/art/hallway
	int introChoice = 0;
	std::cout << R"(
'.                      |
  '.                    |       ___________________             ____________
    '.                  |      |  _______________  |           |.----------.|
 .    '.                |      | |               | |           ||     |>   ||
 |'.    '.              |      | |   ___   ___   | |           || O  /|    ||
 |. '.    '.____________|      | |  |   | |   |  | |           ||   /_(    ||
 |||.|     |            |      | |  |   | |   |  | |           || .___|__  ||
 |||||     |            |      | |  |   | |   |  | |           ||~~\____|~~||
 |||||     |            |      | |  |   | |   |  | |           || ~  ~    ~||
 |||||     |            |      | |  |   | |   |  | |           |:__________:|
 |||||     |        *   |      | |  |   | |   |  | |           '------------'
 |||||     |    *..'    |      | |  '---___---'__| |
 |  '|     |     _:_    |      | |    |_____| |__= |
 |. ()     |    (   )   |      | |   ___   ___ ()| |
 |||||     |     ) (    |      | |  |   | |   |  | |
 |||||     | ===========|      | |  |   | |   |  | |
 |||||     |   | |    | |      | |  |   | |   |  | |
 |||||     |   | |    | |      | |  |   | |   |  | |
 |||||    _|___|_|____|_|      | |  |   | |   |  | |
 |||:|--"" |___|_|____|_|      | |  '---' '---'  | |
"|  _|..--"'   '      ' |______| |               | |___________________grp___
 -'"'                   |______|_|_______________|_|_________________________
	    )";
	std::cout << std::endl ;
	std::cout << "\nYou should investigate. Enter [1] to go to the Reception Area: ";
	std::cin >> introChoice;

	// validation loop to "force" user to enter Reception Area
	while (introChoice != 1) {
		std::cin.clear();
		std::cin.ignore(1000, '\n');
		std::cout << "You can't ignore that noise! Enter [1] to go to the Reception Area: ";
		std::cin >> introChoice ;
	}


	// Prompt for specific actions in the Reception Area
	std::cout << "\n>> You are now in the Reception Area. It's dark and quiet..." << std::endl;
	std::cout << "\n>> Pick an action: " << std::endl;
	std::cout << "a. Look under the reception desk" << std::endl;
	std::cout << "b. Check around the vending machine" << std::endl;
	std::cout << "c. Inspect behind the window blinds" << std::endl;
	std::cout << "Choice: ";
	char actionChoice;
	std::cin >> actionChoice;
	std::cout << "\n----------------------------------------------------------------------" << std::endl;

	// input validation loop
	while(!(actionChoice == 'a' || actionChoice == 'b' || actionChoice == 'c')) {
		std::cin.clear();
		std::cin.ignore(1000, '\n');
		std::cout << "Invalid input. Please enter [a], [b] or [c]: " ;
		std::cin >> actionChoice ;
	}

	// responses for the intro actions
	/* CONCEPT INCLUDED: if/else if Statements */
	if (actionChoice == 'a') {
		std::cout << "\n>> You check under the desk, there's nothing but a trash can..." << std::endl;
		std::cout << R"(⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⢰⡟⠛⠛⠛⠛⢻⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⣤⣤⣤⣤⣼⣧⣤⣤⣤⣤⣼⣧⣤⣤⣤⣤⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠛⣛⣛⣛⣛⣛⣛⣛⣛⣛⣛⣛⣛⣛⣛⠛⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⢻⣿⣿⢹⣿⣿⡏⢹⣿⣿⡏⣿⣿⡟⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⢸⣿⣿⠘⣿⣿⡇⢸⣿⣿⠃⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠸⣿⣿⠀⣿⣿⡇⢸⣿⣿⠀⣿⣿⠇⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⣿⣿⠀⣿⣿⡇⢸⣿⣿⠀⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⣿⣿⠀⢿⣿⡇⢸⣿⡿⠀⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⢸⣿⡆⢸⣿⡇⢸⣿⡇⢰⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⢸⣿⡇⢸⣿⡇⢸⣿⡇⢸⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠈⣿⣷⣾⣿⣷⣾⣿⣷⣾⣿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
        )";
		std::cout << std::endl ;
		// https://emojicombos.com/trash-can-ascii-art
	}

	else if (actionChoice == 'b') {
		std::cout << "\n>> You check behind the vending machine... something scurries away!" << std::endl;
		std::cout << ">> It was just a rat... false alarm." << std::endl;
		std::cout << R"(
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⢤⢄⠀⡚⠿⢶⣠⣴⣶⣶⣶⣶⢷⢦⡄⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣇⠻⢷⣽⣷⣴⣿⣿⣿⣿⣽⣿⣧⡤⠋⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⢦⠠⠽⣿⣿⡿⣾⡟⣿⣿⣿⠏⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢉⣿⡾⣿⣾⡿⣿⣿⣿⠛⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣾⣿⣿⣿⣿⢿⣿⣿⣇⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣺⣿⣿⣿⣿⣿⣽⣳⣿⣿⠃⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣿⣯⣿⣽⣿⣿⣿⣿⣿⣿⣦⡀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣿⣿⣿⣿⣧⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣖⢒⣽⠛
⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⣿⣿⣿⣿⣿⣿⣿⣷⡀⠻⣿⣿⡿⠉⠁⠀⠀⠑⠽⠷
⠀⠀⠀⠀⠀⠀⠀⠀⠰⣿⣿⣿⣿⣿⣿⣿⣿⣿⢗⣷⣿⣿⠁⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀ ⠀⠀⠀⠀⠚⣿⣿⣿⣿⡿⠚⢡⣴⣿⣵⣯⣿⣿⡀⠀⠀⠀⠀⠀⠀⠀
⠉⠉⠉⠉⢈⣱⠀⠀⠈⢿⣿⣿⣿⣧⣤⣿⣾⣿⣿⣿⣿⠏⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⢠⡖⠋⠀⠀⠀⣀⣈⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⣄⣀⡀⠀⠀⠀⠀⠀⠀
⠀⠀⠈⠛⠳⠟⠛⠛⠉⠉⠉⠻⢿⡿⢿⡟⠛⠛⠉⠉⠉⠓⠚⠚⠂⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠑⣶⡗⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
        )" ;
		std::cout << std::endl;
		// https://emojicombos.com/rat
	}

	else if (actionChoice == 'c') {
		std::cout << "\n>> You check behind the window blinds, and the window is opened!!" << std::endl;
		std::cout << ">> Could it be the rat? I don't think so..." << std::endl;
		std::cout << R"(⠀
⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠘⣿⣿⣷⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠈⠛⠻⠿⢀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⠿⠿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⣤⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⢻⣿⣿⡀⠀⠀⠀⠀⠀⣾⣿⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠿⢃⣄⠀⠀⠀⠀⠘⠿⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠿⠃⠀⠀⠀⠀⠀⠈⣩⣤⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠛⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⣿⣿⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⣴⣶⡄⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠀⠀⠀⠀⠀⠀⠀⠀
		)";
		std::cout << std::endl ;
		// https://emojicombos.com/foot-prints
	}


	// instruct player to continue searching
	// then the actual "core game loop" starts
	std::cout << "\nContinue to patrol the building..." << std::endl;
}


// --- Setup Functions ---

// Function to initialize each location
void setupBuilding(Room building[], int size) {
	building[0] = {"Lobby", "lobby", false, 0, false, false, false, ' '};
	building[1] = {"Conference A", "conference", false, 0, false, false, false, ' '};
	building[2] = {"Conference B", "conference", false, 0, false, false, false, ' '};
	building[3] = {"Conference C", "conference", false, 0, false, false, false, ' '};
	building[4] = {"Restroom #1", "restroom", false, 0, false, false, false, ' '};
	building[5] = {"Restroom #2", "restroom", false, 0, false, false, false, ' '};
	building[6] = {"Break Room", "break", false, 0, false, false, false, ' '};
	building[7] = {"Storage Room", "storage", false, 0, false, false, false, ' '};
	building[8] = {"Security Office", "security", false, 0, false, false, false, ' '};
}

// Function to place the intruder in a hiding spot
void placeIntruder(Room building [], int size) {

	// generate a random number (0 to 8)
	// place the intruder in the location with the index number
	int const MAX_ROOM_INDEX = 8 ;
	int const MIN_ROOM_INDEX = 0 ;
	int intruderRoomIndex = (std::rand() % (MAX_ROOM_INDEX - MIN_ROOM_INDEX + 1)) + MIN_ROOM_INDEX ;
	building[intruderRoomIndex].hasIntruder = true;

	// generate a random number (0, 1, or 2)
	int const MAX_ROLL = 2 ;
	int const MIN_ROLL = 0 ;
	int spotRoll = (std::rand() % (MAX_ROLL - MIN_ROLL + 1)) + MIN_ROLL ;

	// assign hiding spots 'a', 'b', or 'c' based on the roll
	// place the intruder in the spot of the location
	if (spotRoll == 0) {
		building[intruderRoomIndex].intruderSpot = 'a';
	} else if (spotRoll == 1) {
		building[intruderRoomIndex].intruderSpot = 'b';
	} else {
		building[intruderRoomIndex].intruderSpot = 'c';
	}
}


// --- Main Gameplay Functions ---

// Function for the Core Game Loop
/* CONCEPT INCLUDED: Pass by Reference */
void coreGameLoop(Room building[], PlayerStat &player) {

	bool gameOver = false;  // flag to control gameplay loop
	int choiceRoom ;        // store player's choice of room number

	while (!gameOver) {

		displayMap(building, SIZE); // display the building map

		// only show if the threat level is still below the limit
		if (player.threatLevel < maxThreat) {
			displayThreatLevel(player);
		}

		// prompt user to enter a location
		std::cout << "\nEnter room number to enter (or 0 to quit): ";
		std::cin >> choiceRoom;

		// input validation loop
		while(std::cin.fail() || choiceRoom < 0 || choiceRoom > 9) {
			std::cin.clear();
			std::cin.ignore(1000, '\n');

			std::cout << "Invalid input. Please enter a valid room number: " ;
			std::cin >> choiceRoom;
		}

		// player ends the game early by entering 0
		if (choiceRoom == 0) {
			gameOver = true;
			std::cout << "Game over." ;
		}

		// player chooses a room to enter
		else if (choiceRoom > 0 && choiceRoom <= SIZE) {
			// reference to current room
			Room &selectedRoom = building[choiceRoom - 1];
			// track for the sorting function
			selectedRoom.timesVisited++;

			// call up function and display art for corresponding location
			displayRoomArts(choiceRoom);

			// player picked a room to enter
			// use -1 because array index starts at 0
			std::cout << "\n>> You entered the " << building[choiceRoom - 1].name << std::endl;

			/* CONCEPT INCLUDED: Mathematical Operations */
			// variable to keep track of how many times each room has been searched
			int total = (selectedRoom.spot1Cleared + selectedRoom.spot2Cleared + selectedRoom.spot3Cleared);
			if (total >= 3) {
				std::cout << ">> You've thoroughly searched every corner in here" << std::endl;
				std::cout << ">> Enter [d] to leave and check a different location" << std::endl;
			}
			else {
				std::cout << ">> Note: " << total << "/3 areas investigated" << std::endl;
			}

			//----------------------------------------------------------------------------------------------------------------
			// variable for returned value from searchAction function
			int result = searchAction(selectedRoom);

			// check if the player found the intruder before checking anything else
			if (result == 1) {
				std::cout << R"(
  ⠀⠀⠀⠀⠀⠀⠀⣀⣴⣶⣾⣷⣶⣦⣄⠀⠀⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣷⡀⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⣸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⣿⡏⠀⠀⠀⢙⡏⠀⠀⠀⢹⣿⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⣿⣿⣶⣶⣶⣿⣿⣶⣶⣶⣾⣿⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⢸⣿⣿⣿⠟⠛⠛⠛⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⢀⣠⣴⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣦⣄⡀⠀⠀⠀
  ⢀⣴⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣄⠀
  ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠆
  ⠙⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠋⠀
  ⠀⠀⠀⠀⠀⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠀⠀⠀⠀⠀⠀
				    )";
				// https://emojicombos.com/robber-text-art
				std::cout << std::endl ;
				std::cout << "Now you have to put the intruder in handcuffs before backup arrives." << std::endl ;
				std::cout << "They are not just going to surrender... To defeat the intruder, you must win this tic-tac-toe game." << std::endl;


				// call up the mini-game after intruder is found
				// true - player wins Tic-Tac-Toe, display winning art
				// false - player loses Tic-Tac-Toe, jumps to else
				if (ticTacToe()) {
					player.intruderCaught = true;
					std::cout << R"(
 ⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
 ⠀⠀⠀⠀⠀⣴⠾⠛⢉⣹⣧⣤⣤⠦⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
 ⠀⠀⠀⠀⠀⣽⠷⠾⠛⠉⠀⠸⣧⣄⡈⢹⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
 ⠀⠀⠀⠀⣰⣿⣄⠀⠀⠀⠀⠀⠀⠙⠛⣿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
 ⠀⠀⠀⠀⢿⡀⠙⢷⣄⠀⠀⠀⠀⠀⣴⠟⠷⢀⣤⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
 ⠀⠀⠀⠀⠈⠻⢦⡴⣿⣀⡀⠀⠀⠀⠛⢂⣴⡿⢿⣿⣷⣶⣶⣶⣶⣤⣀⠀⠀⠀
 ⠀⠀⠀⠀⠀⠀⠀⠀⠛⠉⢻⡆⠀⠀⠀⠙⢿⣷⣶⡿⠋⠉⠀⠈⠙⢿⣿⣷⡀⠀
 ⠀⠀⠀⠀⠀⠀⢀⣿⣷⢶⣤⣤⠀⠀⠀⠀⢀⠙⠋⠀⠀⠀⠀⠀⠀⠀⢹⣿⣧⠀
 ⠀⠀⠀⢀⣤⠀⣼⣿⣧⣰⣿⡇⠀⠀⠀⠀⢸⣷⡄⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⠀
 ⠀⠀⣴⣿⡿⠀⠀⠉⠉⠛⢿⣿⣦⠀⠀⠀⠈⢿⣿⣄⠀⠀⠀⠀⠀⠀⣾⣿⠇⠀
 ⠀⣸⣿⡏⠀⠀⠀⠀⠀⠀⠀⢻⣿⡆⠀⠀⠀⠈⠻⣿⣿⣶⣶⣶⣷⣄⠉⠉⠀⠀
 ⠀⢿⣿⡇⠀⠀⠀⠀⠀⠀⠀⢸⣿⡇⠀⠀⠀⠀⠀⠀⠉⠉⠛⠋⠉⠁⠀⠀⠀⠀
 ⠀⠸⣿⣷⣄⠀⠀⠀⠀⠀⣠⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
 ⠀⠀⠘⠿⠏⢠⣶⣶⣶⣿⣿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
 ⠀⠀⠀⠀⠀⠈⠙⠛⠋⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
				    )";
					// https://emojicombos.com/handcuffs
					std::cout << std::endl ;
				}

				// player lose Tic-Tac-Toe, display losing art
				else {
					player.intruderCaught = false;
					std::cout << R"(⠀
⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⢀⣴⣿⣿⣿⣿⣿⣿⣦⡀⠀⠀⠀⠀⠀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⣰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⡀⠀⠀⠉⠛⠛⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⢰⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠿⠿⣷⡄⠀⣀⣀⠓⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⡿⠋⣠⣶⣦⣈⠻⣦⠈⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⢻⣿⣿⣿⣿⣿⣿⡿⠁⣼⡿⠻⣿⣿⣷⣄⣠⣴⠆⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠻⣿⣿⣿⣿⡟⠀⢸⣿⣿⣦⣄⡉⠛⠛⠛⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠈⠛⠿⠋⠀⠀⢻⣿⣿⣿⣿⣿⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⣀⣀⣤⣶⣄⠀⠀⠈⠿⢿⣿⣿⣿⣿⣿⣶⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠸⠟⠛⠉⠻⣿⣧⡀⣼⣶⣤⣄⠉⠉⠛⠛⠻⢿⣿⣦⡀⠀⠀⠀⣠⡀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿⡿⠋⠁⠀⠀⠀⠀⠀⠀⠙⢿⣿⣆⣠⣾⠟⠁⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠻⣿⡿⠁⠀⠀⠀⠀
                )" ;
					// https://emojicombos.com/robber
					std::cout << std::endl;
				}

				gameOver = true;
			}


			// searched but did NOT find the intruder
			else if (result == 0) {
				player.threatLevel++;

				// player LOSE (max threat level reached)
				if (player.threatLevel >= maxThreat) {
					std::cout << R"(⠀
⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⢀⣴⣿⣿⣿⣿⣿⣿⣦⡀⠀⠀⠀⠀⠀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⣰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⡀⠀⠀⠉⠛⠛⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⢰⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠿⠿⣷⡄⠀⣀⣀⠓⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⡿⠋⣠⣶⣦⣈⠻⣦⠈⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⢻⣿⣿⣿⣿⣿⣿⡿⠁⣼⡿⠻⣿⣿⣷⣄⣠⣴⠆⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠻⣿⣿⣿⣿⡟⠀⢸⣿⣿⣦⣄⡉⠛⠛⠛⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠈⠛⠿⠋⠀⠀⢻⣿⣿⣿⣿⣿⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⣀⣀⣤⣶⣄⠀⠀⠈⠿⢿⣿⣿⣿⣿⣿⣶⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠸⠟⠛⠉⠻⣿⣧⡀⣼⣶⣤⣄⠉⠉⠛⠛⠻⢿⣿⣦⡀⠀⠀⠀⣠⡀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿⡿⠋⠁⠀⠀⠀⠀⠀⠀⠙⢿⣿⣆⣠⣾⠟⠁⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠻⣿⡿⠁⠀⠀⠀⠀
                )" ;
					// https://emojicombos.com/robber
					std::cout << std::endl ;
					std::cout << "\n>> The sun is rising... you spent too much time wandering" << std::endl;
					std::cout << ">> The intruder has escaped with the company's secrets..." << std::endl;
					gameOver = true;
				}

				// game continues (max threat level NOT reached)
				else {
					std::cout << "\n----------------------------------------------------------------------" << std::endl;
					displayWarning(building, SIZE, (choiceRoom - 1)); // -1 for the index number
				}
			}

			// player choose to leave the location
			else if (result == 2) {
				std::cout << ">> Search a different spot or go to another location" << std::endl;
				std::cout << ">> (Threat Level remains " << player.threatLevel << ")" << std::endl;
				std::cout << "\n----------------------------------------------------------------------" << std::endl;
			}
		}
	}
}

// Function to prompt user to choose actions
int searchAction(Room &currentRoom) {

	char actionChoice;      // variable to hold user's choice of action
	std::string winMessage = "\n!!! YOU FOUND THE INTRUDER !!!" ;

	std::cout << "\n--- Investigating the " << currentRoom.name << " ---" << std::endl;

	// determine and display the actions menu based on type
	if (currentRoom.type == "conference") {
		std::cout << "a. Look under the desks" << std::endl;
		std::cout << "b. Check behind the presentation screen" << std::endl;
		std::cout << "c. Search the cabinets" << std::endl;
	}
	else if (currentRoom.type == "restroom") {
		std::cout << "a. Check all the stalls" << std::endl;
		std::cout << "b. Search the janitor’s closet" << std::endl;
		std::cout << "c. Inspect the air vents" << std::endl;
	}
	else if (currentRoom.type == "lobby") {
		std::cout << "a. Search behind the front desk" << std::endl;
		std::cout << "b. Look under the furniture" << std::endl;
		std::cout << "c. Check behind the fish tank" << std::endl;
	}
	else if (currentRoom.type == "break") {
		std::cout << "a. Search under the tables" << std::endl;
		std::cout << "b. Look behind the vending machine" << std::endl;
		std::cout << "c. Check behind the refrigerator" << std::endl;
	}
	else if (currentRoom.type == "security") {
		std::cout << "a. Search inside the lockers" << std::endl;
		std::cout << "b. Check on top of the shelves" << std::endl;
		std::cout << "c. Look under the console desks" << std::endl;
	}
	else if (currentRoom.type == "storage") {
		std::cout << "a. Look behind the boxes" << std::endl;
		std::cout << "b. Search inside the cabinets" << std::endl;
		std::cout << "c. Check on top of the shelves" << std::endl;
	}
	std::cout << "d. Leave the room (save time/threat)" << std::endl;

	std::cout << "\n>> Pick an action: ";
	std::cin >> actionChoice;

	// input validation loop
	while (!(actionChoice == 'a' || actionChoice == 'b' || actionChoice == 'c' || actionChoice == 'd')) {
		std::cin.clear();
		std::cin.ignore(1000, '\n');
		std::cout << "\n>> Don't just stand there... the intruder won't come out and say hi" << std::endl;
		std::cout << ">> Pick an action: " ;
		std::cin >> actionChoice ;
	}
	std::cout << std::endl;

	// use different cases to handle the choice outcomes
	switch (actionChoice) {
	case 'a':
		if (currentRoom.spot1Cleared) {     // tell the player this area has been searched
			std::cout << ">> You already searched this spot" << std::endl;
			return 2; // threat level does NOT increase
		}

		std::cout << ">> You search the first area" ;
		currentRoom.spot1Cleared = true;    // update the status of spot1

		// check if intruder was placed in this area
		if (currentRoom.hasIntruder && currentRoom.intruderSpot == 'a') {
			std::cout << winMessage << std::endl;
			return 1; // intruder is found
		}

		// display if intruder is not in this area
		std::cout << "\nEverything seems normal..." << std::endl;
		return 0 ; // nothing found

	case 'b':
		if (currentRoom.spot2Cleared) {     // tell the player this area has been searched
			std::cout << ">> You already searched this spot" << std::endl;
			return 2; // threat level does NOT increase
		}

		std::cout << ">> You check the second area" ;
		currentRoom.spot2Cleared = true;    // update the status of spot2

		// check if intruder was placed in this area
		if (currentRoom.hasIntruder && currentRoom.intruderSpot == 'b') {
			std::cout << winMessage << std::endl;
			return 1; // intruder is found
		}

		// display if intruder is not in this area
		std::cout << "\nNothing was found..." << std::endl;
		return 0 ; // nothing found

	case 'c':
		if (currentRoom.spot3Cleared) {     // tell the player this area has been searched
			std::cout << ">> You already searched this spot" << std::endl;
			return 2; // threat level does NOT increase
		}

		std::cout << ">> You inspect the third area" ;
		currentRoom.spot3Cleared = true;    // update the status of spot3

		// check if intruder was placed in this area
		if (currentRoom.hasIntruder && currentRoom.intruderSpot == 'c') {
			std::cout << winMessage << std::endl;
			return 1; // intruder is found
		}

		// display if intruder is not in this area
		std::cout << "\nNo signs of the intruder..." << std::endl;
		return 0 ; // nothing found

	case 'd':
		std::cout << ">> You decided to leave this location" << std::endl;
		return 2; // leave the location w/o increase the threat level
	}

	return 0;
}


// --- Display Functions ---

// Function to display random warning messages
void displayWarning(Room rooms[], int numRooms, int currentRoomIndex) {

	// range for random number generated
	const int MAX_ROOM = 8 ;
	const int MIN_ROOM = 0 ;

	int roomIndex;      // variable to hold the random number generated
	bool isInvalid;     // flag to determine if random number needs to be generated again

	// do-while validation loop
	// to make sure the random warning message "makes sense"
	/* CONCEPT INCLUDED: while / do while Loops */
	do {
		roomIndex = (std::rand() % (MAX_ROOM - MIN_ROOM + 1)) + MIN_ROOM;

		// check if the room is fully searched (sum of the 3 bools)
		int totalCleared = (rooms[roomIndex].spot1Cleared +
		                    rooms[roomIndex].spot2Cleared +
		                    rooms[roomIndex].spot3Cleared);

		// location of random warnings after their action will NOT be the room they just investigated
		// after the room is FULLY checked, make sure NOT to display warning of that room
		bool isCurrentRoom = (roomIndex == currentRoomIndex);
		bool isFullySearched = (totalCleared >= 3);

		// if either rule is broken, room choice is invalid
		isInvalid = (isCurrentRoom || isFullySearched);

	} while (isInvalid); // if choice is invalid, go back to "do" and roll again


	// array of 6 different types of warning messages
	std::string noiseBank[] = {
		"A heavy thud echoed from the ",
		"A tall shadow passes by the ",
		"Door creaking noise coming from the ",
		"A glass object just shattered inside the ",
		"The lights flickered briefly in the ",
		"Sound of rapid footsteps coming from the "
	};

	// generate a random number between 0 and 5 inclusive
	const int MAX_NOISE = 5 ;
	const int MIN_NOISE = 0 ;
	int noiseIndex = (std::rand() % (MAX_NOISE - MIN_NOISE + 1)) + MIN_NOISE ;

	// display the random warning message from random location
	std::cout << "\n⚠ WARNING ⚠" << std::endl ;
	std::cout << "ALERT: " << noiseBank[noiseIndex] << rooms[roomIndex].name << "!" << std::endl;
}

// Function to display the building map
/* CONCEPT INCLUDED: for Loops */
void displayMap(Room rooms[], int numRooms) {

	std::cout << std::endl;
	// display header of the map and top boarder
	std::cout << "                         --- BUILDING MAP ---                       " << std::endl;
	std::cout << "╔══════════════════════╦══════════════════════╦══════════════════════╗" << std::endl;

	for (int i = 0; i < numRooms; i++) {

		// start of a new row (Room 1, 4, or 7)
		if (i == 0 || i == 3 || i == 6) {
			std::cout << "║ ";
		}

		// print the room numbers
		std::cout << "[" << (i + 1) << "] ";

		// set the width so the boarders are aligned
		std::cout << std::left << std::setw(16) << rooms[i].name;

		// print the right divider wall
		std::cout << " ║ ";

		// end of a row (Room 3, 6, or 9)
		if (i == 2 || i == 5) {
			std::cout << "\n╠══════════════════════╬══════════════════════╬══════════════════════╣" << std::endl;
		}

		else if (i == 8) {
			std::cout << "\n╚══════════════════════╩══════════════════════╩══════════════════════╝" ;
		}
	}
}

// Function to display art for corresponding location
/* CONCEPT INCLUDED: switch Statements */
void displayRoomArts(int roomNum) {
	switch (roomNum) {
	case 1: // Lobby
		std::cout << R"(
 ________________________________________________________________________
|: : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : |
|__________ : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : :|
|__]\% % % | : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : |
|___]\% % %|: :______ : : : : : : : : : : : : : : : : : : : : :___: : : :|
|____]\% % | :|======| :_: : : : : : . : : : : : :_: : : : : :/   \: : : |
|_____]\% %|: ||\|||||:/_\: :,: : :.'o'.: : :,: :/_\: : : : :|//   |: : :|
|______]\% | :|======| =|= __#_____|___|_____#__ =|= : : : : | ,*, | : : |
|_______]\%|: |______|: ^ :[___]           [___]: ^ : : : : : \*;*/ : : :|
|________]\| :|__  __| : : [_|_] o  `(,  o [_|_] : : : : :_____(_)_____: |
|_________]\: | .||. |: : :[___] |  ( )  | [___]: : :_!_: ||   .|.   || :|
|__________]==|__||__|====_[_|_]/!\_@@@_/!\[_|_]_===/___\=||____|____||==|
     _                    '====================='     | _                |
    |_)         __.;;.__      _______________         |( |               |
    /_\__      / ;(;;); \    (               )       _|_)|               |
  ~=_|_ _)====/__________\==(\               /)=====(____|========~      |
 ~=|___|LL====|==========|===|               |======LLLLLL=========~     |
~============================|_______________|======================~    |
=============================LLLLLLLLLLLLLLLLL=======================~   |
lc====================================================================~  '
        )";
		std::cout << std::endl;
		// https://ascii.co.uk/art/livingroom
		break;

	case 2: // Conference A
	case 3: // Conference B
	case 4: // Conference C
		std::cout << R"(
 ____________________________________________________________________________
|: : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : |
| : : : : : : :_______________________________: : : : : : : : : : : : : : : :|
|: : : : : : :|!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!|: : : : : : : : : : : : : : : |
| : : : : : : |.-=^=-.-=^=-.-=^=-.-=^=-.-=^=-.| : : : : : : : : : : : : : : :|
|: : : : : : :'|    _'     '     '     '     |': : : : : : : : : ____: : : : |
| : : : : : : :|   (  ) _                    |: : : : : : : : : /    \: : : :|
|: : : : : : : |  ( _)__ )_)                 | : : : : : : : : |//    |: : : |
| : : : : : : :|                   \_/       |: : : : : : : : :|      | : : :|
|==_===========|                 --(_)--     |==================\____/=======|
| / \          |                   / \       |              ,    ,;;,    ,   |
|/___\         |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|             _d___;(;;);___b_  |
|  |'          |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|            =======`;;`======= |
|  |      ||   ==============(_)==============   ||         /""""""""""""\   |
|  |      ||        _________~|~_________        ||         |     `(,    |   |
|  |      \\_____  (_____________________)  _____//         |  O  )   O  |   |
|  |       |_____)          )   (          (_____|          |  | (@@) |  |   |
|__|_______||___||__________(   )__________||___||__________|_/!\@@@@/!\_|__lc
  _|_    .;|';;;'|;;;;;;;;;;_) (_;;;;;;;;;;|';;;'|;.       ================
        :;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;:
        :;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;:
        )";
		std::cout << std::endl;
		// https://ascii.co.uk/art/rooms
		break;

	case 5: // Restroom 1
	case 6: // Restroom 2
		std::cout << R"(
 _____________________________________________________________________________
|; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ;|
| ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; |
|; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ;|
| ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; |
|; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ;|
| ;_; ||==========||==========||=============||=============||;_; ; ___ ; ;_; |
|;/_\;||          ||          ||             ||             ||/_\; /   \ ;/_\;|
| =|= ||   ~()    ||   ~()    ||_____________||_____________||=|= | //  | =|= |
|; ; ;||    ||    ||    ||    ||             ||             || ; ;|     |; ; ;|
| ; ; ||    ||    ||    ||    |:]            |:]            ||; ; ;\___/; ; ; |
|=====||  ..||..  ||  ..||..  ||             ||             ||================|
|     || //"II"\\ || //"II"\\ ||           ~:||           ~:||    ___/___     |
|     || || '' || || || '' || ||             ||             ||  / .-T|T-. \   |
|     || ||    || || ||    || ||             ||             || /_'-------'_\  |
|     || ||____|| || ||____|| |:]            |:]            || |  ___ ___  |  |
|     ||  \____/  ||  \____/  ||_____________||_____________|| | |   |   | |  |
|     ||          ||          ||    \| |/    ||    \| |/    || | |  {|}  | |  |
'-----||-.------.-||-.------.-||-.---|_|---.-||-.---|_|---.-||-| |   |   | |--' 
      ||/________\||/________\||/___________\||/___________\lc |_|___|___|_|
        )";
		std::cout << std::endl;
		// https://www.asciiart.eu/art/cacccb57e96dbb85
		break;

	case 7: // Break room
		std::cout << R"(
_____________________________________________________
____________________________________________________\\
|.-------.-------.|_.----._.----._|.-------.-------.\\\
|]       |       [|       .       |]       |       [ \\\
||       |       ||     .':'.     ||       |       |  |\\
||       |       ||    .' : '.    ||       |       |  |\\\
||     (O|O)     ||   .'  :  '.   ||     (O|O)     |  | \\\
||       |       ||  .'===:==='.  ||       |       | O|  |\\
||       |       ||=='    :    '==||       |       |  |  |\\\
|]       |       [|  )    :    (  |]       |       [  |O | \\\
||_______|_______||"" ____:_____""||_______|_______|  |  |  |\\
'-----------------'_______________'----------------'  |  |  |\\\
|.--------.  |    '---------------'  (o)______)(0)  \ |  |  | \\\
||        |::|_________________________________())___\|  | O|  \\\______
||        |::|-----______*!*______-------------))( .'.\  |  |   | _____ |
||________|::|  _ /       '       \  _        _   (__.'\ |  |O  ||     ||
|____________| _  \_______________/     _           (_.'\|  |   ||  _  ||
 ___________________________________________      _  (___\  |   ||     ||
||.-----.|.------.|.-.-.--.--.-.-.||.-----.||\   _        \ |   ||     ||
||| === ||| ==== ||| | |  |  | | |||| === ||| \     _      \|   ||    _||
||'-----'|'------'|'-'-'--'--'-'-'||'-----'||. \          _ \   ||     ||
||.-----.|.------.|.------.------.||.-----.|| `|\       _    \  || _   ||
||| === |||      |||      |      |||| === |||\ | \  _         \ ||_____||
||'-----'|]      ||]      |      [||'-----'|| \|. \        _   \|_______|
||.-----.||    (O|||    (O|O)    |||.-----.||  | `|\                   ||
||| === |||      |||      |      |||| === |||  |\ | \__________________||
|||     ||]      ||]      |      [|||     ||| O| \|. |  _____________  ||
||'-----'||______|||______|______|||'-----'||  |  | `| |             | ||
||LGB____|________|_______________||_______||  |O |\ | |   _         | ||
''-----------------------------------------' \ |  | \| |          _  | ||
   ____                 _______               \|  |  | |       _     | ||
           _________                  ______   \  |O | |             | ||
                                                \ |  | |   _      _  | ||
                                _________        \|  | |             | ||
      ___________        __                       \  | | _        _  | ||
    __                              _________      \ | |_____________| ||
               ___________                          \|_________________||
        )";
		std::cout << std::endl;
		// https://www.asciiart.eu/art/31675c6169b952ac
		break;

	case 8: // Storage room
		std::cout << R"(
                    _______________  _______________
                 .'               .'               .|
               .'               .'               .' |
             .'_______________.'______________ .'   |
             | ___ _____ ___ || ___ _____ ___ |     |
             ||_=_|__=__|_=_||||_=_|__=__|_=_||     |
       ______||_____===_____||||_____===_____||     | __________
    .'       ||_____===_____||||_____===_____||    .'          .'|
  .'         ||_____===_____||||_____===_____||  .'          .'  |
.'___________|_______________||_______________|.'__________.'    |
|.----------.|.-----___-----.||.-----___-----.||    |_____.----------.
|]          |||_____________||||_____________|||  .'      [          |
||          ||.-----___-----.||.-----___-----.||.'        |          |
||          |||_____________||||_____________|||==========|          |
||          ||.-----___-----.||.-----___-----.||    |_____|          |
|]         o|||_____________||||_____________|||  .'      [        'o|
||          ||.-----___-----.||.-----___-----.||.'        |          |
||          |||             ||||_____________|||==========|          |
||          |||             |||.-----___-----.||    |_____|          |
|]          |||             ||||             |||  .'      [          |
||__________|||_____________||||_____________|||.'________|__________|
''----------'''------------------------------'''----------''
            (o)LGB                           (o)
        )";
		std::cout << std::endl;
		// https://www.asciiart.eu/art/6997f00167e6a071
		break;

	case 9: // Security office
		std::cout << R"(
__ _____ ____ _____ ______ _______ _____ ______ ______ ______ ___
__]_____]____]_____]______]_______]_____]______]______]______]___]
             _                       _______  |||"||;;|.||##||=|||
  _                           _     |   *  3| |||-|| =|-||==||+|||
  ____________       _              |       | |||_||__|_||__||_|||
|`.   --__     `.        _______    |       | ||================||
|  `._____________`.  .'|.-----.|   _ ======| ||| | -|&|^^|!!|-|||
|   | .-----------.| |  ||     ||  (o))   _ | ||| |**|=|+-|##|=|||
|   | |  .-------.|| |  ||     ||  /||   / \`._|  .-.|_|__|__|_|||
|   | |  |       |||_`..|'_____'| //||___\_/.'\| (( ))==========||
|   | |`.|  ==== ||| | `---------(o)||         \  /-'-=|+|.-|-'|||
|`. | |`.|_______|||/|______________||__.--._ (o)/|=|;:|-|&&|&&|||
|  `|_|===========||_|                 (____)-.'(o)_|__|_|__|__|||
|   | |  .-------.||                           `._\=============||
|   | |  |       |||                             `.     |       ||
|   | |`.|  ==== |||`._____________________________`.  o|o      ||
|`. | |`.|_______||| |._.----------------.__.-------.|__|_______||
|  `|_|===========|| || '----------------'  | .---. ||  __
|   | |  .-------.|| ||               |     |_______||.'\.'.
|   | |  |       ||| || ______________|     | .---. ||'.__.'
|   | |`.|  ==== ||| ||                `.   |_______|||  _ |
 `. | |`.|_______||| ||                  `. | .---. |||_  ||
   `|_|========LGB||`||                    `|_______|||____|
                       `.                    `.
                         `.____________________`.
        )";
		std::cout << std::endl;
		// https://www.asciiart.eu/art/a1b6b10005b2a27172
		break;
	}
}

// Function to display threat level
void displayThreatLevel(const PlayerStat &player) {

	// display current / max threat level
	std::cout << "\n[ THREAT LEVEL: " << player.threatLevel << " / " << maxThreat << " ]" << std::endl;

	// display extra warning message when threat level is high
	if (player.threatLevel >= 12) {
		std::cout << "WARNING: The intruder is planning to leave the building! Hurry!" << std::endl;
	}
}

// Function to display final summary
void displayFinalSummary(Room building[], int size) {

	// call up function to sort the building array
	sortRoomsByVisits(building, size);

	// display the header
	std::cout << "=====================================================" << std::endl;
	std::cout << "               FINAL PATROL SUMMARY                  " << std::endl;
	std::cout << "           (Locations sorted by visits)              " << std::endl;
	std::cout << "=====================================================" << std::endl;

	// loop through the sorted array and display the statistics
	for (int i = 0; i < size; i++) {

		// calculate total areas cleared for this specific location
		int totalCleared = 0;
		if (building[i].spot1Cleared) {
			totalCleared++;
		}
		if (building[i].spot2Cleared) {
			totalCleared++;
		}
		if (building[i].spot3Cleared) {
			totalCleared++;
		}
		/* CONCEPT INCLUDED: Formatting Output (<iomanip>: setw) */
		std::cout << i + 1 << ". " << std::left << std::setw(15) << building[i].name
		          << " | Visits: " << std::left << std::setw(2) << building[i].timesVisited
		          << " | Searched: " << totalCleared << "/3 Areas" << std::endl;
	}
	std::cout << "=====================================================\n" << std::endl;
}


// --- Utility Functions ---

// Function to sort the locations by visits
/* CONCEPT INCLUDED: Sorting Algorithms (Bubble Sort) */
void sortRoomsByVisits(Room array[], int size) {
	bool swap;
	Room temp;

	do {
		swap = false;
		for (int count = 0; count < (size - 1); count++) {
			// compare timesVisited and perform swap in descending order (most to least visited)
			if (array[count].timesVisited < array[count + 1].timesVisited) {
				temp = array[count];
				array[count] = array[count + 1];
				array[count + 1] = temp;
				swap = true;
			}
		}
	} while (swap); // if swap was true (needed), loop back and check again
}


// Function to find "the location" that the intruder is hiding
/* CONCEPT INCLUDED: Search Algorithms (Linear Search) */
int findIntruderLocation(Room rooms[], int size) {

	int index = 0;           // initialize counter variable
	int position = -1;       // initialize result variable
	bool found = false;      // flag to stop the loop when target found

	// repeat search until intruder is found or the end of array is reached
	while (!found && index < size) {
		if (rooms[index].hasIntruder == true) { // check the struct member
			found = true;                       // set flag to true
			position = index;                   // record the index
		}
		index++; // go to next element
	}

	return position;    // return index number of target (or -1 if not found)
}

// Function to determine "the area" that the intruder is hiding
std::string findIntruderSpot(const Room &location) {

	// If the intruder was in a Conference room
	if (location.type == "conference") {
		if (location.intruderSpot == 'a') {             // intruder hide in spot (a)
			return "under the desks";
		}
		else if (location.intruderSpot == 'b') {        // intruder hide in spot (b)
			return "behind the presentation screen";
		}
		else {                                          // intruder hide in spot (c)
			return "inside the cabinets";
		}
	}
	// If the intruder was in a Restroom
	else if (location.type == "restroom") {
		if (location.intruderSpot == 'a') {
			return "inside the stalls";
		}
		else if (location.intruderSpot == 'b') {
			return "in the janitor's closet";
		}
		else {
			return "inside the air vents";
		}
	}
	// If the intruder was in the Lobby
	else if (location.type == "lobby") {
		if (location.intruderSpot == 'a') {
			return "behind the front desk";
		}
		else if (location.intruderSpot == 'b') {
			return "under the furniture";
		}
		else {
			return "behind the fish tank";
		}
	}
	// If the intruder was in the Break Room
	else if (location.type == "break") {
		if (location.intruderSpot == 'a') {
			return "under the tables";
		}
		else if (location.intruderSpot == 'b') {
			return "behind the vending machine";
		}
		else {
			return "behind the refrigerator";
		}
	}
	// If the intruder was in the Security Office
	else if (location.type == "security") {
		if (location.intruderSpot == 'a') {
			return "inside the lockers";
		}
		else if (location.intruderSpot == 'b') {
			return "on top of the shelves";
		}
		else {
			return "under the console desks";
		}
	}
	// If the intruder was in the Storage Room
	else if (location.type == "storage") {
		if (location.intruderSpot == 'a') {
			return "behind the boxes";
		}
		else if (location.intruderSpot == 'b') {
			return "inside the cabinets";
		}
		else {
			return "on top of the shelves";
		}
	}

	return "unknown"; // if nothing found
}


// --- MINI GAME FUNCTIONS ---

// Function to determine who makes the first move for the Tic-Tac-Toe
bool diceRoll() {

	int playerRoll,     // player's dice roll
	    intruderRoll;   // intruder's dice roll

	const int MAX_DICE = 6; // max random number
	const int MIN_DICE = 1; // min random number

	// clear leftover newline characters before using cin.get()
	// prompt user to roll the dice
	std::cin.ignore(1000, '\n');
	std::cout << "\nRoll the dice to see who makes the first move!" << std::endl;

	// loop for the dice rolling
	do {
		// Player's turn to roll dice
		std::cout << ">> Press [ENTER] to roll your dice";
		std::cin.get();
		playerRoll = (std::rand() % (MAX_DICE - MIN_DICE + 1)) + MIN_DICE;
		std::cout << "\nYou rolled a [" << playerRoll << "]" << std::endl;

		// Intruder's turn to roll dice
		std::cout << "\n>> Press [ENTER] to see the Intruder's roll";
		std::cin.get();
		intruderRoll = (std::rand() % (MAX_DICE - MIN_DICE + 1)) + MIN_DICE;
		std::cout << "\nIntruder rolled a [" << intruderRoll << "]" << std::endl;

		if (playerRoll == intruderRoll) {
			std::cout << "\n>> It's a tie! Roll the dice again..." << std::endl;
		}
	} while (playerRoll == intruderRoll);   // re-roll if result is tie

	// compare determine the result from dice roll
	if (playerRoll > intruderRoll) {
		std::cout << "\nYou beat the intruder! You move first." << std::endl;
		return true;    // to signal the player won
	} else {
		std::cout << "\nThe intruder beat you! They move first." << std::endl;
		return false;   // to signal the intruder won
	}
}

// Function for the Tic-Tac-Toe game
bool ticTacToe() {

	// constant variables for rows and columns
	const int ROWS = 3;
	const int COLS = 3;
	// 2D array of the board
	char board[ROWS][COLS];

	// initialize the board with '*'
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			board[i][j] = '*';
		}
	}

	// turn flag: even numbers = player (x) , odd numbers = intruder (O)
	// if player goes first, turns starts at 0, if intruder goes first, turns starts at 1
	bool isPlayerTurn = diceRoll();
	int turns = isPlayerTurn ? 0 : 1;
	int gameOver = 0;

	// gameplay loop
	while (gameOver == 0) {
		displayBoard(board);

		int row, col;
		char symbol;
		std::string name;

		// assign the player = X, intruder = O
		// player's turn
		if (turns % 2 == 0) {
			name = "You (the guard)";
			symbol = 'X';
			std::cout << "Please enter a row and column: ";
			std::cin >> row >> col;

			// input validation loop for the player
			while (std::cin.fail() || row < 1 || row > 3 || col < 1 || col > 3 || board[row - 1][col - 1] != '*') {
				std::cout << "Invalid input! Please enter a row and column that isn't taken: ";
				std::cin.clear();
				std::cin.ignore(100, '\n');
				std::cin >> row >> col;
			}
		}
		// intruder's turn
		else {
			name = "The Intruder";
			symbol = 'O';
			std::cout << name << " made a move..." << std::endl;

			// flag to determine if an available spot is found
			bool validMove = false;

			const int MAX_MOVE = 3;
			const int MIN_MOVE = 1;

			// repeat until available spot is found
			while (!validMove) {

				// generate random row and column numbers
				row = (std::rand() % (MAX_MOVE - MIN_MOVE + 1)) + MIN_MOVE;
				col = (std::rand() % (MAX_MOVE - MIN_MOVE + 1)) + MIN_MOVE;

				// check if the spot is available
				if (board[row - 1][col - 1] == '*') {
					validMove = true;
				}
			}
		}

		// update the board
		// use -1 for the index numbers
		board[row - 1][col - 1] = symbol;

		// check for Win
		if (checkWin(board, symbol) == 1) {
			displayBoard(board);
			if (symbol == 'X') {
				std::cout << ">> You defeated the Intruder! Backup is on the way, good job!" << std::endl;
				return true; // player won
			} else {
				std::cout << ">> You lost! The intruder knocked you out and escaped with the company's secrets..." << std::endl;
				return false; // player failed
			}
		}
		// check for Tie
		else if (checkTie(board) == 1) {
			displayBoard(board);
			std::cout << ">> It's a tie! The intruder escaped with the company's secrets..." << std::endl;
			std::cout << std::endl ;
			return false; // player failed
		}
		turns++; // increment the variable value to switch turns
	}
	return false;
}

// Function to display the Tic-Tac-Toe board
void displayBoard(char board[3][3]) {

	std::cout << std::endl;

	// outer loop (rows)
	for (int i = 0; i < 3; i++) {
		std::cout << " ";

		// inner loop (columns)
		for (int j = 0; j < 3; j++) {
			std::cout << board[i][j];

			if (j < 2) {
				std::cout << " | ";     // vertical dividers
			}
		}

		std::cout << std::endl;

		if (i < 2) {
			std::cout << "---+---+---" << std::endl;    // horizontal dividers
		}
	}

	std::cout << std::endl;
}

// Function to check if player or intruder wins
int checkWin(char board[3][3], char symbol) {

	// check the rows and columns
	for (int i = 0; i < 3; i++) {
		if ((board[i][0] == symbol && board[i][1] == symbol && board[i][2] == symbol) ||
		        (board[0][i] == symbol && board[1][i] == symbol && board[2][i] == symbol)) {
			return 1;
		}
	}
	// check the diagonals
	if ((board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol) ||
	        (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol)) {
		return 1;
	}
	return 0;
}

// Function to check if tic-tac-toe ends in a tie
int checkTie(char board[3][3]) {

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == '*') {
				return 0;   // board still not full
			}
		}
	}
	return 1;   // board is full (tie)
}

