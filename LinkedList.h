/*
Integrantes: Raphael Iniesta Reis
RA: 10396285
*/

#ifndef __LinkedList_H__
#define __LinkedList_H__

#include "Tokens.h"
#include <string.h>

typedef struct Node {
    TokenTypes type;
    int lineCount;
    char *value;
    char *attributeName;

    struct Node *next;
    struct ErrorNode *error;
}Node;

typedef struct ErrorNode {
    TokenTypes expected;
    TokenTypes found;
}ErrorNode;

typedef struct LinkedList {
    Node *head;
}LinkedList;

LinkedList *new_list(void) {
    LinkedList *l = (LinkedList *)malloc(sizeof(LinkedList));
    Node *n = (Node *)malloc(sizeof(Node));

    n->type = null_token;
    n->lineCount = 0;
    n->value = 0;
    n->attributeName = "";
    n->next = NULL;

    l->head = n;

    return l;
}

Node *new_node(TokenTypes type, int lineCount, char *value, char *attributeName) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    
    newNode->attributeName = malloc(strlen(attributeName) + 1);
    strcpy(newNode->attributeName, attributeName);
    newNode->lineCount = lineCount;
    newNode->value = malloc(strlen(value) + 1);
    strcpy(newNode->value, value);
    newNode->type = type;
    newNode->next = NULL;

    return newNode;
}

ErrorNode *new_error_node(TokenTypes expected, TokenTypes found) {
    ErrorNode *newNode = (ErrorNode *)malloc(sizeof(ErrorNode));

    newNode->expected = expected;
    newNode->found = found;

    return newNode;
}

void insert_node(LinkedList *list, Node *node) {
    Node *n = list->head;

    while(n->next != NULL) {
        n = n->next;
    }

    n->next = node;
    // printf("Node inserted!\n");
}

void print_list(LinkedList *list) {
    Node *n = list->head;
    n = n->next; // Ignora o nó head
    while(n != NULL) {
        printf("%d:", n->lineCount);
        print_token(n->type);
        if(n->type == id_token) printf(" | %s",n->attributeName);
        if(n->value != NULL) printf("%s", n->value);
        printf("\n");
        n = n->next;
    }
    printf("\nAll nodes printed! No errors found!\n");
}

void free_list(LinkedList *list) {
    Node *current = list->head;
    Node *next = current->next;

    while(current != NULL) {
        free(current);
        current = next;
        next = next->next;
    }

    free(list);
    printf("List was freed!\nEnding program.\n");
}

#endif