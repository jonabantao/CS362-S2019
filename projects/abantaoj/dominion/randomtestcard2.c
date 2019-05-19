#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "rngs.h"

int testGreatHallCard(struct gameState*, int, int);

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

    printf("Random Test: Great Hall Card\n");

    for (int i = 0; i < TEST_COUNT; i++)
    {
        memcpy(&testState, &initState, sizeof(struct gameState));
        int SEED = (rand() % 1000) + 1;
        int PLAYERS = (rand() % (MAX_PLAYERS - 2)) + 2;

        initializeGame(PLAYERS, KINGDOM_CARDS, SEED, &testState);

        int player = testState.whoseTurn;
        int handPos = floor(Random() * MAX_HAND);
        testState.deckCount[player] = floor(Random() * MAX_DECK);
        testState.discardCount[player] = floor(Random() * MAX_DECK);
        testState.handCount[player] = floor(Random() * MAX_HAND);
        testState.numActions = floor(Random() * 10);

        int result = testGreatHallCard(&testState, player, handPos);

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

int testGreatHallCard(struct gameState* state, int currPlayer, int handPos)
{
    int hasPassedTests = 1;
    int prevDeckCount = state->deckCount[currPlayer];
    int prevActions = state->numActions;

    playGreatHallCard(currPlayer, state, handPos);


    if (prevDeckCount != (state->deckCount[currPlayer] + 1))
    {
        hasPassedTests = 0;
    }

    if (prevActions != (state->numActions - 1))
    {
        hasPassedTests = 0;
    }

    return hasPassedTests;
}