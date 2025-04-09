// Include standard headers
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <ctime>
using namespace std;

// Include custom headers
#include "./GameSpecs.hpp"
#include "./EnemyAI.hpp"

// Fills the board with the specified tile value (default: WATER).
void wipeBoard(Tile board[BOARD_LENGTH][BOARD_LENGTH], Tile tileValue = WATER);

// Displays both the enemy's and player's boards side by side.
void displayBoards(const Tile enemyBoard[BOARD_LENGTH][BOARD_LENGTH], 
    const Tile playerBoard[BOARD_LENGTH][BOARD_LENGTH], bool showAll = false);

bool placeShipHorizontally(Tile shipType, int shipLength, int row, int col, 
    Tile board[BOARD_LENGTH][BOARD_LENGTH]);

bool placeShipVertically(Tile shipType, int shipLength, int row, int col, 
    Tile board[BOARD_LENGTH][BOARD_LENGTH]);

// Prompts the player to manually place ships on their board.
void placeShipsRandomly(Tile board[BOARD_LENGTH][BOARD_LENGTH]);

// Places ships randomly on the board at valid positions.
void placePlayersShips(Tile playerBoard[BOARD_LENGTH][BOARD_LENGTH]);

// Prototype of function to handle the player's turn
string playersTurn(Tile enemyBoard[BOARD_LENGTH][BOARD_LENGTH], 
    int enemyHitsRemaining[6]);

// Prototype of function to handle the enemy's turn
void enemyTurn(Tile playerBoard[BOARD_LENGTH][BOARD_LENGTH], 
    int playersHitsRemaining[6]);

const int NUM_SHIPS = 12;

// Clears the board by setting all tiles to the initial state 
[BOARD_LENGTH][BOARD_LENGTH]);

/**
 * Displays random battleship coordinates in quick succession to make the
 * illusion that the computer/enemy is thinking about which coordinate to shoot
 * at.
 *
 * After the function returns the last random coordinate is still on the screen,
 * but the cursor is back up so that it can be overwritten with something new.
 */
void randomCoordinatesAnimation();

   // Array is the board for the computers fleet 
   Tile enemyBoard[BOARD_LENGTH][BOARD_LENGTH];

bool isAllZeros(const int HITSREMAINING[], int length);

int main ()
{
    // Ships sizes from Aircraft Carrier to Patrol Boat
    int hitsRemaining[12] = {0, 5, 4, 3, 3, 2, 5, 4, 3, 3, 2, 0};

    // Same for the player, initialized with ship sizes
    int playersHitsRemaining[12] = {0, 5, 4, 3, 3, 2, 5, 4, 3, 3, 2, 0};

    // Seed the random number generator
    srand(time(0));
    
    // Array is the board for the player's fleet 
    Tile playerBoard[BOARD_LENGTH][BOARD_LENGTH];

    // Set all tiles to WATER for the player's board
    wipeBoard(playerBoard);

    // Set all tiles to WATER for the enemy's board
    wipeBoard(enemyBoard);

    // Place ships randomly on the enemy's board
    placeShipsRandomly(enemyBoard);

    // Place the player's ships
    placePlayersShips(playerBoard);

    // Display both boards
    displayBoards(enemyBoard, playerBoard, true);

    // Game loop: Player and enemy take turns until one side sinks all ships
    do {
        // Player's turn: make a shot and display message
        playersTurn(enemyBoard, hitsRemaining);
        cout << "Your shot: " << endl;
        displayBoards(enemyBoard, playerBoard, true);  

        // Check if all enemy ships are sunk
        if (isAllZeros(hitsRemaining, NUM_SHIPS)) 
        {
            cout << "Congratulations! You have sunk all enemy ships!" << endl;
            break;  // Player wins, exit loop
        }

        // Enemy's turn: enemy makes a shot and display message
        enemyTurn(playerBoard, playersHitsRemaining);
        cout << "Enemy's shot: " << endl;  
        displayBoards(enemyBoard, playerBoard, true); 

        // Check if all player's ships are sunk
        if (isAllZeros(playersHitsRemaining, NUM_SHIPS)) {
            cout << "Game Over! The enemy has sunk all your ships!" << endl;
            break;  // Enemy wins, exit loop
        }

    } while (true);  // Continue looping until a winner is determined

    // After the loop, display the final state of the boards with all ships 
    // revealed
    displayBoards(enemyBoard, playerBoard, false);

    return 0;
}

