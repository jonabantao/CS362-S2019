#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "rngs.h"

int testAdventurerCard(struct gameState*, int, int);

int main()
{
    srand(time(0));

    int TEST_COUNT = 600;
    int passedTests = 0;
    int printMax = 60;
    int printCount = 0;

    struct gameState initState, testState;

    int KINGDOM_CARDS[10] = { adventurer, council_room, feast, mine, smithy,
                              great_hall, steward, salvager, minion, sea_hag };

    printf("Random Test: Adventurer Card\n");

    for (int i = 0; i < TEST_COUNT; i++)
    {
        memcpy(&testState, &initState, sizeof(struct gameState));
        int SEED = (rand() % 1000) + 1;
        int PLAYERS = (rand() % (MAX_PLAYERS - 2)) + 2;

        initializeGame(PLAYERS, KINGDOM_CARDS, SEED, &testState);

        int currPlayer = testState.whoseTurn;
        int currHandCount = testState.handCount[currPlayer];
        int randomHandPos = rand() % currHandCount;
        testState.hand[currPlayer][randomHandPos] = adventurer;

        // Initialize deck of cards and randomize positions of treasures
        int deckCount = testState.deckCount[currPlayer];

        for (int j = 0; j < deckCount; j++)
        {
            int cardSelection = rand() % 6;

            switch (cardSelection)
            {
                case 0:
                    testState.deck[currPlayer][j] = copper;
                    break;
                case 1:
                    testState.deck[currPlayer][j] = silver;
                    break;
                case 2:
                    testState.deck[currPlayer][j] = gold;
                    break;
                default: // Non treasure dummy card
                    testState.deck[currPlayer][j] = feast;
            }
        }

        int result = testAdventurerCard(&testState, currPlayer, randomHandPos);

        if (printCount == printMax)
        {
            printf("\n");
            printCount = 0;
        }

        if (result == 1)
        {
            passedTests++;
            printf(".");
        }
        else
        {
            printf("X");
        }
        printCount++;
    }

    printf("\n");

    if (TEST_COUNT - passedTests == 0)
    {
        printf("ALL TESTS PASSED.\n\n");
    }
    else
    {
        printf("%d out of %d tests passed.\n\n", passedTests, TEST_COUNT);
    }

    return 0;
}

int testAdventurerCard(struct gameState* state, int currPlayer, int handPos)
{
    int hasPassedTests = 1;
    int deckTreasureCount = 0;
    int cardsToDiscardBeforeTreasureDrawn = 0;
    int prevDiscardCount = state->discardCount[currPlayer];
    int prevTreasuresOnHand = 0;
    int currTreasuresOnHand = 0;

    for (int i = 0; i < state->deckCount[currPlayer]; i++)
    {
        int deckCardType = state->deck[currPlayer][i];

        switch (deckCardType)
        {
            case copper:
            case silver:
            case gold:
                deckTreasureCount++;
                break;
            default:
                cardsToDiscardBeforeTreasureDrawn++;
        }
    }

    for (int j = 0; j < state->handCount[currPlayer]; j++)
    {
        int handCardType = state->hand[currPlayer][j];

        switch (handCardType)
        {
            case copper:
            case silver:
            case gold:
                prevTreasuresOnHand++;
                break;
            default:
                break;
        }
    }

    playAdventurerCard(currPlayer, state);

    // Test if discard count is too small
    if ((state->discardCount[currPlayer] - prevDiscardCount) < cardsToDiscardBeforeTreasureDrawn)
    {
        hasPassedTests = 0;
    }

    // Test if treasure increased on hand
    for (int k = 0; k < state->handCount[currPlayer]; k++)
    {
        int currHandType = state->hand[currPlayer][k];

        switch (currHandType)
        {
            case copper:
            case silver:
            case gold:
                currTreasuresOnHand++;
                break;
            default:
                break;
        }
    }

    if (deckTreasureCount < 2)
    {
        // If deck treasures were less than 2, only compare difference
        if ((currTreasuresOnHand - prevTreasuresOnHand) != deckTreasureCount)
        {
            hasPassedTests = 0;
        }
        else if (currTreasuresOnHand - prevTreasuresOnHand != 2) // 2 treasures max
        {
            hasPassedTests = 0;
        }
    }

    return hasPassedTests;
}