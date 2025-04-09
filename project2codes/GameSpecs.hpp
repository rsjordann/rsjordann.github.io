/*
 * This file holds global constants, data types, and their associated helper
 * functions for the game of battleship.
 */
#ifndef ___GAME_SPECS___
#define ___GAME_SPECS___

#include <string>
using namespace std;

// Put new code here.

// Stores ship placement and hit/miss information
enum Tile 
{
    WATER, 
    AIRCRAFT_CARRIER, 
    BATTLESHIP, 
    SUBMARINE, 
    DESTROYER, 
    PATROL_BOAT,
    MISS, 
    AIRCRAFT_CARRIER_HIT, 
    BATTLESHIP_HIT, 
    SUBMARINE_HIT, 
    DESTROYER_HIT, 
    PATROL_BOAT_HIT,
    HIT
};

// Height and width of each board
const int BOARD_LENGTH = 10;

// Function declaration that returns a character symbol for the given Tile
char tileToSymbol(const Tile& tile); 

// Function declartion that returns the name of the ship corresponding to the 
// given Tile
string shipToString(Tile tile);

// Sizes of each ship
const int SHIP_SIZE[] = 
{
    0,  // WATER
        5,  // AIRCRAFT_CARRIER
        4,  // BATTLESHIP
        3,  // SUBMARINE
        3,  // DESTROYER
        2   // PATROL_BOAT
};

#endif
