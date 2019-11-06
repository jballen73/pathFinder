#include "logic.h"
#include "graphics.h"
#include "gba.h"
#include <stdlib.h>
#include "levels/testLevel.h"
//extern volatile OamEntry* shadow;
static Level** gameLevels;
static Piece** map;
PieceType typeFromInt(int i) {
    switch(i) {
        case 0:
            return HPIECE;
        case 1:
            return VPIECE;
        case 2:
            return XPIECE;
        case 3:
            return LUPIECE;
        case 4:
            return URPIECE;
        case 5:
            return RDPIECE;
        case 6:
            return DLPIECE;
        case 7:
            return SOURCE;
        case 8:
            return SINK;
        case 9:
            return NONE;
        default:
            return NONE;
    }
}
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
        case XPIECE:
            break;
        case NONE:
            break;
        case SOURCE:
            break;
        case SINK:
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
        if (map[x * 15 + y] == NULL) {
            return 0;
        }
        type = map[x * 15 + y]->type;
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
    return 0;
}
void loadLevel(AppState *currentAppState) {
    currentAppState->currentLevel = gameLevels[currentAppState->levelNum];
    Piece *curr = currentAppState->unusedList->head;
    while (curr) {
        removeFromList(curr, currentAppState->unusedList);
        free(curr);
        curr = currentAppState->unusedList->head;
    }
    curr = currentAppState->usedList->head;
    while (curr) {
        removeFromList(curr, currentAppState->usedList);
        free(curr);
        curr = currentAppState->usedList->head;
    }
    for (int i = 0; i < currentAppState->currentLevel->numPieces; i++) {
        curr = malloc(sizeof(Piece));
        curr->id = i+2;
        curr->type = typeFromInt(currentAppState->currentLevel->pieces[i]);
        curr->next = NULL;
        curr->prev = NULL;
        curr->xpos = 0;
        curr->ypos = 0;
        addToList(curr, currentAppState->unusedList);
    }
    for (int i = 0; i < 105; i++) {
        map[i] = NULL;
        
    }
    curr = malloc(sizeof(Piece));
    curr->id = 0;
    curr->type = SOURCE;
    curr->next = NULL;
    curr->prev = NULL;
    curr->xpos = currentAppState->currentLevel->sourcex * 16;
    curr->ypos = currentAppState->currentLevel->sourcey * 16;
    addToList(curr, currentAppState->usedList);
    map[currentAppState->currentLevel->sourcex * 15 + currentAppState->currentLevel->sourcey] = curr;
    curr = malloc(sizeof(Piece));
    curr->id = 1;
    curr->type = SINK;
    curr->next = NULL;
    curr->prev = NULL;
    curr->xpos = currentAppState->currentLevel->sinkx * 16;
    curr->ypos = currentAppState->currentLevel->sinky * 16;
    addToList(curr, currentAppState->usedList);
    map[currentAppState->currentLevel->sinkx * 15 + currentAppState->currentLevel->sinky] = curr;
    currentAppState->selectedPiece = currentAppState->unusedList->head;
    currentAppState->inStash = 1;
}