// Function to check if all elements in the array are zero (all ships sunk)
bool isAllZeros(const int HITSREMAINING[], int length) 
{
    for (int index = 0; index < length; ++index) 
    {
        if (HITSREMAINING[index] > 0) {

            return false;  // There are still ships with remaining hits
        }
    }
    return true;  // All ships have been sunk
}

// Wipes the board by setting all tiles to water (0)
void wipeBoard(int board[BOARD_LENGTH][BOARD_LENGTH]) 
{
    for (int index = 0; index < BOARD_LENGTH; ++index) 
    {
        for (int iter = 0; iter < BOARD_LENGTH; ++iter) 
        {
            board[index][iter] = 0;  // 0 represents water
        }
    }
}
void enemyTurn(Tile playerBoard[BOARD_LENGTH][BOARD_LENGTH], 
    int playersHitsRemaining[6]) 
{
    string message;
    char rowLetter;
    int columnNumber;
    bool validShot = false;

    do 
    {
        // Randomly pick row and column for attack
        rowLetter = rand() % BOARD_LENGTH + 'A';  // Random row (A - H)
        columnNumber = rand() % BOARD_LENGTH + 1;  // Random column (1 - 10)

        // Convert row letter to index
        int rowIndex = rowLetter - 'A';

        // Convert column to zero-based index
        int columnIndex = columnNumber - 1;

        // Check if the tile has already been shot at
        if (rowIndex < 0 || rowIndex >= BOARD_LENGTH || columnIndex < 0 || 
            columnIndex >= BOARD_LENGTH) 
        {
            message = "Invalid coordinates! Please try again.";
            continue;
        }

        if (enemyBoard[rowIndex][columnIndex] == HIT || 
            enemyBoard[rowIndex][columnIndex] == MISS)
        {
            message = "You already attacked this spot! Try again.";
            continue;
        }

        if (playerBoard[rowIndex][columnIndex] != WATER) 
        {
            Tile shipType = playerBoard[rowIndex][columnIndex];
            switch (shipType) 
            {
                case AIRCRAFT_CARRIER:
                    playerBoard[rowIndex][columnIndex] = AIRCRAFT_CARRIER_HIT;
                    break;
                case BATTLESHIP:
                    playerBoard[rowIndex][columnIndex] = BATTLESHIP_HIT;
                    break;
                case SUBMARINE:
                    playerBoard[rowIndex][columnIndex] = SUBMARINE_HIT;
                    break;
                case DESTROYER:
                    playerBoard[rowIndex][columnIndex] = DESTROYER_HIT;
                    break;
                case PATROL_BOAT:
                    playerBoard[rowIndex][columnIndex] = PATROL_BOAT_HIT;
                    break;
                default:
                    break;
            }

            message = "You hit an enemy ship!";
            int shipIndex = static_cast<int>(shipType);
            playersHitsRemaining[shipIndex]--;  

            if (playersHitsRemaining[shipIndex] == 0) 
            {
                message += " You sunk the enemy's " + shipToString(shipType) + 
                    "!\n";
            }
        } 
        else 
        {
            playerBoard[rowIndex][columnIndex] = MISS;
            message = "You missed!";
        }

        validShot = true;
    } 
    while (!validShot);

    cout << message << endl;
}

