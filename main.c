#include "header.h"

#include <stdio.h>

#include <string.h>

#include <stdlib.h>


// Initialize a doubly linked list
List * init_list() {
  List * lst = (List * ) malloc(sizeof(List)); //allocating memory
  lst -> head = NULL;
  lst -> tail = NULL;
  return lst;
}

// Return 1 if the lift is empty, 0 otherwise
int is_empty(List * list) {
  if (list -> head == NULL && list -> tail == NULL) return 1; //list exists, but no nodes exist
  return 0;
}

// Verify if a node contains certain information
int contine(List * list, char chr) {
  Node * nod = list -> head; //pointer to the first node
  while (nod) {
    if (nod -> letter == chr) return 1;
    nod = nod -> next;
  }
  return 0;
}

//we read another character from the input file in main, and
// dinamically extend the list, by inserting a note to the end
void citire_din_fisier(List * list, char chr, Node ** cursor, int rand) {
  Node * nod = (Node * ) malloc(sizeof(Node));
  nod -> letter = chr;
  nod -> line = rand;
  if (is_empty(list)) // if list is empty and this is the first node we insert
  {
    nod -> next = NULL;
    nod -> prev = NULL;
    list -> tail = nod;
    list -> head = nod;
  } else // list had other nodes
  {
    list -> tail -> next = nod;
    nod -> prev = list -> tail;
    list -> tail = nod;
    nod -> next = NULL;
  }
  * cursor = list -> tail; // cursor points after the inserted element
}

//print list in console for easier debugging
void print_list(List * list) {
  for (Node * it = list -> head; it != NULL; it = it -> next) {
    printf("(%c, %d) ->", it -> letter, it -> line);
  }
  printf("NULL\n");
}

//free memory for the list
void free_list(List * list) {
  for (Node * it = list -> head -> next; it != NULL; it = it -> next) {
    free(it -> prev);
  }
  free(list -> tail);
  free(list);
}

//------------------------------------------------------------------------------------------------------------------------------------

//Delete the character before the cursor
void backspace(Node ** cursor, List * list) {
  Node * nod = * cursor;
  //important special cases:
  //delete first character
  if (nod == list -> head -> next) {
    list -> head = nod;
    list -> head -> prev = NULL;
    free(nod -> prev); // !!!!!
    return;
  }
  if (nod -> next == NULL) // cursor at the end, ready to delete last element
  {
    nod -> prev -> next = NULL;
    list -> tail = nod -> prev;
    free(nod);
    return;
  }
  //delete any character in the middle
  * cursor = nod -> prev;
  nod -> prev -> next = nod -> next;
  nod -> next -> prev = nod -> prev;
  free(nod);
}

//Delete the character following the cursor
void delete_c(List * list, Node ** cursor) {
  Node * nod = ( * cursor) -> next;
  Node * p = ( * cursor) -> next -> next;
  p -> prev = * cursor;
  ( * cursor) -> next = p;
  free(nod);
}

//Delete specified row, or current line of the cursor
void delete_l(List * list, Node ** cursor) {
  int r = ( * cursor) -> line;
  if (r == 1) {
    Node * p = NULL;
    Node * nod = list -> head;
    while (nod -> line == r) {
      p = nod;
      nod = nod -> next; //we get to the first node that is not on that row
      free(p);
    }
    nod -> prev = NULL;
    list -> head = nod;
    *cursor=nod;
    for (nod = * cursor; nod != NULL; nod = nod -> next) {
    (nod -> line) --;
    }
    return;
  }
  Node * nod = * cursor;
  Node * p = NULL;
  //we'll delete current, so we mustn't lose the previous part of the connection
  Node * q = nod -> prev;
  while (nod -> line == r)
  {
    p = nod;
    nod = nod -> next;
    free(p);
  }
  nod -> prev = q;
  q -> next = nod;
  * cursor = nod;
  for (nod = * cursor; nod != NULL; nod = nod -> next) {
    (nod -> line) --;
  }
  return;
}

//Delete any 3 letter word
//Should be optimized for any number of letters, so not with letters individually
void delete_w(List *list, Node **cursor, char l1, char l2, char l3)
{
    for(Node *nod=*cursor; nod->next->next->next!=NULL; nod=nod->next )
    {
       if(nod->letter==l1 && nod->next->letter==l2 && nod->next->next->letter==l3)
       {
           Node *p=nod->prev;
           Node *q=nod->next->next->next;
           q->prev=p;
           p->next=q;
           free(nod->next->next);
           free(nod->next);
           free(nod);
           return;
       }
    }
}

