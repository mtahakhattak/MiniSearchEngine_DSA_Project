// ============================================================
//  MODULE 2 : SEARCH & RANKING           (Member 2)
//  Queue (linked list), recursive merge of sorted lists,
//  doubly linked list of results, insertion sort
// ============================================================
#ifndef SEARCH_H
#define SEARCH_H

#include <string>
using namespace std;

// One result = one document with its score (DOUBLY linked list)
struct ResultNode {
    int docId;
    int score;
    ResultNode* next;
    ResultNode* prev;
};

// Query examples:  stack queue      (AND is the default)
//                  list OR array
//                  list NOT array   (operators must be written in CAPITAL letters)
ResultNode* search(const string& query);   // returns head of the list, best score first
void        freeResults(ResultNode* head);

#endif
