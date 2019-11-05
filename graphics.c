#include "graphics.h"
#include "gba.h"
#include "images/sprites.h"
#include "logic.h"
#include <stdio.h>
#include <stdlib.h>
// TA-TODO: Include any header files for images generated by nin10kit.
// Example for the provided garbage image:
//#include "images/garbage.h"
static void drawSprites(void);
char* itoa(int, char* , int);
volatile OamEntry shadow[128];
//volatile OamEntry* playerCharacter = &shadow[0];

void hideSprites(void) {
    for(int i = 0; i < 128; i++) {
        shadow[i].attr0 = ATTR0_HIDE;
    }
    drawSprites();
}
void graphicsInit(void) {
    DMA[3].src = sprites_palette;
    DMA[3].dst = SPRITEPAL;
    DMA[3].cnt = SPRITES_PALETTE_LENGTH | DMA_ON;

    DMA[3].src = sprites;
    DMA[3].dst = &CHARBLOCKBASE[5];
    DMA[3].cnt = SPRITES_LENGTH | DMA_ON;


    for(int i = 0; i < 128; i++) {
        shadow[i].attr0 = ATTR0_HIDE;
    }
    //playerCharacter = &shadow[0];
    // playerCharacter->attr0 = 50 | SPRITES_PALETTE_TYPE | PLAYERCHARACTERSPRITE_SPRITE_SHAPE;
    // playerCharacter->attr1 = 50 | PLAYERCHARACTERSPRITE_SPRITE_SIZE;
    //playerCharacter->attr2 = PLAYERCHARACTERSPRITE_PALETTE_ID | PLAYERCHARACTERSPRITE_ID;

    
}
void drawPiece(Piece *piece, int x, int y) {

}
void drawPieces(AppState *state) {
    Piece *curr = state->usedList->head;
    while (curr) {
        drawPiece(curr, curr->xpos, curr->ypos);
        curr = curr->next;
    }
    curr = state->unusedList->head;
    int x = 0;
    int y = 128;
    while (curr) {
        drawPiece(curr, x, y);
        x += 16;
        if (x == 240) {
            x = 0;
            y += 16;
        }
        curr = curr->next;
    }
    if (!state->inStash && state->selectedPiece) {
        drawPiece(state->selectedPiece, state->selectedPiece->xpos, state->selectedPiece->ypos);
    }
}
// TA-TODO: Add any draw/undraw functions for sub-elements of your app here.
// For example, for a snake game, you could have a drawSnake function
// or a drawFood function
//
// e.g.:
// static void drawSnake(Snake* snake);
// static void drawFood(Food* food);

/*
static void drawShot0(int xpos, int ypos, int direction) {
    playerShot0->attr0 = (ypos&0x00FF) | SPRITES_PALETTE_TYPE | PLAYERSHOT_SPRITE_SHAPE;
    if (direction){
        playerShot0->attr1 = (xpos&0x00FF) | PLAYERSHOT_SPRITE_SIZE; 
    } else {
        playerShot0->attr1 = (xpos&0x00FF) | PLAYERSHOT_SPRITE_SIZE | ATTR1_HFLIP; 
    }
}
*/
static void drawSprites(void) {
    DMA[3].src = shadow;
    DMA[3].dst = OAMMEM;
    DMA[3].cnt = 128*4 | DMA_ON;
}
// This function will be used to draw everything about the app
// including the background and whatnot.
void fullDrawAppState(AppState *state) {
    // TA-TODO: IMPLEMENT.
    UNUSED(state);
}

// This function will be used to undraw (i.e. erase) things that might
// move in a frame. E.g. in a Snake game, erase the Snake, the food & the score.
void undrawAppState(AppState *state) {
    // TA-TODO: IMPLEMENT.
    UNUSED(state);
}

// This function will be used to draw things that might have moved in a frame.
// For example, in a Snake game, draw the snake, the food, the score.
void drawAppState(AppState *state) {
    // TA-TODO: IMPLEMENT.
    //if (state->levelChange > 0) {

       // state->levelChange--;
    //}
    UNUSED(state);
}