string playersTurn(Tile enemyBoard[BOARD_LENGTH][BOARD_LENGTH], 
    int enemyHitsRemaining[6]) 
{
    string message;
    char rowLetter;
    int columnNumber;
    bool validShot = false;

    do 
    {
        // Ask player for coordinates
        cout << "Enter the coordinates for your shot (e.g. B2): ";
        cin >> rowLetter >> columnNumber;

        // Random row (A - H)
        rowLetter = rand() % BOARD_LENGTH + 'A';

        // Random column (1 - 10)
        columnNumber = rand() % BOARD_LENGTH + 1;

        // Convert row letter to index
        int rowIndex = rowLetter - 'A';

        // Convert column to zero-based index
        int columnIndex = columnNumber - 1;

        // Check if the tile has already been shot at
        if (rowIndex < 0 || rowIndex >= BOARD_LENGTH || columnIndex < 0 || 
            columnIndex >= BOARD_LENGTH) 
        {
            message = "Invalid coordinates! Please try again.";
            continue;
        }

        if (enemyBoard[rowIndex][columnIndex] == HIT || 
            enemyBoard[rowIndex][columnIndex] == MISS)
        {
            message = "You already attacked this spot! Try again.";
            continue;
        }

        if (enemyBoard[rowIndex][columnIndex] != WATER) 
        {
            // It's a hit
            enemyBoard[rowIndex][columnIndex] = HIT;
            message = "You hit an enemy ship!";

            // Decrement the number of hits remaining for the ship
            Tile shipType = enemyBoard[rowIndex][columnIndex];
            int shipIndex = static_cast<int>(shipType);
            enemyHitsRemaining[shipIndex]--;  

            if (enemyHitsRemaining[shipIndex] == 0) 
            {
                message += " You sunk the enemy's " + shipToString(shipType) + 
                    "\n";
            }
        } 
        else 
        {
            // It's a miss
            enemyBoard[rowIndex][columnIndex] = MISS;
            message = string(1, rowLetter) + to_string(columnNumber) + 
                " Miss.";  
        }

        validShot = true;
    } 
    while (!validShot);

    return message;
}

// Function that determines a hit or miss
char tileToSymbol(const Tile& tile) 
{
    switch(tile) 
    {
        case WATER: 
            return '.';
        case AIRCRAFT_CARRIER:
            return 'A';
        case BATTLESHIP:
            return 'B';
        case SUBMARINE:
            return 'S';
        case DESTROYER:
            return 'D';
        case PATROL_BOAT:
            return 'P';
        case MISS: 
            return '~';

            // For hit versions of ships, return the lowercase letter of the 
            // ship
        case AIRCRAFT_CARRIER_HIT: 
            return 'a';  
        case BATTLESHIP_HIT: 
            return 'b';  
        case SUBMARINE_HIT: 
            return 's';  
        case DESTROYER_HIT: 
            return 'd';  
        case PATROL_BOAT_HIT: 
            return 'p';  

        default:
            return ' ';   // Invalid or unknown tile case
            
            // Handle invalid tile
            cout << "Error: Invalid Tile" << endl;
            
            // Return a blank space for invalid tiles
            return ' ';
    }
}

// Function returns the name of the ship 
string shipToString(Tile tile) 
{
    switch(tile) 
    {
        case WATER: 
            return "Water";  
        case AIRCRAFT_CARRIER:
        case AIRCRAFT_CARRIER_HIT: 
            return "Aircraft Carrier";  
        case BATTLESHIP:
        case BATTLESHIP_HIT: 
            return "Battleship";  
        case SUBMARINE:
        case SUBMARINE_HIT: 
            return "Submarine";  
        case DESTROYER:
        case DESTROYER_HIT: 
            return "Destroyer";  
        case PATROL_BOAT:
        case PATROL_BOAT_HIT: 
            return "Patrol Boat";  
        case MISS:
            return "Miss";
        default:
            return "Error";
    }
}

// Function to wipe the board and set all values to the given Tile
void wipeBoard(Tile board[BOARD_LENGTH][BOARD_LENGTH], Tile tileValue) 
{
    for (int row = 0; row < BOARD_LENGTH; ++row) 
    {
        for (int col = 0; col < BOARD_LENGTH; ++col) 
        {
            board[row][col] = tileValue;
        }
    }
}

