/*******************************************************************************
 * cardtest2.c
 *
 * Include the following lines in your makefile:
 * cardtest2: cardtest2.c dominion.o rngs.o
 * gcc -o cardtest2 -g cardtest2.c dominion.o rngs.o $(CFLAGS)
 ******************************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>

int UNUSED_PARAM = 0;

void testAdventurerCard(struct gameState*, int[]);
void testShouldDrawTwoTreasure(struct gameState*);
void testShouldDrawOneTreasure(struct gameState*);
void testShouldShuffleDeck(struct gameState*);
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

    printf("***Card Test 2: Adventurer (2 Player Setup) - Uses refactored playAdventurerCard()***\n");
    testAdventurerCard(&G, KINGDOM_CARDS);
    printf("\n");

    return 0;
}

void testAdventurerCard(struct gameState* state, int kCards[]) {
    /* Card should draw two treasure */
    testShouldDrawTwoTreasure(state);
    /* Card should draw one treasure  */
    testShouldDrawOneTreasure(state);
    /* Should shuffle to find treasure */
    testShouldShuffleDeck(state);
    testOtherPlayerUnaffected(state);
    testOtherPilesUnaffected(state, kCards);
    testEmbargoTokensUnaffected(state);
}

void testShouldDrawTwoTreasure(struct gameState* state) {
    int tempHand[MAX_HAND];
    int currentPlayer = 0;
    int handPos = 0;
    struct gameState testState;

    // Copy state and play adventurer card
    memcpy(&testState, state, sizeof(struct gameState));
    testState.hand[currentPlayer][handPos] = adventurer;
    // Have draw deck contain only two treasure
    testState.deck[currentPlayer][0] = smithy;
    testState.deck[currentPlayer][1] = copper;
    testState.deck[currentPlayer][2] = copper;
    testState.deck[currentPlayer][3] = smithy;
    testState.deck[currentPlayer][4] = smithy;
    // Play card
    playAdventurerCard(currentPlayer, &testState, tempHand);

    printf("Two treasure should have been drawn, placed in hand, and non-treasure discarded\n");
    printf("\tDECK CARDS REMAINING");
    _assert(testState.deckCount[currentPlayer], 2);
    printf("\tHAND COUNT");
    _assert(testState.handCount[currentPlayer], 6);
    printf("\tNUMBER OF CARDS DISCARDED (Adventurer and first non-treasure)");
    _assert(testState.discardCount[currentPlayer], 2);
}

void testShouldDrawOneTreasure(struct gameState* state) {
    int tempHand[MAX_HAND];
    int currentPlayer = 0;
    int handPos = 0;
    struct gameState testState;

    // Copy state and play adventurer card
    memcpy(&testState, state, sizeof(struct gameState));
    testState.hand[currentPlayer][handPos] = adventurer;
    // Have draw deck contain only two treasure
    testState.deck[currentPlayer][0] = smithy;
    testState.deck[currentPlayer][1] = smithy;
    testState.deck[currentPlayer][2] = copper;
    testState.deck[currentPlayer][3] = smithy;
    testState.deck[currentPlayer][4] = smithy;
    // Play card
    playAdventurerCard(currentPlayer, &testState, tempHand);

    printf("One treasure should have been drawn, placed in hand, and two non-treasure discarded\n");
    printf("\tDECK CARDS REMAINING");
    _assert(testState.deckCount[currentPlayer], 2);
    printf("\tHAND COUNT");
    _assert(testState.handCount[currentPlayer], 5);
    printf("\tNUMBER OF CARDS DISCARDED (Adventurer and two non-treasure)");
    _assert(testState.discardCount[currentPlayer], 3);
}


void testShouldShuffleDeck(struct gameState* state) {
    int tempHand[MAX_HAND];
    int currentPlayer = 0;
    int handPos = 0;
    struct gameState testState;

    // Copy state and play adventurer card
    memcpy(&testState, state, sizeof(struct gameState));
    testState.hand[currentPlayer][handPos] = adventurer;
    // Have draw deck contain only two treasure
    testState.deck[currentPlayer][0] = smithy;
    testState.deck[currentPlayer][1] = smithy;
    testState.deck[currentPlayer][2] = smithy;
    testState.deck[currentPlayer][3] = smithy;
    testState.deck[currentPlayer][4] = smithy;

    testState.discard[currentPlayer][0] = copper;
    testState.discard[currentPlayer][1] = copper;
    // Play card
    playAdventurerCard(currentPlayer, &testState, tempHand);

    printf("Two treasure should have been drawn, placed in hand, and non-treasure discarded\n");
    printf("\tDECK CARDS REMAINING");
    _assert(testState.deckCount[currentPlayer], 0);
    printf("\tHAND COUNT");
    _assert(testState.handCount[currentPlayer], 4);
    printf("\tNUMBER OF CARDS DISCARDED (Adventurer and two non-treasure)");
    _assert(testState.discardCount[currentPlayer], 5);
}


void testOtherPlayerUnaffected(struct gameState* state) {
    int tempHand[MAX_HAND];
    int handPos = 0;
    int currentPlayer = 0;
    int otherPlayer = currentPlayer + 1;
    struct gameState testState;

    memcpy(&testState, state, sizeof(struct gameState));
    testState.hand[currentPlayer][handPos] = adventurer;
    playAdventurerCard(currentPlayer, &testState, tempHand);

    printf("Other player's hand and piles should remain untouched:\n");
    printf("\tDRAW PILE (Hand not yet drawn)");
    _assert(testState.deckCount[otherPlayer], 10);
    printf("\tHAND");
    _assert(testState.handCount[otherPlayer], 0);
    printf("\tDISCARD PILE");
    _assert(testState.discardCount[otherPlayer], 0);
}


void testOtherPilesUnaffected(struct gameState* state, int kCards[]) {
    int tempHand[MAX_HAND];
    int handPos = 0;
    int currentPlayer = 0;
    struct gameState testState;

    memcpy(&testState, state, sizeof(struct gameState));
    testState.hand[currentPlayer][handPos] = adventurer;
    playAdventurerCard(currentPlayer, &testState, tempHand);

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
    int tempHand[MAX_HAND];
    int handPos = 0;
    int currentPlayer = 0;
    struct gameState testState;

    memcpy(&testState, state, sizeof(struct gameState));
    testState.hand[currentPlayer][handPos] = adventurer;
    playAdventurerCard(currentPlayer, &testState, tempHand);

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