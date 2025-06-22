# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "player.h"
# include "card.h"

// Player structure
/*
 * Function: add_card
 * -------------------
 *  Add a new card to the player's hand. 
 *
 *  target: the target player
 *  new_card: pointer to the new card to add
 *
 *  returns: return 0 if no error, non-zero otherwise
 */
int add_card(struct player* target, struct card* new_card)
{

    struct hand* new_hand = malloc(sizeof(struct hand));
    new_hand->top = *new_card;
    if(new_hand == NULL){
        return -1;
    }
    new_hand->top = *new_card;
    new_hand->next = target->card_list;
    target->card_list = new_hand;
    target->hand_size++;
    return 0;
}

/*
 * Function: remove_card
 * -------------------
 *  Remove a card from the player's hand. 
 *
 *  target: the target player
 *  card_index: the index of the card to remove
 *
 *  returns: return 0 if no error, non-zero otherwise
 */
int remove_card(struct player* target, struct card* old_card)
{
    if (target->hand_size == 0)
    {
        return -1;
    }
    struct hand* temp = target->card_list;
    struct hand* prev = NULL;
    // check old card with cards in hand list
    while(!(temp->top.rank[0] == old_card->rank[0] && temp->top.suit == old_card->suit))
    {
        prev = temp;
        temp = temp->next;
    }
    // if card is not found
    if(temp == NULL)
    {
        return -1;
    }
    // if card is found
    if(prev == NULL)
    {
        target->card_list = temp->next;
    }
    else
    {
        prev->next = temp->next;
    }
    free(temp);
    target->hand_size--;
    return 0;

}

/*
 * Function: check_add_book
 * ------------------------
 *  Check if a player has all 4 cards of the same rank.
 *  If so, remove those cards from the hand, and add the rank to the book.
 *  Returns after finding one matching set of 4, so should be called after adding each a new card.
 * 
 *  target: pointer to the player to check
 *  
 *  Return: a char that indicates the book that was added; return 0 if no book added.
 */


char check_add_book(struct player* target)
{
    // check for 4 of the same rank and add to book and remove from hand
    // do not use book_size

    struct hand* current = target->card_list;
    struct hand* remove = target->card_list;
    char rank;
    int count = 0;
    if(current == NULL)
    {
        return 0;
    }

    // check if there are 4 cards of the same rank and remove them if there are
    while(current != NULL)
    {
        rank = current->top.rank[0];
        while(remove != NULL)
        {
            if(remove->top.rank[0] == rank)
            {
                count++;
            }
            remove = remove->next;
        }
        if(count == 4)
        {
            break;
        }
        current = current->next;
    }

    // if there are 4 cards of the same rank, remove them
    if(count == 4)
    {
        remove = target->card_list;
        while(remove != NULL)
        {
            if(remove->top.rank[0] == rank)
            {
                remove_card(target, &remove->top);
            }
            remove = remove->next;
        }
        int i = 0;
        while(target->book[i]!=0){
            i++;
        }
        target->book[i] = rank;
        if (target == &user)
        {
            printf("\n\t- Player 1 books - ");
        }
        if (target == &computer)
        {
            printf("\n\t- Player 2 books - ");
        }
        for(int i = 0; i < 7; i++){
            printf("%c ", target->book[i]);
        }
        return rank;
    }
    
    return 0;


}

/*
 * Function: search
 * ----------------
 *  Search a player's hand for a requested rank.
 *  
 *  rank: the rank to search for
 *  target: the player (and their hand) to search
 *
 *  Return: If the player has a card of that rank, return 1, else return 0
 */
int search(struct player* target, char rank)
{
    struct hand* current = target->card_list;
    if (current == NULL) {
        return 0;
    }
    while (current != NULL) {
        if (*current->top.rank == rank) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

/*
 * Function: transfer_cards
 * ------------------------
 *   Transfer cards of a given rank from the source player's 
 *   hand to the destination player's hand. Remove transferred
 *   cards from the source player's hand. Add transferred cards
 *   to the destination player's hand.
 *   
 *   src: a pointer to the source player
 *   dest: a pointer to the destination player
 *   rank: the rank to transfer
 *
 *   Return: 0 if no cards found/transferred, <0 if error, otherwise 
 *   return value indicates number of cards transferred
 */   
int transfer_cards(struct player* src, struct player* dest, char rank)
{
    // transfer cards of a given rank from the source player's hand to the destination player's hand
    // remove transferred cards from the source player's hand
    // add transferred cards to the destination player's hand
    // return the number of cards transferred

    struct hand* current = src->card_list;

    int count = 0;
    while(current != NULL)
    {
        if(*current->top.rank == rank)
        {
            add_card(dest, &current->top);
            printf("%c%c ", current->top.rank[0], current->top.suit);
            remove_card(src, &current->top);
            count++;
        }
        current = current->next;
    }
    return count;


}

/*
 * Function: game_over
 * -------------------
 *   Boolean function to check if a player has 7 books yet
 *   and the game is over
 *
 *   target: the player to check
 *   
 *   Return: 1 if game is over, 0 if game is not over
 */
int game_over(struct player* target) 
{
    // check if the player has 7 books
    // return 1 if true, 0 if false
    int count = 0;
    for(int i = 0; i < 7; i++)
    {
        if(target->book[i] != 0)
        {
            count++;
        }
    }
    if(count == 7)
    {
        return 1;
    }
    return 0;    
}

/* 
 * Function: reset_player
 * ----------------------
 *
 *   Reset player by free'ing any memory of cards remaining in hand,
 *   and re-initializes the book.  Used when playing a new game.
 * 
 *   target: player to reset
 * 
 *   Return: 0 if no error, and non-zero on error.
 */
int reset_player(struct player* target)
{
    // remove all cards from hand using remove_card
    struct hand* next = NULL;
	struct hand* iterator = target -> card_list;
    while (iterator != NULL) {
        next = iterator -> next;
        free(iterator);
        iterator = next;
    }
    target -> card_list = NULL;
    target -> hand_size = 0;
    // reset book
    for (int i = 0; i < 7; i++) {
        target -> book[i] = 0;
    }
    return 0;
}

/* 
 * Function: computer_play
 * -----------------------
 *
 *   Select a rank randomly to play this turn. The player must have at least
 *   one card of the selected rank in their hand.
 *
 *   target: the player's hand to select from
 *
 *   Rank: return a valid selected rank
 */

char computer_play(struct player* target)
{
   
    struct hand* current = target->card_list;
    printf("\nPlayer 2's turn, enter a Rank: ");
    int count = 0;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    if (count == 0) {
        return '0';
    }
    int random = rand() % count;
    current = target->card_list;
    for (int i = 0; i < random; i++) {
        current = current->next;
    }
    printf("%c", current->top.rank[0]);

    return current->top.rank[0];
}

/* 
 * Function: user_play
 * -------------------
 *
 *   Read standard input to get rank user wishes to play.  Must perform error
 *   checking to make sure at least one card in the player's hand is of the 
 *   requested rank.  If not, print out "Error - must have at least one card from rank to play"
 *   and then re-prompt the user.
 *
 *   target: the player's hand to check
 * 
 *   returns: return a valid selected rank
 */
char user_play(struct player* target)
{
    char rank;
    printf("\nPlayer 1's turn, enter a Rank: ");
    scanf(" %c", &rank);
    while (search(target, rank) != 1) {
        printf("Error - must have at least one card from rank to play\n");
        printf("\nPlayer 1's turn, enter a Rank: ");
        scanf(" %c", &rank);
    }
    return rank;
}