// Function to display both boards
void displayBoards(const Tile enemyBoard[BOARD_LENGTH][BOARD_LENGTH], 
    const Tile playerBoard[BOARD_LENGTH][BOARD_LENGTH], bool showAll)
{
    // Print the message "Enemy's Fleet           Your Fleet" on the same line
    cout << "  Enemy's Fleet           Your Fleet" << endl;

    // Print column headers for both boards
    cout << "  ";

    for (int col = 1; col <= BOARD_LENGTH; ++col)
    {
        if (col == 10) 
        {
            // No space between 9 and 10 in the enemy board column headers
            cout << col;
        } 
        else 
        {
            // Regular columns with space between them
            cout << setw(2) << col;
        }
    }

    // Space between both boards
    cout << "    ";

    for (int col = 1; col <= BOARD_LENGTH; ++col)
    {
        if (col == 10) 
        {
            // No space after the last column number for the player's board
            cout << col;
        } 
        else 
        {
            // Regular columns with space between them
            cout << setw(2) << col;
        }
    }

    cout << endl;

    // Print the boards row by row
    for (int row = 0; row < BOARD_LENGTH; ++row)
    {
        // Print the row letter for enemys board
        cout << " " << char('A' + row) << " ";

        // Print the enemy's board (only show hits, misses, and unknowns)
        for (int col = 0; col < BOARD_LENGTH; ++col)
        {
            if (showAll) 
            {
                // Show the full board if showAll is true
                cout << setw(1) << tileToSymbol(enemyBoard[row][col]) << " ";
            } else 
            {
                // Otherwise, show only hits (X), misses (~), and unknowns (.)
                if (enemyBoard[row][col] == MISS) 
                {
                    cout << setw(1) << "~ ";  // Miss
                } 
                else if (enemyBoard[row][col] == WATER) 
                {
                    cout << setw(1) << ". ";  // Unknown
                } 
                // Check for hits
                else if 
                    (
                    enemyBoard[row][col] == AIRCRAFT_CARRIER_HIT || 
                    enemyBoard[row][col] == BATTLESHIP_HIT || 
                    enemyBoard[row][col] == SUBMARINE_HIT || 
                    enemyBoard[row][col] == DESTROYER_HIT || 
                    enemyBoard[row][col] == PATROL_BOAT_HIT)
                {
                    cout << setw(1) << "X ";  // Hit
                }
                else 
                {
                    // This else case handles any other non-revealed ship tiles 
                    // (not hit)
                    cout << setw(1) << ". ";  // Unknown
                }
            }
        }

        cout << " ";

        // Print the row letter for players board
        cout << " " << char('A' + row) << " ";

        // Print the player's board (show the fleet, hits, and misses)
        for (int col = 0; col < BOARD_LENGTH; ++col)
        {
            if (col == BOARD_LENGTH - 1) 
            {
                // No space after the last column for the player's board
                cout << setw(1) << tileToSymbol(playerBoard[row][col]); 
            } 
            else 
            {
                // Regular space after columns for the player's board
                cout << setw(1) << tileToSymbol(playerBoard[row][col]) << " "; 
            }
        }

        cout << endl;  // Move to the next row
    }
}
// Function to place a ship horizontally
bool placeShipHorizontally(Tile shipType, int shipLength, int row, int col, Tile
    board[BOARD_LENGTH][BOARD_LENGTH]) 
{
    if (col < 0 || col + shipLength > BOARD_LENGTH || row < 0 || row >= 
        BOARD_LENGTH) 
    {
        return false;  // Ship would go out of bounds
    }

    // Check if the space is already occupied by another ship
    for (int columnIndex = 0; columnIndex < shipLength; ++columnIndex) 
    {
        if (board[row][col + columnIndex] != WATER) 
        {
            return false;  // Space is already occupied
        }
    }

    // Place the ship on the board
    for (int columnIndex = 0; columnIndex < shipLength; ++columnIndex) 
    {
        // Set the ship's type on the board
        board[row][col + columnIndex] = shipType;
    }

    return true;  // Ship successfully placed
}

