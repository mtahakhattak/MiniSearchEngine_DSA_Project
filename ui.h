// ============================================================
//  MODULE 3 : SUGGESTIONS & INTERFACE    (Member 3)
//  Stack (linked list), circular queue (array),
//  binary search for suggestions, menu, result paging
// ============================================================
#ifndef UI_H
#define UI_H

#include <string>
#include "search.h"
using namespace std;

void showResults(ResultNode* head);      // 5 results per page, N = next, P = previous
void suggest(const string& prefix);      // list indexed words starting with prefix
void runMenu();                          // main console menu

#endif
