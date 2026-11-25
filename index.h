// ============================================================
//  MODULE 1 : INDEXING ENGINE            (Member 1)
//  Singly linked list of words + posting list inside each word,
//  linear search, merge sort (recursive), binary search (recursive)
// ============================================================
#ifndef INDEX_H
#define INDEX_H

#include <string>
using namespace std;

// One posting = one document that contains the word
struct PostingNode {
    int docId;          // document number
    int count;          // how many times the word appears in that document
    PostingNode* next;
};

// One node of the "linked list of words"
struct WordNode {
    string word;
    int totalCount;         // total appearances in all documents
    PostingNode* postHead;  // posting list (sorted by docId automatically)
    PostingNode* postTail;
    WordNode* next;
};

// ---------- functions used by the other modules ----------
void     buildIndex(const string& folder);   // read all .txt files and build index
WordNode* findWord(const string& word);      // binary search in sorted array (NULL if not found)
int      getWordCount();                     // number of different words
string   getWordAt(int i);                   // i-th word of the sorted array
string   getDocName(int id);                 // file name of a document
int      getTotalDocs();                     // number of documents
int      linearSearchWord(const string& w);  // returns number of comparisons used
int      binarySearchWord(const string& w);  // returns number of comparisons used
string   cleanWord(const string& raw);      // lower-case, letters/digits only
bool     isStopWord(const string& w);        // linear search in stop-word array
void     printIndexStats();                  // statistics for the menu

#endif