void initializeAppState(AppState* appState) {
    // TA-TODO: Initialize everything that's part of this AppState struct here.
    // Suppose the struct contains random values, make sure everything gets
    // the value it should have when the app begins.
    Level *testLevel = malloc(sizeof(Level));
    testLevel->numPieces = testLevelNumPieces;
    testLevel->pieces = testLevelPieces;
    testLevel->sinkx = testLevelSinkX;
    testLevel->sinky = testLevelSinkY;
    testLevel->sourcex = testLevelSourceX;
    testLevel->sourcey = testLevelSourceY;

    appState->numLevels = 1;
    gameLevels = malloc((appState->numLevels) * sizeof(Level));
    gameLevels[0] = testLevel;

    Cursor *cursor = malloc(sizeof(Cursor));
    cursor->xpos = 0;
    cursor->ypos = 8;

    PieceList *usedList = malloc(sizeof(PieceList));
    usedList->head = NULL;
    usedList->tail = NULL;

    PieceList *unusedList = malloc(sizeof(PieceList));
    unusedList->head = NULL;
    unusedList->tail = NULL;

    map = malloc(sizeof(Piece) * 7 * 15);
    for (int i = 0; i < 105; i++) {
        map[i] = NULL;
    }
    appState->currentLevel = gameLevels[0];
    appState->levelNum = 0;
    appState->inStash = 1;
    appState->cursor = cursor;
    appState->gameOver = 0;
    appState->nextLevel = 0;
    appState->usedList = usedList;
    appState->unusedList = unusedList;
    loadLevel(appState);
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
            if (currentAppState->selectedPiece) {
                nextAppState.inStash = 0;
                nextAppState.cursor->xpos = 0;
                nextAppState.cursor->ypos = 0;
                while (map[nextAppState.cursor->xpos * 15 + nextAppState.cursor->ypos]) {
                    nextAppState.cursor->xpos += 1;
                    if (nextAppState.cursor->xpos == 15) {
                        nextAppState.cursor->xpos = 0;
                        nextAppState.cursor->ypos += 1;
                    }
                }    
                nextAppState.selectedPiece->xpos = 16 * nextAppState.cursor->xpos;
                nextAppState.selectedPiece->ypos = 16 * nextAppState.cursor->ypos;
                removeFromList(currentAppState->selectedPiece, currentAppState->unusedList);
            }
        } else if (KEY_JUST_PRESSED(BUTTON_UP, keysPressedNow, keysPressedBefore)) {
            nextAppState.selectedPiece = NULL;
            nextAppState.cursor->ypos = 6;
            nextAppState.inStash = 0;
        }
    } else {
        if (KEY_JUST_PRESSED(BUTTON_A, keysPressedNow, keysPressedBefore)) {
            if (currentAppState->selectedPiece) {
                map[currentAppState->cursor->xpos * 15 + currentAppState->cursor->ypos] = currentAppState->selectedPiece;
                addToList(currentAppState->selectedPiece, nextAppState.usedList);
                nextAppState.nextLevel = isSolved(&nextAppState);
                nextAppState.selectedPiece = currentAppState->unusedList->head;
                nextAppState.cursor->xpos = 0;
                nextAppState.cursor->ypos = 8;
                nextAppState.inStash = 1;
            } else {
                Piece *curr = map[currentAppState->cursor->xpos * 15 + currentAppState->cursor->ypos];
                if (curr != NULL) {
                    if (curr->type != SOURCE && curr->type != SINK && curr->type != NONE) {
                        nextAppState.selectedPiece = curr;
                        map[currentAppState->cursor->xpos * 15 + currentAppState->cursor->ypos] = NULL;
                        removeFromList(curr, nextAppState.usedList);
                    }
                }
            }
        } else if (KEY_JUST_PRESSED(BUTTON_B, keysPressedNow, keysPressedBefore)) {
            if (currentAppState->selectedPiece) {
                addToList(currentAppState->selectedPiece, nextAppState.unusedList);
                nextAppState.selectedPiece = nextAppState.unusedList->head;
                nextAppState.cursor->xpos = 0;
                nextAppState.cursor->ypos = 8;
                nextAppState.inStash = 1;
            } else {
                nextAppState.selectedPiece = nextAppState.unusedList->head;
                nextAppState.cursor->xpos = 0;
                nextAppState.cursor->ypos = 8;
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
        } else if (KEY_JUST_PRESSED(BUTTON_RIGHT, keysPressedNow, keysPressedBefore)) {
            if (currentAppState->cursor->xpos < 14) {
                nextAppState.cursor->xpos += 1;
                if (currentAppState->selectedPiece) {
                    nextAppState.selectedPiece->xpos += 16;
                }
            }
        } else if (KEY_JUST_PRESSED(BUTTON_LEFT, keysPressedNow, keysPressedBefore)) {
            if (currentAppState->cursor->xpos > 0) {
                nextAppState.cursor->xpos -= 1;
                if (currentAppState->selectedPiece) {
                    nextAppState.selectedPiece->xpos -= 16;
                }
            }
        } else if (KEY_JUST_PRESSED(BUTTON_UP, keysPressedNow, keysPressedBefore)) {
            if (currentAppState->cursor->ypos > 0) {
                nextAppState.cursor->ypos -= 1;
                if (currentAppState->selectedPiece) {
                    nextAppState.selectedPiece->ypos -= 16;
                }
            }
        } else if (KEY_JUST_PRESSED(BUTTON_DOWN, keysPressedNow, keysPressedBefore)) {
            if (currentAppState->cursor->ypos < 6) {
                nextAppState.cursor->ypos += 1;
                if (currentAppState->selectedPiece) {
                    nextAppState.selectedPiece->ypos += 16;
                }
            } else if (!currentAppState->selectedPiece) {
                nextAppState.selectedPiece = currentAppState->unusedList->head;
                nextAppState.cursor->xpos = 0;
                nextAppState.cursor->ypos = 8;
                nextAppState.inStash = 1;
            }
        }
    }
    return nextAppState;
}
