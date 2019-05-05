/*******************************************************************************
 * unittest1.c
 *
 * Include the following lines in your makefile:
 * unittest1: unittest1.c dominion.o rngs.o
 * gcc -o unittest1 -g unittest1.c dominion.o rngs.o $(CFLAGS)
 ******************************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>

void testGameOverEmptyProvince(struct gameState*, struct gameState*);
void testGameOverExistingProvince(struct gameState*, struct gameState*);
void testGameOverThreeSupplyDecksEmpty(struct gameState*, struct gameState*);
void testGameOverLessThanThreeSupplyDecksEmpty(struct gameState*, struct gameState*);
void _checkIfGameOver(struct gameState*, int);
void _printPass();
void _printFail();

int main() {
    int SEED = 100;
    int NUM_PLAYER = 2;
    struct gameState G, TEST_G;
    int KINGDOM_CARDS[10] = { adventurer, council_room, feast, mine, smithy,
                              great_hall, steward, salvager, minion, sea_hag };

    // Initialize sample game
    initializeGame(NUM_PLAYER, KINGDOM_CARDS, SEED, &G);

    printf("***Unit Test 1: isGameOver()***\n");
    testGameOverEmptyProvince(&G, &TEST_G);
    testGameOverExistingProvince(&G, &TEST_G);
    testGameOverThreeSupplyDecksEmpty(&G, &TEST_G);
    testGameOverLessThanThreeSupplyDecksEmpty(&G, &TEST_G);
    printf("\n");

    return 0;
}


void testGameOverEmptyProvince(struct gameState *state, struct gameState *testState) {
    // Create copy of init game to testState
    memcpy(testState, state, sizeof(struct gameState));

    printf("Game should be over if the stack of province cards is empty - ");

    testState->supplyCount[province] = 0;

    _checkIfGameOver(testState, 1);
}

void testGameOverExistingProvince(struct gameState *state, struct gameState *testState) {
    // Create copy of init game to testState
    memcpy(testState, state, sizeof(struct gameState));

    printf("Game should continue if the stack of province cards has at least 1 card remaining - ");

    testState->supplyCount[province] = 3;

    _checkIfGameOver(testState, 0);
}

void testGameOverThreeSupplyDecksEmpty(struct gameState *state, struct gameState *testState) {
    // Create copy of init game to testState
    memcpy(testState, state, sizeof(struct gameState));

    printf("Game should be over if more than three supply decks are empty - ");

    testState->supplyCount[adventurer] = 0;
    testState->supplyCount[minion] = 0;
    testState->supplyCount[silver] = 0;

    _checkIfGameOver(testState, 1);
}

void testGameOverLessThanThreeSupplyDecksEmpty(struct gameState *state, struct gameState *testState) {
    // Create copy of init game to testState
    memcpy(testState, state, sizeof(struct gameState));
    printf("Game should continue if at most two supply decks are empty (excluding Province):\n");

    // Pick some non-Province deck and check gameOver state
    testState->supplyCount[adventurer] = 0;
    printf("\tGame continues when one supply is empty - ");
    _checkIfGameOver(testState, 0);

    // Empty another deck and check gameOver state
    testState->supplyCount[gold] = 0;
    printf("\tGame continues when two supplies are empty - ");
    _checkIfGameOver(testState, 0);
}

void _checkIfGameOver(struct gameState *state, int expectedState) {
    int gameOver = isGameOver(state);

    if (gameOver == expectedState) {
        _printPass();
    } else {
        _printFail();
    }
}

void _printPass() {
    printf("\033[1;32mPASS\033[0m\n");
}

void _printFail() {
    printf("\033[0;31mFAIL\033[0m\n");
}