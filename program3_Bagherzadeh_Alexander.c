//Name: Alexander
//Dr. Steinberg
//COP3502 Spring 2022
//Programming Assignment 3 Skeleton

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct card_s{
    int rank;    //number
    char * type; //type of card
    struct card_s * nextptr;
}card_t;

//function prototypes
void rules(); //display rules
int playRound(); //simulate round
card_t * openCardDeck(); //open the card deck and place into a linkedlist setup
card_t * insertBackSetup(card_t *node, char *name, int cardrank); //take card from orginial deck and place in back of linked list for setup of game
int empty(card_t * node); //check to see if linked list is empty
void cleanUp(card_t * head); //free memory to prevent memory leaks
int deckSize(card_t * head); //count number of nodes in the linked list
card_t * search(card_t * node, int spot); //search list for a specific spot in the card deck indexing is similar to array setup
card_t * copyCard(card_t * node); //make a deep copy of card
card_t * removeCard(card_t * node, int spot); //remove card from linkedlist
card_t * insertBackDeck(card_t *head, card_t *node); //place card at end of pile
int compareCard(card_t * cardp1, card_t * cardp2); //compare cards
card_t * moveCardBack(card_t *head); //place card at top of deck to the bottom of the deck

int main()
{
    int seed;
    printf("Enter Seed: ");
    scanf("%d", &seed);

    srand(seed); //seed set
    rules();

    int player; //1 or 2
    int result;

    printf("Would you like to be player 1 or 2?\n");
    printf("Enter 1 or 2: ");
    scanf("%d", &player); //choose player

    for(int game = 1; game <= 5; ++game) //simulate games
    {
        printf("Alright lets play game %d.\n", game);
        printf("Lets split the deck.\n");

        result = playRound(); //play game

        if((result == 1 && player == 1) || (result == 2 && player == 2)) //determine who won
            printf("You won game %d.\n", game);
        else
            printf("I won game %d.\n", game);
    }

    return 0;
}

void rules()
{
    printf("Welcome to the card game war!\n");
    printf("Here are the rules.\n");
    printf("You have a pile of cards and I have a pile of cards.\n");
    printf("We will each pull the top card off of our respective deck and compare them.\n");
    printf("The card with the highest number will win the round and take both cards.\n");
    printf("However if there is a tie, then we have to we have to place one card faced down and the next one faced up and compare the results.\n");
    printf("Winner of the tie, will grab all the cards out. However if it's a tie again, then we repeat the same action.\n");
    printf("Ready? Here we go!\n");
}

