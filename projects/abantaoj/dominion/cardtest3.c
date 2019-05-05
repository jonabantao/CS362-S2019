/*******************************************************************************
 * cardtest3.c
 *
 * Include the following lines in your makefile:
 * cardtest3: cardtest3.c dominion.o rngs.o
 * gcc -o cardtest3 -g cardtest3.c dominion.o rngs.o $(CFLAGS)
 ******************************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>

int UNUSED_PARAM = 0;

void testOutpostCard(struct gameState*, int[]);
void testShouldBeCurrentPlayerNextTurn(struct gameState*);
void testShouldDrawOnlyThreeCardsNextTurn(struct gameState*);
void testShouldNotBeCurrentPlayerNextTurn(struct gameState*);
void testOtherPlayerUnaffected(struct gameState*);
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
    int KINGDOM_CARDS[10] = { adventurer, council_room, feast, mine, smithy,
                              great_hall, steward, salvager, minion, sea_hag };

    // Initialize sample game
    initializeGame(NUM_PLAYER, KINGDOM_CARDS, SEED, &G);

    printf("***Card Test 3: Outpost (2 Player Setup)***\n");
    testOutpostCard(&G, KINGDOM_CARDS);
    printf("\n");

    return 0;
}

void testOutpostCard(struct gameState* state, int kCards[]) {
    /* Next player turn should be current player when outpost flag not set */
    testShouldBeCurrentPlayerNextTurn(state);
    /* When outpost flag set next player should only draw 3 cards */
    testShouldDrawOnlyThreeCardsNextTurn(state);
    /* Next player should not be current player if outpost flag is set  */
    testShouldNotBeCurrentPlayerNextTurn(state);
    testOtherPlayerUnaffected(state);
    testOtherPilesUnaffected(state, kCards);
    testEmbargoTokensUnaffected(state);
}


void testShouldBeCurrentPlayerNextTurn(struct gameState* state) {
    int currentPlayer = 0;
    int handPos = 0;
    struct gameState testState;

    memcpy(&testState, state, sizeof(struct gameState));
    testState.hand[currentPlayer][handPos] = outpost;
    // Force outpost flag to 0
    testState.outpostPlayed = 0;
    // Play card
    cardEffect(outpost, UNUSED_PARAM, UNUSED_PARAM, UNUSED_PARAM, &testState, handPos, &UNUSED_PARAM);
    // End turn to check next player
    endTurn(&testState);
    int nextPlayer = testState.whoseTurn;

    printf("Next player should be current player again if outpost has not been played twice");
    _assert(nextPlayer, currentPlayer);
}


void testShouldDrawOnlyThreeCardsNextTurn(struct gameState* state) {
    int currentPlayer = 0;
    int handPos = 0;
    struct gameState testState;

    memcpy(&testState, state, sizeof(struct gameState));
    testState.hand[currentPlayer][handPos] = outpost;
    // Force outpost flag to 0
    testState.outpostPlayed = 0;
    // Play card
    cardEffect(outpost, UNUSED_PARAM, UNUSED_PARAM, UNUSED_PARAM, &testState, handPos, &UNUSED_PARAM);
    // End turn to check next player
    endTurn(&testState);
    int nextPlayer = testState.whoseTurn;

    printf("Next player's hand should only contain 3 cards");
    _assert(state->handCount[nextPlayer], 3);
}

void testShouldNotBeCurrentPlayerNextTurn(struct gameState* state) {
    int currentPlayer = 0;
    int handPos = 0;
    struct gameState testState;

    memcpy(&testState, state, sizeof(struct gameState));
    testState.hand[currentPlayer][handPos] = outpost;
    // Force outpost flag to 1
    testState.outpostPlayed = 1;
    // Play card
    cardEffect(outpost, UNUSED_PARAM, UNUSED_PARAM, UNUSED_PARAM, &testState, handPos, &UNUSED_PARAM);
    // End turn to check next player
    endTurn(&testState);
    int nextPlayer = testState.whoseTurn;

    printf("Next player should not be same player if outpost card played by same player two times");
    _assert(nextPlayer, 1);
}


void testOtherPlayerUnaffected(struct gameState* state) {
    int handPos = 0;
    int currentPlayer = 0;
    int otherPlayer = currentPlayer + 1;
    struct gameState testState;

    memcpy(&testState, state, sizeof(struct gameState));
    testState.hand[currentPlayer][handPos] = adventurer;
    cardEffect(adventurer, UNUSED_PARAM, UNUSED_PARAM, UNUSED_PARAM, &testState, handPos, &UNUSED_PARAM);

    printf("Other player's hand and piles should remain untouched:\n");
    printf("\tDRAW PILE (Hand not yet drawn)");
    _assert(testState.deckCount[otherPlayer], 10);
    printf("\tHAND");
    _assert(testState.handCount[otherPlayer], 0);
    printf("\tDISCARD PILE");
    _assert(testState.discardCount[otherPlayer], 0);
}


void testOtherPilesUnaffected(struct gameState* state, int kCards[]) {
    int handPos = 0;
    int currentPlayer = 0;
    struct gameState testState;

    memcpy(&testState, state, sizeof(struct gameState));
    testState.hand[currentPlayer][handPos] = adventurer;
    cardEffect(adventurer, UNUSED_PARAM, UNUSED_PARAM, UNUSED_PARAM, &testState, handPos, &UNUSED_PARAM);

    printf("Kingdom Card piles are unaffected (Victory: 8, Others: 10 for two player game)");
    _assert(_checkKingdomPile(state, kCards), 1);
    printf("Other supply piles are unaffected:\n");
    printf("\tCURSE PILE");
    _assert(testState.supplyCount[curse], 10);
    printf("\tCOPPER PILE");
    _assert(testState.supplyCount[copper], (60 - (7 * 2)));  // 2 Player Game
    printf("\tSILVER PILE");
    _assert(testState.supplyCount[silver], 40);
    printf("\tGOLD PILE");
    _assert(testState.supplyCount[gold], 30);
    printf("\tESTATE PILE");
    _assert(testState.supplyCount[estate], 8);
    printf("\tDUCHY PILE");
    _assert(testState.supplyCount[duchy], 8);
    printf("\tPROVINCE PILE");
    _assert(testState.supplyCount[province], 8);
}

void testEmbargoTokensUnaffected(struct gameState* state) {
    int handPos = 0;
    int currentPlayer = 0;
    struct gameState testState;

    memcpy(&testState, state, sizeof(struct gameState));
    testState.hand[currentPlayer][handPos] = adventurer;
    cardEffect(adventurer, UNUSED_PARAM, UNUSED_PARAM, UNUSED_PARAM, &testState, handPos, &UNUSED_PARAM);

    printf("No embargo tokens should be set");
    _assert(_checkEmbargoTokens(&testState), 0);
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
    printf(" - \033[1;32mPASS\033[0m\n");
}

void _printFail(int expected, int actual) {
    printf(" - \033[0;31mFAIL\033[0m (Expected: %d, Actual: %d)\n", expected, actual);
}