//Replace any 3 letters word with another of length 3
void replace(List *list,Node **cursor, char l1, char l2, char l3, char l4, char l5, char l6)
{
    for(Node *nod=*cursor; nod->next->next->next!=NULL; nod=nod->next )
    {
       if(nod->letter==l1 && nod->next->letter==l2 && nod->next->next->letter==l3)
       {
           nod->letter=l4;
           nod->next->letter=l5;
           nod->next->next->letter=l6;
           return;
       }
    }
}

//Replace cursor at the position specified through parameters
void go_to_c(List * list, Node ** cursor, int al_catalea) {
  Node * nod = * cursor;
  int i;
  for (i = 1; i < al_catalea; i++) nod = nod -> next;
  * cursor = nod;
}

//Place cursor at the beginning of the line
void go_to_line(List * list, Node ** cursor, int rand) {
  Node * nod = list -> head;
  while (nod -> line != rand) nod = nod -> next;
  * cursor = nod;
}

//Write the edited version of the input text in the output file
void salveaza(List * list) {
  FILE * output;
  output = fopen("editor.out", "w");
  for (Node * it = list -> head; it != NULL; it = it -> next) {
    fputc(it -> letter, output);
  }
  fclose(output);
}

//------------------------------------------------------------------------------------------------------------------------------------
int main() {
  FILE * fisier;
  fisier = fopen("editor.in", "r");

  int ok = 1;

  char s[100] = "\0";
  int rand = 1; //first node(s) will be considered line 1
  int redo = 0;
  int undo = 0;

  Node * cursor = NULL;
  List * list = init_list();

  fgets(s, 11, fisier); //read first line from file

  int nu_le_pune = 0;

  while (s[0] != 'q') {
    nu_le_pune = 0;
    //printf("%s",s);
    if (strstr(s, "::i") != NULL && ok == 1) {
      ok = 0;
      nu_le_pune = 1;
    } //switch between reading and executing commands
    else if (strstr(s, "::i") != NULL && ok == 0) {
      ok = 1;
      nu_le_pune = 1;
    }
    if (ok == 1 && nu_le_pune == 0) { //insert mode
      int i;
      for (i = 0; i < strlen(s); i++) {
        /* put every character from the line, in the list */
        citire_din_fisier(list, s[i], & cursor, rand);
        if (s[i] == '\n') rand++;
      }
      //print_list(list);
    }
    if (ok == 0 && nu_le_pune == 0) { // execution mode
      /* check first letters to see what command we are dealing with */
      /* an eventually the parameters */
      if (s[0] == 'q') return 0;
      //print_list(list);
      if (s[0] == 'r')
        redo++;
      if (s[0] == 'u') undo++;
      if (s[0] == 'g' && s[1] == 'l') {
        int randul;
        randul = s[3] - '0'; // char -> int
        go_to_line(list, & cursor, randul);
      }
      if (s[0] == 'g' && s[1] == 'c') {
        int al_catalea;
        al_catalea = s[3] - '0';

        if (strlen(s) < 7) //to check if we have parameters and how many
          go_to_c(list, & cursor, al_catalea);
        else { //we have parameters
          int rrandul;
          rrandul = s[5] - '0';
          //first, we position ourselves at the beginning of the line
          go_to_line(list, & cursor, rrandul);
          // then we move to desired position
          go_to_c(list, & cursor, al_catalea);
        }
      }

      if(s[0]=='r' && s[1]=='e')
      {
          replace(list,&cursor,s[3],s[4],s[5],s[7],s[8],s[9]);
          //print_list(list);
      }

      if(s[0]=='d' && s[1]=='w')
      {
          delete_w(list,&cursor,s[3],s[4],s[5]);
          //print_list(list);

      }

      if (s[0] == 'd' && s[1] == 'l') {
            //print_list(list);
        if (strlen(s) < 4) delete_l(list, & cursor);
        else {
          int randul;
          randul = s[3] - '0';
          //first, we position ourselves at the beginning of the line
          go_to_line(list, & cursor, randul);
          delete_l(list, & cursor); //delete line
        }
      }

      if (s[0] == 'b') backspace( & cursor, list);
      if (s[0] == 'd') {
        if (strlen(s) < 3) delete_c(list, & cursor); //if there are no indices
        else {
          int repetari;
          repetari = s[2] - '0'; //parameter represents number of repetitions
          int i;
          for (i = 1; i <= repetari; i++)
            delete_c(list, & cursor);
        }
      }
      if (strcmp(s, "s\n") == 0 && !(undo == 1 && redo == 0)) salveaza(list);
    }
    fgets(s, 11, fisier);
  }
  free_list(list);
  fclose(fisier);

  return 0;
}
