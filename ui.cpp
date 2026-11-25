// ============================================================
//  MODULE 3 : SUGGESTIONS & INTERFACE    (Member 3)
// ============================================================
#include "ui.h"
#include "index.h"
#include <iostream>
using namespace std;

// ---------------- Stack (linked list) : search history ----------------
struct StackNode {
    string data;
    StackNode* next;
};

class SearchStack {
    StackNode* top;
    int count;
public:
    SearchStack() { top = NULL; count = 0; }
    ~SearchStack() { while (!isEmpty()) pop(); }
    bool isEmpty() { return top == NULL; }
    int  size()    { return count; }
    void push(const string& s) {
        StackNode* n = new StackNode;
        n->data = s;
        n->next = top;
        top = n;
        count++;
    }
    string pop() {
        StackNode* t = top;
        string s = t->data;
        top = top->next;
        delete t;
        count--;
        return s;
    }
    string peek() { return top->data; }
};

// ---------------- Circular queue (array) : last 5 searches ----------------
class RecentQueue {
    static const int CAP = 5;
    string items[CAP];
    int front, count;
public:
    RecentQueue() { front = 0; count = 0; }
    void dequeue() { front = (front + 1) % CAP; count--; }
    void enqueue(const string& s) {
        if (count == CAP) dequeue();             // queue full -> remove the oldest
        items[(front + count) % CAP] = s;
        count++;
    }
    void show() {
        if (count == 0) { cout << "No recent searches yet.\n"; return; }
        cout << "\n--- Recent searches (newest first) ---\n";
        for (int i = 0; i < count; i++) {
            int idx = (front + count - 1 - i) % CAP;
            cout << (i + 1) << ". " << items[idx] << endl;
        }
    }
};

static SearchStack history;
static RecentQueue recent;

// ---------------- input helper ----------------
static bool readLine(const string& prompt, string& out) {
    cout << prompt;
    if (!getline(cin, out)) return false;        // end of input
    return true;
}

// ---------------- showing results (doubly linked list) ----------------
void showResults(ResultNode* head) {
    if (head == NULL) { cout << "No documents found.\n"; return; }

    int total = 0;
    for (ResultNode* r = head; r != NULL; r = r->next) total++;
    const int PAGE = 5;
    int totalPages = (total + PAGE - 1) / PAGE;
    int page = 1;
    ResultNode* start = head;

    cout << "\n" << total << " document(s) found.\n";
    while (true) {
        cout << "\n--- Page " << page << " of " << totalPages << " ---\n";
        ResultNode* cur = start;
        for (int i = 0; i < PAGE && cur != NULL; i++) {
            cout << ((page - 1) * PAGE + i + 1) << ". " << getDocName(cur->docId)
                 << "   (matches: " << cur->score << ")\n";
            cur = cur->next;
        }
        if (totalPages == 1) break;

        string c;
        if (!readLine("[N]ext  [P]revious  [Q]uit results: ", c)) break;
        if (c == "N" || c == "n") {
            if (page < totalPages) {
                for (int i = 0; i < PAGE; i++) start = start->next;   // move forward
                page++;
            } else cout << "Already on the last page.\n";
        } else if (c == "P" || c == "p") {
            if (page > 1) {
                for (int i = 0; i < PAGE; i++) start = start->prev;   // move back (doubly linked!)
                page--;
            } else cout << "Already on the first page.\n";
        } else break;
    }
}

// ---------------- suggestions (recursive binary search) ----------------
// first index whose word is >= prefix
static int lowerBound(const string& p, int lo, int hi) {
    if (lo > hi) return lo;
    int mid = (lo + hi) / 2;
    if (getWordAt(mid) < p) return lowerBound(p, mid + 1, hi);
    return lowerBound(p, lo, mid - 1);
}

static bool startsWith(const string& w, const string& p) {
    return w.size() >= p.size() && w.compare(0, p.size(), p) == 0;
}

void suggest(const string& prefix) {
    string p = cleanWord(prefix);
    if (p == "") { cout << "Please type some letters.\n"; return; }

    int i = lowerBound(p, 0, getWordCount() - 1);
    int shown = 0;
    cout << "\nWords starting with \"" << p << "\":\n";
    while (i < getWordCount() && startsWith(getWordAt(i), p) && shown < 10) {
        cout << "  " << getWordAt(i) << endl;
        i++;
        shown++;
    }
    if (shown == 0) cout << "  (no words found)\n";
}

// ---------------- menu actions ----------------
static void runSearch(const string& q, bool record) {
    ResultNode* res = search(q);
    if (record) {
        history.push(q);
        recent.enqueue(q);
    }
    showResults(res);
    freeResults(res);
}

static void goBack() {
    if (history.size() < 2) { cout << "There is no previous search.\n"; return; }
    history.pop();                               // remove the current search
    string prev = history.peek();                // previous search
    cout << "Going back to: " << prev << endl;
    runSearch(prev, false);
}

static void compareSearches() {
    string w;
    if (!readLine("Enter a word: ", w)) return;
    cout << "\nWords in index         : " << getWordCount() << endl;
    cout << "Linear search compares : " << linearSearchWord(w) << " words\n";
    cout << "Binary search compares : " << binarySearchWord(w) << " words\n";
}

void runMenu() {
    string choice, text;
    while (true) {
        cout << "\n========== MINI SEARCH ENGINE ==========\n";
        cout << "1. Search\n";
        cout << "2. Word suggestions\n";
        cout << "3. Go back to previous search\n";
        cout << "4. Recent searches\n";
        cout << "5. Compare linear and binary search\n";
        cout << "6. Index statistics\n";
        cout << "0. Exit\n";
        if (!readLine("Choice: ", choice)) break;

        if (choice == "1") {
            if (!readLine("Search (use AND / OR / NOT): ", text)) break;
            if (text == "") cout << "Empty query.\n";
            else runSearch(text, true);
        } else if (choice == "2") {
            if (!readLine("Type the first letters: ", text)) break;
            suggest(text);
        } else if (choice == "3") goBack();
        else if (choice == "4") recent.show();
        else if (choice == "5") compareSearches();
        else if (choice == "6") printIndexStats();
        else if (choice == "0") { cout << "Goodbye!\n"; break; }
        else cout << "Invalid choice.\n";
    }
}