int playRound() {
    int winner;
    int result2;
    card_t * deck = openCardDeck(); //initial sorted deck
    card_t * p1Deck = NULL; //player 1's deck
    card_t * p2Deck = NULL; //player 2's deck
    int size;
    card_t * card;
    int spot;
    int amountOfCards;
    int i = 0;
    while(deck != NULL) { //Initial sorting of the cards using rand to split the deck evenly and randomly
        size = deckSize(deck); //get the size of the deck
        spot = rand() % size; //use rand to choose a spot to remove a card and move it to a player's deck
        card = search(deck, spot); //find the node
        card = copyCard(card); //make a copy of the node
        card->nextptr = NULL; //prepare it to be the end of a linked list
        p2Deck = insertBackDeck(p2Deck, card); //insert in the back of a player's deck
        deck = removeCard(deck, spot); //remove the node from the original sorted deck

        size = deckSize(deck); //Repeat the previous process for the other deck
        spot = rand() % size;
        card = search(deck, spot);
        card = copyCard(card);
        card->nextptr = NULL;
        p1Deck = insertBackDeck(p1Deck, card);
        deck = removeCard(deck, spot);
    }
    printf("There are 52 cards in the deck.\n");
    while(empty(p1Deck) == 0 && empty(p2Deck) == 0) { //Loops while both decks still have cards
        printf("Player 1 pulled out %s. \t Player 2 pulled out %s.\n",p1Deck->type, p2Deck->type);
        result2 = compareCard(p1Deck, p2Deck); //compares the cards on the top of each linked list
        if(result2 == 2) { //The case that player 2 won
            printf("Player 2 won that round.\n");
            card = copyCard(p1Deck); //add cards to player 2's deck in appropriate order
            card->nextptr = NULL;
            p2Deck = moveCardBack(p2Deck);
            p2Deck = insertBackDeck(p2Deck, card);
            p1Deck = removeCard(p1Deck, 0);
        }else if (result2 == 1) { //The case that player 1 won
            printf("Player 1 won that round.\n");
            card = copyCard(p2Deck); //add cards to player 1's deck in appropriate order
            card->nextptr = NULL;
            p1Deck = moveCardBack(p1Deck);
            p1Deck = insertBackDeck(p1Deck, card);
            p2Deck = removeCard(p2Deck, 0);
        } else { //The case that there is a WAR
            card = NULL;
            card_t  * temp = NULL;
            i = 0;
            while (result2 == 0) { //Continues to loop until there is no more ties
                if (i != 0) {
                    printf("Player 1 has %d cards.\nPlayer 2 has %d cards.\n", deckSize(p1Deck), deckSize(p2Deck));
                }
                printf("Ugh oh...We have a tie! W-A-R!\n");
                if (deckSize(p1Deck) > 2 && deckSize(p2Deck) > 2) { //The case that there are at least 3 cards left both the decks
                    amountOfCards = 2;
                } else if (deckSize(p1Deck) > 1 && deckSize(p2Deck) > 1) { //The case that there are at least 2 cards left in both the decks
                    amountOfCards = 1;
                } else { //The case that there is 1 card left in one of the decks
                    amountOfCards = 0;
                }
                for (int j = 0; j < amountOfCards; ++j) { //stores cards in a temporary linked list (separate loops to preserve order)
                    card = copyCard(p1Deck);
                    card->nextptr = NULL;
                    temp = insertBackDeck(temp, card);
                    p1Deck = removeCard(p1Deck, 0);
                }
                for (int j = 0; j < amountOfCards; ++j) { //stores more cards in a temporary linked list (separate loops to preserve order)
                    card = copyCard(p2Deck);
                    card->nextptr = NULL;
                    temp = insertBackDeck(temp, card);
                    p2Deck = removeCard(p2Deck, 0);
                }
                if (amountOfCards == 0) { //The case that there is 1 card left in a deck. Win is given to the player with more cards to prevent infinite ties
                    if (deckSize(p1Deck) > 1) {
                        result2 = 1;
                    } else {
                        result2 = 2;
                    }
                } else { //calculates the result of the next duel
                    result2 = compareCard(p1Deck, p2Deck);
                }
                printf("Player 1 pulled out %s. \t Player 2 pulled out %s.\n",p1Deck->type, p2Deck->type);
                i++;
            }
            if (result2 == 2) { //The case that the winner of the next duel is player 2
                if (deckSize(p1Deck) > 1) {
                    printf("Player 2 won that W-A-R!\n");
                }
                p2Deck = insertBackDeck(p2Deck, temp);
                card = copyCard(p1Deck);
                card->nextptr = NULL;
                p2Deck = insertBackDeck(p2Deck, card);
                p1Deck = removeCard(p1Deck, 0);
                p2Deck = moveCardBack(p2Deck);
            } else { //The case that the winner of the next duel is player 1
                if (deckSize(p2Deck) > 1) {
                    printf("Player 1 won that W-A-R!\n");
                }
                p1Deck = insertBackDeck(p1Deck, temp);
                p1Deck = moveCardBack(p1Deck);
                card = copyCard(p2Deck);
                card->nextptr = NULL;
                p1Deck = insertBackDeck(p1Deck, card);
                p2Deck = removeCard(p2Deck, 0);
            }
        }
        printf("Player 1 has %d cards.\nPlayer 2 has %d cards.\n", deckSize(p1Deck), deckSize(p2Deck));
    }
    if (p1Deck != NULL) { //Determine winner
        winner = 1;
    } else{
        winner = 2;
    }
    cleanUp(p1Deck); //free the memory of the linked list if it's not empty
    cleanUp(p2Deck); //free the memory of the linked list if it's not empty
    return winner;
}

card_t * openCardDeck()
{
    FILE *fptr = fopen("deck.txt", "r");

    char *name = (char *) malloc(sizeof(char) * 20);

    if (name == NULL)
    {
        printf("Error in malloc...\n");
        exit(1);
    }

    card_t * head = NULL;

    int cardrank = 13;
    int tracker = 1;
    while(fscanf(fptr, "%s", name) == 1)
    {
        strcat(name, " ");

        if(tracker % 5 == 1)
        {
            strcat(name, "of Clubs");
            head = insertBackSetup(head, name, cardrank);
        }
        else if(tracker % 5 == 2)
        {
            strcat(name, "of Diamonds");
            head = insertBackSetup(head, name, cardrank);
        }
        else if(tracker % 5 == 3)
        {
            strcat(name, "of Hearts");
            head = insertBackSetup(head, name, cardrank);
        }
        else if(tracker % 5 == 4)
        {
            strcat(name, "of Spades");
            head = insertBackSetup(head, name, cardrank);
            tracker = 0;
            --cardrank;
        }

        ++tracker;

    }

    free(name);
    fclose(fptr);

    return head;
}

