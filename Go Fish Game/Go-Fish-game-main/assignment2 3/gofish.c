#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "card.h"
#include "deck.h"
#include "gofish.h"


void printboard()
{
  printf("\nPlayer 1's Hand -");
  printhand(&user);
  printf("\nPlayer 1's Book - ");
  printbook(&user);
  printf("\nPlayer 2's Book - ");
  printbook(&computer);
}


void printhand(struct player* target) {
  struct hand* temp = target->card_list;
  while (temp != NULL) {
    printf(" %c%c", temp->top.rank[0], temp->top.suit);
    temp = temp->next;
  }
}

void printbook(struct player* target) {
  for (int i = 0; i < 7; i++)
  {
    if(target->book[i] != '0')
    {
      printf("%c ", target->book[i]);
    }
  }
}

int countbooks(struct player* target) {
  int count = 0;
  for (int i = 0; i < 7; i++)
  {
    if(target->book[i] != '0')
    {
      count++;
    }
  }
  return count;
}

void user_turn()
{
  printboard();
  char rank = user_play(&user);
  if(search(&computer, rank) == 1){
    printf("\n\t- Player 2 has a ");
    while(search(&computer, rank) == 1){
      transfer_cards(&computer, &user, rank);
    }
    
    check_add_book(&user);
    if(game_over(&user)|| game_over(&computer))
    {
      return;
    }
    printf("\n\t- Player 1 gets another turn");
    user_turn();

  }
  else
  {
    printf("\t- Player 2 has no %c's", rank);
    add_card(&user, next_card());
    // print the card drawn
    printf("\n\t- Gofish, Player 1 draws a %c%c\n", user.card_list->top.rank[0], user.card_list->top.suit);
    check_add_book(&user);
    if(game_over(&user)|| game_over(&computer))
    {
      return;
    }
    if (user.card_list->top.rank[0] == rank)
    {
      printf("\n\t -Player 1 gets another turn");
      user_turn();
    }
    else{
      computer_turn();
    }

  }

}

void computer_turn()
{
  printboard();
  char rank = computer_play(&computer);
  if(search(&user, rank) == 1){
    printf("\n\t- Player 1 has a ");
    while(search(&user, rank) == 1){
      transfer_cards(&user, &computer, rank);
    }
    check_add_book(&computer);
    if(game_over(&user)|| game_over(&computer))
    {
      return;
    }
    printf("\n\t- Player 2 gets another turn");
    computer_turn();
  }
  else
  {
    printf("\n\t- Player 1 has no %c's", rank);
    add_card(&computer, next_card());
    // print the card drawn
    printf("\n\t- Gofish, Player 2 draws a card\n");
    check_add_book(&computer);
    if(game_over(&user)|| game_over(&computer))
    {
      return;
    }
    if (computer.card_list->top.rank[0] == rank)
    {
      printf("\n\t- Player 2 gets another turn");
      computer_turn();
    }
    else{
      user_turn();
    }
  }
}

int main(int args, char* argv[])
{
  char option;
  do{
    printf("Shuffling deck...\n");
    shuffle();
    deal_player_cards(&user);
    deal_player_cards(&computer);
    printf("\n");
    while (1)
    {
    user_turn();
    // if cards in hand are 0, break
    
    if (game_over(&user) == 1)
    {
      printf("\nPlayer 1 wins!");
      break;
    }
    if (game_over(&computer) == 1)
    {
  
      printf("\nPlayer 2 wins!");
      break;
    }
    if (user.card_list == NULL)
    {
      printf("\nPlayer 1 has no cards left in hand");
      printf("\nPlayer 2 wins!");
      break;
    }
    if (computer.card_list == NULL)
    {
      printf("\nPlayer 2 has no cards left in hand");
      printf("\nPlayer 1 wins!");
      break;
    }
    break;
    }
    // ending part asking to play again
    printf("Do you want to play again? (y/n): ");
    scanf(" %c", &option);
    if(option == 'y' || option == 'Y')
    {
      reset_player(&user);
      reset_player(&computer);
      printf("\n\n");
      continue;

    }
    else if(option == 'n'){
      break;
    }

  }while(1);
  return 0;
}



