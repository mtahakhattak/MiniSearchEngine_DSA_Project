// ============================================================
//  MODULE 1 : INDEXING ENGINE            (Member 1)
// ============================================================
#include "index.h"
#include <iostream>
#include <fstream>
#include <cctype>
#include <filesystem>
using namespace std;

// ---------------- data ----------------
static WordNode*  head = NULL;          // linked list of words (in order of first appearance)
static WordNode*  tail = NULL;
static int        wordCount = 0;
static WordNode** sortedWords = NULL;   // array of pointers, sorted alphabetically

static const int  MAX_DOCS = 200;
static string     docNames[MAX_DOCS];
static string     docPaths[MAX_DOCS];
static int        docCount = 0;

static const string STOP_WORDS[] = {
    "a", "an", "the", "is", "am", "are", "was", "were", "be", "been",
    "and", "or", "of", "to", "in", "on", "at", "by", "for", "with",
    "as", "it", "its", "this", "that", "these", "those", "from", "can", "will",
    "we", "you", "he", "she", "they", "i", "so", "if", "than", "then"
};
static const int STOP_COUNT = sizeof(STOP_WORDS) / sizeof(STOP_WORDS[0]);

// ---------------- text cleaning ----------------
string cleanWord(const string& raw) {
    string out = "";
    for (size_t i = 0; i < raw.size(); i++) {
        unsigned char c = raw[i];
        if (isalnum(c)) out += (char)tolower(c);
    }
    return out;
}

// LINEAR SEARCH in the stop-word array
bool isStopWord(const string& w) {
    for (int i = 0; i < STOP_COUNT; i++) {
        if (STOP_WORDS[i] == w) return true;
    }
    return false;
}

// ---------------- linked list of words ----------------
// LINEAR SEARCH in the linked list of words
static WordNode* linearFindInList(const string& w) {
    WordNode* cur = head;
    while (cur != NULL) {
        if (cur->word == w) return cur;
        cur = cur->next;
    }
    return NULL;
}

static void addOccurrence(const string& w, int docId) {
    WordNode* node = linearFindInList(w);
    if (node == NULL) {                       // new word -> add at end of the list
        node = new WordNode;
        node->word = w;
        node->totalCount = 0;
        node->postHead = node->postTail = NULL;
        node->next = NULL;
        if (head == NULL) head = tail = node;
        else { tail->next = node; tail = node; }
        wordCount++;
    }
    node->totalCount++;

    // posting list: same document -> increase count, otherwise add new posting
    if (node->postTail != NULL && node->postTail->docId == docId) {
        node->postTail->count++;
    } else {
        PostingNode* p = new PostingNode;
        p->docId = docId;
        p->count = 1;
        p->next = NULL;
        if (node->postHead == NULL) node->postHead = node->postTail = p;
        else { node->postTail->next = p; node->postTail = p; }
    }
}

// ---------------- merge sort (recursive) on the word array ----------------
static void mergeParts(WordNode** a, WordNode** tmp, int lo, int mid, int hi) {
    int i = lo, j = mid + 1, k = lo;
    while (i <= mid && j <= hi) {
        if (a[i]->word <= a[j]->word) tmp[k++] = a[i++];
        else                          tmp[k++] = a[j++];
    }
    while (i <= mid) tmp[k++] = a[i++];
    while (j <= hi)  tmp[k++] = a[j++];
    for (k = lo; k <= hi; k++) a[k] = tmp[k];
}

static void mergeSort(WordNode** a, WordNode** tmp, int lo, int hi) {
    if (lo >= hi) return;                     // base case
    int mid = (lo + hi) / 2;
    mergeSort(a, tmp, lo, mid);
    mergeSort(a, tmp, mid + 1, hi);
    mergeParts(a, tmp, lo, mid, hi);
}

// ---------------- binary search (recursive) ----------------
static int binarySearchIndex(const string& w, int lo, int hi, int& comps) {
    if (lo > hi) return -1;                   // base case: not found
    int mid = (lo + hi) / 2;
    comps++;
    if (sortedWords[mid]->word == w) return mid;
    if (w < sortedWords[mid]->word) return binarySearchIndex(w, lo, mid - 1, comps);
    return binarySearchIndex(w, mid + 1, hi, comps);
}

