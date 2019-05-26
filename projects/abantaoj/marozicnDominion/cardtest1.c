/*******************************************************************************
 * cardtest1.c
 *
 * Include the following lines in your makefile:
 * cardtest1: cardtest1.c dominion.o rngs.o
 * gcc -o cardtest1 -g cardtest1.c dominion.o rngs.o $(CFLAGS)
 ******************************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>

void testSmithyCard(struct gameState*, struct gameState*, int[]);
void testCurrentPlayerReceivesThreeCards(struct gameState*, int);
void testCurrentPlayerPileReduced(struct gameState*, int);
void testOtherPlayerUnaffected(struct gameState*, int);
void testOtherPilesUnaffected(struct gameState*, int[]);
void testEmbargoTokensUnaffected(struct gameState*);
int _checkKingdomPile(struct gameState*, int[]);
int _checkEmbargoTokens(struct gameState*);
void _assert(int, int);
void _printPass();
void _printFail(int, int);

int main() {
    int SEED = 100;
    int NUM_PLAYER = 2;
    struct gameState G;
    struct gameState TEST_G;
    int KINGDOM_CARDS[10] = { adventurer, council_room, feast, mine, smithy,
                              great_hall, steward, salvager, minion, sea_hag };

    // Initialize sample game
    initializeGame(NUM_PLAYER, KINGDOM_CARDS, SEED, &G);

    printf("***Card Test 1: Smithy (2 Player Setup)  - Uses refactored smithyCard()***\n");
    testSmithyCard(&G, &TEST_G, KINGDOM_CARDS);
    printf("\n");

    return 0;
}

void testSmithyCard(struct gameState* state, struct gameState* testState, int kCards[]) {
    int currentPlayer = 0;
    int handPos = 0;
    int UNUSED_PARAM = 0;
    int bonus = 0;
    int choice1 = 0;
    int choice2 = 0;
    int choice3 = 0;

    // Make copy of the initialized game
    memcpy(testState, state, sizeof(struct gameState));

    // Give the current player a smithy card on their hand
    testState->hand[currentPlayer][handPos] = smithy;

    // Play the smithy card
    smithyCard(smithy, choice1, choice2, choice3, testState, handPos, &bonus, currentPlayer);

    // Run tests for single execution of smithy card
    testCurrentPlayerReceivesThreeCards(testState, currentPlayer);
    testCurrentPlayerPileReduced(testState, currentPlayer);
    testOtherPlayerUnaffected(testState, currentPlayer + 1);
    testOtherPilesUnaffected(testState, kCards);
    testEmbargoTokensUnaffected(testState);
}

void testCurrentPlayerReceivesThreeCards(struct gameState* state, int currentPlayer) {
    printf("Current player should have seven cards on hand (after smithy is discarded)");

    _assert(state->handCount[currentPlayer], 7);
}

void testCurrentPlayerPileReduced(struct gameState* state, int currentPlayer) {
    printf("Current player's pile should be reduced to two (from original five)");

    _assert(state->deckCount[currentPlayer], 2);
}

void testOtherPlayerUnaffected(struct gameState* state, int otherPlayer) {
    printf("Other player's hand and piles should remain untouched:\n");
    printf("\tDRAW PILE (Hand not yet drawn)");
    _assert(state->deckCount[otherPlayer], 10);
    printf("\tHAND");
    _assert(state->handCount[otherPlayer], 0);
    printf("\tDISCARD PILE");
    _assert(state->discardCount[otherPlayer], 0);
}


void testOtherPilesUnaffected(struct gameState* state, int kCards[]) {
    printf("Kingdom Card piles are unaffected (Victory: 8, Others: 10 for two player game)");
    _assert(_checkKingdomPile(state, kCards), 1);
    printf("Other supply piles are unaffected:\n");
    printf("\tCURSE PILE");
    _assert(state->supplyCount[curse], 10);
    printf("\tCOPPER PILE");
    _assert(state->supplyCount[copper], (60 - (7 * 2)));  // 2 Player Game
    printf("\tSILVER PILE");
    _assert(state->supplyCount[silver], 40);
    printf("\tGOLD PILE");
    _assert(state->supplyCount[gold], 30);
    printf("\tESTATE PILE");
    _assert(state->supplyCount[estate], 8);
    printf("\tDUCHY PILE");
    _assert(state->supplyCount[duchy], 8);
    printf("\tPROVINCE PILE");
    _assert(state->supplyCount[province], 8);
}

void testEmbargoTokensUnaffected(struct gameState* state) {
    printf("No embargo tokens should be set");
    _assert(_checkEmbargoTokens(state), 0);
}

int _checkKingdomPile(struct gameState* state, int kCards[]) {
    int KINGDOM_PILES_PER_GAME = 10;

    for (int i = 0; i < KINGDOM_PILES_PER_GAME; i++) {
        int currCard = kCards[i];
        int currCount = state->supplyCount[currCard];

        if ((currCard == great_hall || currCard == gardens)) {
            if (currCount != 8) {
                return 0;
            }
        } else if (currCount != 10) {
            return 0;
        }
    }

    return 1;
}

int _checkEmbargoTokens(struct gameState* state) {
    for (int i = 0; i <= treasure_map; i++)
    {
        if (state->embargoTokens[i] != 0) {
            return 1;
        };
    }

    return 0;
}


void _assert(int actualCount, int expectedCount) {
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
    printf(" - FAIL (Expected: %d, Actual: %d)\n", expected, actual);
}