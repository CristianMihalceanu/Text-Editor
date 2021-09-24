
//doubly linked list node
typedef struct Node {
  struct Node * next;
  struct Node * prev;
  char letter;
  int line;
}
Node;

//doubly linked list
typedef struct List {
  Node * head;
  Node * tail;
}
List;

// Initialize doubly linked list
List * init_list();

// Return 1 if list is empty, 0 otherwise
int is_empty(List * list);

// Verify if a node contains certain information
int contine(List * list, char chr);

//we read another character from the input file in main, and
// dinamically extend the list, by inserting a note to the end
void citire_din_fisier(List * list, char chr, Node ** cursor, int rand);

//print list in console for easier debugging
void print_list(List * list);

//free memory for the list
void free_list(List * list);

//------------------------------------------------------------------------------------------------------------------------------------

//Delete the character before the cursor
void backspace(Node ** cursor, List * list);

//Delete the character following the cursor
void delete_c(List * list, Node ** cursor);

//Delete specified row, or current line of the cursor
void delete_l(List * list, Node ** cursor);

//Delete any 3 letter word specified
void delete_w(List *list, Node **cursor, char l1, char l2, char l3);

//Replace any 3 letters word with another of length 3
void replace(List *list,Node **cursor, char l1, char l2, char l3, char l4, char l5, char l6);

//Replace cursor at the position specified through parameters
void go_to_c(List * list, Node ** cursor, int al_catalea);

//Place cursor at the beginning of the line
void go_to_line(List * list, Node ** cursor, int rand);

//Write the edited version of the input text in the output file
void salveaza(List * list);