card_t * insertBackSetup(card_t *node, char *name, int cardrank)
{

    if(empty(node)) //check to see if list is empty
    {
        node = (card_t *) malloc(sizeof(card_t));

        if(empty(node)) //check to see if malloc worked
        {
            printf("Did not allocate head successfully...");
            printf("Program Terminating...\n");
            exit(1);
        }
        else //otherwise populate data of new head node
        {
            node->type = (char *) malloc(sizeof(char) * 20);

            if(node->type == NULL)
            {
                printf("Error with malloc...\n");
                exit(1);
            }

            strcpy(node->type, name);
            node->rank = cardrank;
            node->nextptr = NULL; //must make new tail nextptr NULL!!!

            return node;  //terminate
        }

    }

    //here we know that the list has at least one node

    card_t *head = node; //keep pointer to head since we will need to return this address

    while(node->nextptr != NULL) //traverse to tail
        node = node->nextptr;

    node->nextptr = (card_t *) malloc(sizeof(card_t));

    if(node->nextptr == NULL) //see if new tail was allocated successfully
    {
        printf("Did not allocate node successfully...");
        return head; //terminate if tail didn't get created
    }

    //populate new node
    node->nextptr->type = (char *) malloc(sizeof(char) * 20);

    if(node->nextptr->type == NULL)
    {
        printf("Error with malloc...\n");
        exit(1);
    }

    strcpy(node->nextptr->type, name);
    node->nextptr->rank = cardrank;
    node->nextptr->nextptr = NULL; //very important

    return head; //return head node since we need our starting point of the linked list
}

int empty(card_t * node) //checks if a linked list is empty
{
    return (node == NULL); //return condition result
}

int deckSize(card_t * head) { //checks the size of the deck
    if(head == NULL) { //The case that the deck is empty
        return 0;
    }
    int temp = 1;
    while(head->nextptr != NULL){ //The case that the deck is not empty
        temp++;
        head = head->nextptr;
    }
    return temp; //return the counter
}

card_t * copyCard(card_t * node) { //Hard copies a card
    card_t * copy = (card_t *) malloc(sizeof(card_t)); //makes space in the heap for the copied card
    copy->type = (char *) malloc(sizeof(char) * 20); //makes space in the heap for the copied card's type
    copy->rank = node->rank;
    strcpy(copy->type,node->type); //used string copy to have a new string instead of a reference to the old one
    copy->nextptr = node->nextptr;
    return copy; //returns the address of the copied card
}

int compareCard(card_t * cardp1, card_t * cardp2) { //compares the rank in the current node of each player deck
    if(cardp1->rank > cardp2->rank) {
        return 1; //player 1's deck card is a higher rank
    } else if(cardp1->rank < cardp2->rank) {
        return 2; //player 2's deck card is a higher rand
    } else {
        return 0; //the cards are the same rank
    }
}

card_t * moveCardBack(card_t *head) { //moves the head of the linked list to the back
    if (deckSize(head) == 1) { //The case that the linked list is the size of 1
        return head;
    }
    card_t * temp = head->nextptr; //Stores the address of the new head
    card_t *temp2 = head; //Used to cycle to the end of the linked list
    while (temp2->nextptr != NULL) {
        temp2 = temp2->nextptr;
    }
    temp2->nextptr = head; //move the old head node to the end of the linked list
    head->nextptr = NULL; //make the next pointer of the head point to NULL because it's the new end of the linked list
    return temp; //return the new head address
}

card_t * search(card_t * node, int spot) { //searches a linked list by going to a specific spot and returning that node address
    for(int i = 0; i < spot;++i) { //cycles to the spot where the node is
        node = node->nextptr;
    }
    return node; //returns the specific node address
}

card_t * insertBackDeck(card_t *head, card_t *node){ //inserts a node to the back of the passed in linked list
    if(head == NULL) { //The case that the linked list is empty
        return node;
    }
    card_t * temp = head; //Used to point to the node at the end of the linked list
    while (temp->nextptr != NULL) {
        temp = temp->nextptr;
    }
    temp->nextptr = node; //Adds node to the linked list
    return head; //returns the head address of the linked list
}

card_t * removeCard(card_t * node, int spot) { //removes a node from the linked list passed in
    card_t * head = node; //used to keep track of the head
    card_t * temp = node; //used to keep track of the node before the spot to be deleted
    for(int i = 0; i < spot - 1;++i) {
        temp = temp->nextptr;
    }
    if (spot != 0) {
        node = temp->nextptr; //points to the node in the spot to be deleted
    }
    if(deckSize(head) < 2) { //The case that there is 1 node left in the list
        free(node->type);
        free(node);
        return NULL;
    } else if (node->nextptr == NULL) { //The case that the node is the last one in the list
        free(node->type);
        free(node);
        temp->nextptr = NULL;
    } else if (spot == 0) { //The case that the node is the first one in the list
        head = head->nextptr;
        free(node->type);
        free(node);
        return head;
    } else { //The case that the node is in between other nodes in the list
        temp->nextptr = node->nextptr;
        free(node->type);
        free(node);
    }
    return head; //returns the head address of the linked list
}

void cleanUp(card_t * head) { //frees up memory used in linked list
    if(head == NULL) {
        return; //does not run if list is already empty to avoid dereferencing a NULL address
    }
    card_t * temp = head->nextptr;
    while(temp != NULL) { //frees each node then goes to the next node using a temp pointer
        free(head->type);
        free(head);
        head = temp;
        temp = temp->nextptr;
    }
    free(head->type); //frees the final node
    free(head);
    head = NULL;
}