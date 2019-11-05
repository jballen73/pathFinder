#ifndef LOGIC_SEEN
#define LOGIC_SEEN

#include "gba.h"



typedef enum {HPIECE, VPIECE, XPIECE, LUPIECE, URPIECE, RDPIECE, DLPIECE, SOURCE, SINK} PieceType;
typedef enum {DOWN, RIGHT, LEFT, UP} Direction;
typedef struct {
    int xpos;
    int ypos;
} Cursor;
typedef struct Piece Piece;
struct Piece {
    int xpos;
    int ypos;
    int inPlay;
    PieceType type;
    Piece *next;
    Piece *prev;
};
typedef struct {
    Piece *head;
} PieceList;
typedef struct {
    int sourcex;
    int sourcey;
    int sinkx;
    int sinky;
    int pieces[30];
} Level;
typedef struct {
    // Store whether or not the game is over in this member:
    int gameOver;
    Piece *map[15][7];
    PieceList *usedList;
    PieceList *unusedList;
    Piece *selectedPiece;
    Cursor *cursor;
    Level *currentLevel;
    int levelNum;
    Level *levels[20];
    /*
    * TA-TODO: Add any logical elements you need to keep track of in your app.
    *
    * For example, for a Snake game, those could be:
    *
    * Snake snake;
    * Food foods[10];
    * int points;
    *
    */
   
} AppState;

/*
* TA-TODO: Add any additional structs that you need for your app.
*
* For example, for a Snake game, one could be:
*
* typedef struct {
*   int heading;
*   int length;
*   int x;
*   int y;
* } Snake;
*
*/
// This function can initialize an unused AppState struct.
void initializeAppState(AppState *appState);
// This function will be used to process app frames.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow);
// If you have anything else you need accessible from outside the logic.c
// file, you can add them here. You likely won't.
#endif