WordNode* findWord(const string& word) {
    string w = cleanWord(word);
    int comps = 0;
    int idx = binarySearchIndex(w, 0, wordCount - 1, comps);
    if (idx == -1) return NULL;
    return sortedWords[idx];
}

int binarySearchWord(const string& w) {
    int comps = 0;
    binarySearchIndex(cleanWord(w), 0, wordCount - 1, comps);
    return comps;
}

int linearSearchWord(const string& word) {
    string w = cleanWord(word);
    int comps = 0;
    for (int i = 0; i < wordCount; i++) {
        comps++;
        if (sortedWords[i]->word == w) break;
    }
    return comps;
}

// ---------------- simple getters ----------------
int    getWordCount()        { return wordCount; }
string getWordAt(int i)      { return sortedWords[i]->word; }
int    getTotalDocs()        { return docCount; }
string getDocName(int id)    { return (id >= 0 && id < docCount) ? docNames[id] : "?"; }

// ---------------- build the index ----------------
// insertion sort for the file names (so document numbers are always the same)
static void sortDocNames() {
    for (int i = 1; i < docCount; i++) {
        string n = docNames[i], p = docPaths[i];
        int j = i - 1;
        while (j >= 0 && docNames[j] > n) {
            docNames[j + 1] = docNames[j];
            docPaths[j + 1] = docPaths[j];
            j--;
        }
        docNames[j + 1] = n;
        docPaths[j + 1] = p;
    }
}

static void indexOneFile(int docId) {
    ifstream in(docPaths[docId]);
    string current = "";
    char ch;
    while (in.get(ch)) {
        if (isalnum((unsigned char)ch)) {
            current += (char)tolower((unsigned char)ch);
        } else {                               // end of a word
            if (current.size() >= 2 && !isStopWord(current))
                addOccurrence(current, docId);
            current = "";
        }
    }
    if (current.size() >= 2 && !isStopWord(current))
        addOccurrence(current, docId);
}

void buildIndex(const string& folder) {
    namespace fs = std::filesystem;
    if (!fs::exists(folder) || !fs::is_directory(folder)) {
        cout << "Folder not found: " << folder << endl;
        return;
    }
    // 1) collect the .txt files
    for (const auto& entry : fs::directory_iterator(folder)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt" && docCount < MAX_DOCS) {
            docNames[docCount] = entry.path().filename().string();
            docPaths[docCount] = entry.path().string();
            docCount++;
        }
    }
    sortDocNames();

    // 2) read every file and fill the linked lists
    for (int d = 0; d < docCount; d++) indexOneFile(d);

    // 3) copy the words into an array and sort it alphabetically (merge sort)
    sortedWords = new WordNode*[wordCount > 0 ? wordCount : 1];
    WordNode** tmp = new WordNode*[wordCount > 0 ? wordCount : 1];
    int i = 0;
    for (WordNode* cur = head; cur != NULL; cur = cur->next) sortedWords[i++] = cur;
    mergeSort(sortedWords, tmp, 0, wordCount - 1);
    delete[] tmp;
}

void printIndexStats() {
    long total = 0;
    WordNode* best = NULL;
    for (WordNode* cur = head; cur != NULL; cur = cur->next) {
        total += cur->totalCount;
        if (best == NULL || cur->totalCount > best->totalCount) best = cur;
    }
    cout << "\n--- Index statistics ---\n";
    cout << "Documents indexed     : " << docCount << endl;
    cout << "Different words       : " << wordCount << endl;
    cout << "Total words (no stop) : " << total << endl;
    if (best != NULL)
        cout << "Most frequent word    : \"" << best->word << "\" (" << best->totalCount << " times)\n";
    for (int d = 0; d < docCount; d++)
        cout << "  Doc " << d << " = " << docNames[d] << endl;
}
