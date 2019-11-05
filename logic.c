#include "logic.h"
#include "graphics.h"
#include "gba.h"
#include <stdlib.h>
//extern volatile OamEntry* shadow;
void removeFromList(Piece *piece, PieceList *list) {
    Piece *prev = piece->prev;
    Piece *next = piece->next;
    if (prev) {
        prev->next = next;
        if (next) {
            next->prev = prev;
        }
    } else if (!next) {
        list->head = NULL;
    }
    piece->prev = NULL;
    piece->next = NULL;
}
void rotatePiece(Piece *piece, int r) {
    switch(piece->type) {
        case VPIECE:
            piece->type = HPIECE;
            break;
        case HPIECE:
            piece->type = VPIECE;
            break;
        case LUPIECE:
            piece->type = r ? URPIECE : DLPIECE;
            break;
        case URPIECE:
            piece->type = r ? RDPIECE : LUPIECE;
            break;
        case RDPIECE:
            piece->type = r ? DLPIECE : URPIECE;
            break;
        case DLPIECE:
            piece->type = r ? LUPIECE : RDPIECE;
            break;

    }
}
void successor(int *px, int *py, Direction d) {
    switch(d) {
        case UP:
            *py -= 1;
            break;
        case DOWN:
            *py += 1;
            break;
        case RIGHT:
            *px += 1;
            break;
        case LEFT:
            *px -= 1;
            break;
    }
}
int isSolved(AppState *currentAppState) {
    int x = currentAppState->currentLevel->sourcex;
    int y = currentAppState->currentLevel->sourcey;
    PieceType type = SOURCE;
    Direction direction = RIGHT;
    while(type != SINK) {
        successor(&x, &y, direction);
        if (x < 0 || x > 14 || y < 0 || y > 6) {
            return 0;
        }
        type = currentAppState->map[x][y]->type;
        switch (type) {
            case SINK:
                if (direction == RIGHT) {
                    return 1;
                } else {
                    return 0;
                }
            case SOURCE:
                return 0;
            case XPIECE:
                break;
            case HPIECE:
                if (direction == LEFT || direction == RIGHT) {
                    break;
                } else {
                    return 0;
                }
            case VPIECE:
                if (direction == DOWN || direction == UP) {
                    break;
                } else {
                    return 0;
                }
            case LUPIECE:
                if (direction == RIGHT) {
                    direction = UP;
                    break;
                } else if (direction == DOWN) {
                    direction = LEFT;
                    break;
                } else {
                    return 0;
                }
            case URPIECE:
                if (direction == DOWN) {
                    direction = RIGHT;
                    break;
                } else if (direction == LEFT) {
                    direction = UP;
                    break;
                } else {
                    return 0;
                }
            case RDPIECE:
                if (direction == LEFT) {
                    direction = DOWN;
                    break;
                } else if (direction == UP) {
                    direction = RIGHT;
                    break;
                } else {
                    return 0;
                }
            case DLPIECE:
                if (direction == UP) {
                    direction = LEFT;
                    break;
                } else if (direction == RIGHT) {
                    direction = DOWN;
                    break;
                } else {
                    return 0;
                }

        }
    }
}
void initializeAppState(AppState* appState) {
    // TA-TODO: Initialize everything that's part of this AppState struct here.
    // Suppose the struct contains random values, make sure everything gets
    // the value it should have when the app begins.
    
}



// This function processes your current app state and returns the new (i.e. next)
// state of your application.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow) {
    AppState nextAppState = *currentAppState;
    
    return nextAppState;
}
