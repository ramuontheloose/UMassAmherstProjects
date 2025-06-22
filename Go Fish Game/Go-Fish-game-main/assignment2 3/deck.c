# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "deck.h"
# include <time.h>
/*
 * Function: shuffle
 * --------------------
 *  Initializes deck_instance and shuffles it.
 *  Resets the deck if a prior game has been played.
 * 
 *  returns: 0 if no error, and non-zero on error
 */ 
int shuffle()
{ // initialize deck
    char suit[4] = {'S', 'H', 'D', 'C'};
    char rank[13] = {'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K'};

    //initialize and shuffle deck at the same time
    int i = 0;
    for(i = 0; i < 52; i++)
    {
        deck_instance.list[i].suit = suit[i/13];
        deck_instance.list[i].rank[0] = rank[i%13];
        deck_instance.list[i].rank[1] = '\0';
    }
    //shuffle deck
    srand(time(NULL));
    for(i = 0; i < 52; i++)
    {
        int j = rand() % 52;
        struct card temp = deck_instance.list[i];
        deck_instance.list[i] = deck_instance.list[j];
        deck_instance.list[j] = temp;
    }
    deck_instance.top_card = 52;
    
    return 0;
}

/*
 * Function: deal_player_cards
 * ---------------------------
 *  Deal 7 random cards to the player specified in the function.
 *  Remove the dealt cards from the deck. 
 *
 *  target: pointer to the player to be dealt cards
 *
 *  returns: 0 if no error, and non-zero on error
 */

// function deals cards to the player
int deal_player_cards(struct player* target)
{
    // deal 7 cards to the player from the deck
    for(int i = 0; i<=6; i++){
        if(deck_instance.top_card == 0){
            return -1;
        }
        add_card(target, next_card());

    }
    return 0;

}
/*
 * Function: next_card
 * -------------------
 *  Return a pointer to the top card on the deck.
 *  Removes that card from the deck. 
 *
 *  returns: pointer to the top card on the deck.
 */
struct card* next_card( )
{
        deck_instance.top_card--;
        return &deck_instance.list[deck_instance.top_card];
}

/*
 * Function: size
 * --------------
 *  Return the number of cards left in the current deck.
 *
 *  returns: number of cards left in the deck.
 */
size_t deck_size( )
{
        return deck_instance.top_card;
}