// Function to place a ship vertically
bool placeShipVertically(Tile shipType, int shipLength, int row, int col, Tile 
    board[BOARD_LENGTH][BOARD_LENGTH]) 
{
    if (row < 0 || row + shipLength > BOARD_LENGTH || col < 0 || col >= 
        BOARD_LENGTH) 
    {
        return false;  // Ship would go out of bounds
    }

    // Check if the space is already occupied by another ship
    for (int rowIndex = 0; rowIndex < shipLength; ++rowIndex) 
    {
        if (board[row + rowIndex][col] != WATER) 
        {
            return false;  // Space is already occupied
        }
    }

    // Place the ship on the board
    for (int rowIndex = 0; rowIndex < shipLength; ++rowIndex) 
    {
        // Set the ship's type on the board
        board[row + rowIndex][col] = shipType;
    }

    return true;  // Ship successfully placed
}

// Function to place ships randomly on the board
void placeShipsRandomly(Tile board[BOARD_LENGTH][BOARD_LENGTH]) 
{
    // Set the board to WATER initially
    wipeBoard(board, WATER);

    // Ship sizes (from largest to smallest)
 const int SHIPSIZES[] = {5, 4, 3, 3, 2};  // Aircraft Carrier, Battleship, 
    // etc.
     const Tile shipTypes[] = {AIRCRAFT_CARRIER, BATTLESHIP, SUBMARINE, 
    DESTROYER, PATROL_BOAT};

    // Try to place each ship randomly
    for (int shipIndex = 0; shipIndex < 5; ++shipIndex) 
    {
        bool placed = false;
        while (!placed) 
        {
            // Generate random orientation (0 = horizontal, 1 = vertical)
            bool isHorizontal = rand() % 2 == 0;

            // Generate random row and column
            int row = rand() % BOARD_LENGTH;
            int col = rand() % BOARD_LENGTH;

            if (isHorizontal)
            {
                placed = placeShipHorizontally(shipTypes[shipIndex], 
                                        SHIPSIZES[shipIndex], row, col, board);
             } 
            else 
            {
                placed = placeShipVertically(shipTypes[shipIndex], 
                                        SHIPSIZES[shipIndex], row, col, board);
             }
        }
    }
}

// Function to place the player's ships
void placePlayersShips(Tile playerBoard[BOARD_LENGTH][BOARD_LENGTH]) 
{
    // Place all ships randomly on the player's board
    placeShipsRandomly(playerBoard);

    // Display both boards
    displayBoards(playerBoard, playerBoard, true);

    // Ask the user if they want to play with this board layout
    char choice;
    do {
        cout << "Do you want to play with this board? (y/n): ";
        cin >> choice;

        // Ignore invalid input until 'y', 'Y', 'n', or 'N' is entered
        while (cin.fail() || (choice != 'y' && choice != 'Y' && choice != 'n' &&
                             choice != 'N')) 
         {
            cout << "Please enter 'y' or 'n': ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
             cin >> choice;
        }

    } while (choice == 'n' || choice == 'N');
}

void randomCoordinatesAnimation()
{
    // Variables to hold random coordinates to print
    int colNumber;
    char rowLetter;

    cout << left; // left align columns

    // Repeatedly output a random coordinate, then wait, then backup to 
    // overwrite the pauses increase in time exponentially until it is >= 
    // 160 msecs
    for (int msecs = 10; msecs < 160; msecs = static_cast<int>(msecs * 1.1))
    {
        // Get random numbers
        rowLetter = rand() % BOARD_LENGTH + 'A';
        colNumber = rand() % (BOARD_LENGTH) + 1;

        // Output numbers
        cout << rowLetter << colNumber << flush;

        // Wait the alloted time
        this_thread::sleep_for(chrono::milliseconds(msecs));

        // Backup for next output.
        cout << "\b\b\b";
    }

    cout << right; // reset alignment back to the default
}

