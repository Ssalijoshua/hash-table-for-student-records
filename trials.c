#include <stdio.h>
#include <stdlib.h>

typedef struct Age{
    int age;
    struct Age *next;
}Age;

// Creating the node 
Age *create_node(int n){
    Age *current = malloc(sizeof(Age));
    current -> age = n;

    current -> next = NULL;

    return current;

};

//  Inserts to the linked list
void insertInList(Age *node, Age **head){
    
    
    // We are inserting start
    node -> next = *head ;
    
    *head = node;    
};




int main(void){



    // Giving them data to store
    Age *newnode1  = create_node(45);
    Age *newnode2  = create_node(30);
    Age *newnode3  = create_node(12);
    Age *newnode4  = create_node(60);
    Age *newnode5  = create_node(56);

    // Inserting each node store the next address

    Age *head;

    insertInList( newnode1, &head);
    insertInList( newnode2, &head);
    insertInList( newnode3, &head);
    insertInList( newnode4, &head);
    insertInList( newnode5, &head);

    Age *current;

    current = head;

    while(current  != NULL){
        printf("%d  --->  ", current -> age);

        current = current -> next;
    }

    printf("NULL");





    return 0;
}