/*******************************************************************************
 * unittest2.c
 *
 * Include the following lines in your makefile:
 * unittest2: unittest2.c dominion.o rngs.o
 * gcc -o unittest2 -g unittest2.c dominion.o rngs.o $(CFLAGS)
 ******************************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>

void testGetCostExistingCards();
void testGetCostNonExistentCard();
void _checkCorrectCosts(char*, int, int);
void _printPass();
void _printFail(int, int);

int main() {
    printf("***Unit Test 2: getCost()***\n");
    testGetCostExistingCards();
    testGetCostNonExistentCard();
    printf("\n");

    return 0;
}

// Card cost referenced from http://wiki.dominionstrategy.com/index.php/List_of_cards
void testGetCostExistingCards() {
    printf("Should return the correct cost for cards:\n");

    _checkCorrectCosts("Curse", curse, 0);
    _checkCorrectCosts("Estate", estate, 2);
    _checkCorrectCosts("Duchy", duchy, 5);
    _checkCorrectCosts("Province", province, 8);
    _checkCorrectCosts("Copper", copper, 0);
    _checkCorrectCosts("Silver", silver, 3);
    _checkCorrectCosts("Gold", gold, 6);
    _checkCorrectCosts("Adventurer", adventurer, 6);
    _checkCorrectCosts("Council Room", council_room, 5);
    _checkCorrectCosts("Feast", feast, 4);
    _checkCorrectCosts("Gardens", gardens, 4);
    _checkCorrectCosts("Mine", mine, 5);
    _checkCorrectCosts("Remodel", remodel, 4);
    _checkCorrectCosts("Smithy", smithy, 4);
    _checkCorrectCosts("Village", village, 3);
    _checkCorrectCosts("Baron", baron, 4);
    _checkCorrectCosts("Great Hall", great_hall, 3);
    _checkCorrectCosts("Minion", minion, 5);
    _checkCorrectCosts("Steward", steward, 3);
    _checkCorrectCosts("Tribute", tribute, 5);
    _checkCorrectCosts("Ambassador", ambassador, 3);
    _checkCorrectCosts("Cutpurse", cutpurse, 4);
    _checkCorrectCosts("Embargo", embargo, 2);
    _checkCorrectCosts("Outpost", outpost, 5);
    _checkCorrectCosts("Salvager", salvager, 4);
    _checkCorrectCosts("Sea Hag", sea_hag, 4);
    _checkCorrectCosts("Treasure Map", treasure_map, 4);
}

void testGetCostNonExistentCard() {
    printf("Should return a -1 for nonexistent cards\n");

    // Negative integer in case more cards are added to the enum
    _checkCorrectCosts("Superman", -1, -1);
}

void _checkCorrectCosts(char* message, int cardNumber, int expectedCost) {
    printf("\t%s", message);

    int actualCost = getCost(cardNumber);

    if (actualCost == expectedCost) {
        _printPass();
    } else {
        _printFail(expectedCost, actualCost);
    }
}

void _printPass() {
    printf(" - PASS\n");
}

void _printFail(int expected, int actual) {
    printf(" - FAIL (Expected Cost: %d, Actual Cost: %d)\n", expected, actual);
}