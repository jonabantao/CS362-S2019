/*******************************************************************************
 * unittest3.c
 *
 * Include the following lines in your makefile:
 * unittest3: unittest3.c dominion.o rngs.o
 * gcc -o unittest3 -g unittest3.c dominion.o rngs.o $(CFLAGS)
 ******************************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>

void testSupplyCount(struct gameState*);
void _checkCorrectCount(struct gameState*, int, int);
void _printPass();
void _printFail(int, int);

int main() {
    int SEED = 100;
    int NUM_PLAYER = 2;
    struct gameState G;
    int KINGDOM_CARDS[10] = { adventurer, council_room, feast, mine, smithy,
                              great_hall, steward, salvager, minion, sea_hag };

    // Initialize sample game
    initializeGame(NUM_PLAYER, KINGDOM_CARDS, SEED, &G);

    printf("***Unit Test 3: supplyCount()***\n");
    testSupplyCount(&G);
    printf("\n");

    return 0;
}

void testSupplyCount(struct gameState* state) {
    printf("Should return the correct supply count:\n");

    // Test each slot instead of selecting a certain card
    // Assuming game stays at 25 cards total
    int TOTAL_CARDS = 25;

    for (int i = 0; i < TOTAL_CARDS; i++) {
        state->supplyCount[i] = i;
    }

    for (int j = 0; j < TOTAL_CARDS; j++) {
        printf("\tSupply deck %d should contain %d cards", j, j);
        _checkCorrectCount(state, j, j);
    }
}


void _checkCorrectCount(struct gameState* state, int cardNum, int expectedCount) {
    int actualCount = supplyCount(cardNum, state);

    if (actualCount == expectedCount) {
        _printPass();
    } else {
        _printFail(expectedCount, actualCount);
    }
}

void _printPass() {
    printf(" - PASS\n");
}

void _printFail(int expected, int actual) {
    printf(" - FAIL (Expected Count: %d, Actual Count: %d)\n", expected, actual);
}