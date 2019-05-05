/*******************************************************************************
 * unittest4.c
 *
 * Include the following lines in your makefile:
 * unittest4: unittest4.c dominion.o rngs.o
 * gcc -o unittest4 -g unittest4.c dominion.o rngs.o $(CFLAGS)
 ******************************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>

void testWhoseTurnInitGame(struct gameState*, struct gameState*);
void testWhoseTurnNextPlayer(struct gameState*, struct gameState*);
void _checkCorrectPlayer(struct gameState*, int);
void _printPass();
void _printFail();

int main() {
    int SEED = 100;
    int NUM_PLAYER = 4;
    struct gameState G, TEST_G;
    int KINGDOM_CARDS[10] = { adventurer, council_room, feast, mine, smithy,
                              great_hall, steward, salvager, minion, sea_hag };

    // Initialize sample game
    initializeGame(NUM_PLAYER, KINGDOM_CARDS, SEED, &G);

    printf("***Unit Test 4: whoseTurn()***\n");
    testWhoseTurnInitGame(&G, &TEST_G);
    testWhoseTurnNextPlayer(&G, &TEST_G);
    printf("\n");

    return 0;
}

void testWhoseTurnInitGame(struct gameState* state, struct gameState* testState) {
    // Copy init state to test state
    memcpy(testState, state, sizeof(struct gameState));

    printf("Newly initialized game should start with player one");
    _checkCorrectPlayer(testState, 0);
}

void testWhoseTurnNextPlayer(struct gameState* state, struct gameState* testState) {
    // Copy init state to test state
    memcpy(testState, state, sizeof(struct gameState));

    // Replicate endTurn's increment without calling the function
    int currentPlayer = whoseTurn(testState);

    testState->whoseTurn = currentPlayer + 1;

    printf("Game should correctly point to next player when player is incremented");
    _checkCorrectPlayer(testState, 1);
}


void _checkCorrectPlayer(struct gameState* state, int expectedPlayer) {
    int actualPlayer = whoseTurn(state);

    if (actualPlayer == expectedPlayer) {
        _printPass();
    } else {
        _printFail();
    }
}

void _printPass() {
    printf(" - \033[1;32mPASS\033[0m\n");
}

void _printFail() {
    printf(" - \033[0;31mFAIL\033[0m\n");
}
