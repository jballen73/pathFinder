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
        } else {
            list->tail = prev;
        }
    } else if (!next) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        next->prev = NULL;
        list->head = next;
    }
    piece->prev = NULL;
    piece->next = NULL;
}
void addToList(Piece *piece, PieceList *list) {
    Piece *prev = list->tail;
    if (!prev) {
        list->head = piece;
        list->tail = piece;
    } else {
        prev->next = piece;
        piece->prev = prev;
        list->tail = piece;
    }
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
            case NONE:
                return 0;
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
    if (currentAppState->inStash) {
        if (KEY_JUST_PRESSED(BUTTON_RIGHT, keysPressedNow, keysPressedBefore)) {
            if (currentAppState->selectedPiece && currentAppState->selectedPiece->next) {
                nextAppState.cursor->xpos += 1;
                if (nextAppState.cursor->xpos == 15) {
                    nextAppState.cursor->ypos += 1;
                    nextAppState.cursor->xpos = 0;
                }
                nextAppState.selectedPiece = currentAppState->selectedPiece->next;
            }
        } else if (KEY_JUST_PRESSED(BUTTON_LEFT, keysPressedNow, keysPressedBefore)) {
            if (currentAppState->selectedPiece && currentAppState->selectedPiece->prev) {
                nextAppState.cursor->xpos -= 1;
                if (nextAppState.cursor->xpos == -1) {
                    nextAppState.cursor->ypos -= 1;
                    nextAppState.cursor->xpos = 14;
                }
                nextAppState.selectedPiece = currentAppState->selectedPiece->prev;
            }
        } else if (KEY_JUST_PRESSED(BUTTON_A, keysPressedNow, keysPressedBefore)) {
            nextAppState.inStash = 0;
            nextAppState.cursor->xpos = 0;
            nextAppState.cursor->ypos = 0;
            while (!currentAppState->map[nextAppState.cursor->xpos][nextAppState.cursor->ypos]) {
                nextAppState.cursor->xpos += 1;
                if (nextAppState.cursor->xpos == 15) {
                    nextAppState.cursor->xpos = 0;
                    nextAppState.cursor->ypos += 1;
                }
            }
            nextAppState.selectedPiece->xpos = 16 * nextAppState.cursor->xpos;
            nextAppState.selectedPiece->ypos = 16 * nextAppState.cursor->ypos;
            removeFromList(currentAppState->selectedPiece, currentAppState->unusedList);
        } else if (KEY_JUST_PRESSED(BUTTON_UP, keysPressedNow, keysPressedBefore)) {
            nextAppState.selectedPiece = NULL;
            nextAppState.cursor->ypos = 114;
            nextAppState.inStash = 0;
        }
    } else {
        if (KEY_JUST_PRESSED(BUTTON_A, keysPressedNow, keysPressedBefore)) {
            if (currentAppState->selectedPiece) {
                nextAppState.map[currentAppState->cursor->xpos][currentAppState->cursor->ypos] = currentAppState->selectedPiece;
                addToList(currentAppState->selectedPiece, nextAppState.usedList);
                nextAppState.nextLevel = isSolved(&nextAppState);
                nextAppState.selectedPiece = currentAppState->unusedList->head;
                nextAppState.cursor->xpos = 0;
                nextAppState.cursor->ypos = 128;
                nextAppState.inStash = 1;
            } else {
                Piece *curr = currentAppState->map[currentAppState->cursor->xpos][currentAppState->cursor->ypos];
                if (curr) {
                    nextAppState.selectedPiece = curr;
                    nextAppState.map[currentAppState->cursor->xpos][currentAppState->cursor->ypos] = NULL;
                    removeFromList(curr, nextAppState.usedList);
                }
            }
        } else if (KEY_JUST_PRESSED(BUTTON_B, keysPressedNow, keysPressedBefore)) {
            if (currentAppState->selectedPiece) {
                addToList(currentAppState->selectedPiece, nextAppState.unusedList);
                nextAppState.selectedPiece = nextAppState.unusedList->head;
                nextAppState.cursor->xpos = 0;
                nextAppState.cursor->ypos = 128;
                nextAppState.inStash = 1;
            } else {
                nextAppState.selectedPiece = nextAppState.unusedList->head;
                nextAppState.cursor->xpos = 0;
                nextAppState.cursor->ypos = 128;
                nextAppState.inStash = 1;
            }
        } else if (KEY_JUST_PRESSED(BUTTON_L, keysPressedNow, keysPressedBefore)) {
            if (currentAppState->selectedPiece) {
                rotatePiece(nextAppState.selectedPiece, 0);
            }
        } else if (KEY_JUST_PRESSED(BUTTON_R, keysPressedNow, keysPressedBefore)) {
            if (currentAppState->selectedPiece) {
                rotatePiece(nextAppState.selectedPiece, 1);
            }
        }
    }
    return nextAppState;
}
