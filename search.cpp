// ============================================================
//  MODULE 2 : SEARCH & RANKING           (Member 2)
// ============================================================
#include "search.h"
#include "index.h"
#include <iostream>
using namespace std;

// ---------------- Queue (linked list) for the query words ----------------
struct QNode {
    string data;
    QNode* next;
};

class QueryQueue {
    QNode* front;
    QNode* rear;
public:
    QueryQueue() { front = rear = NULL; }
    ~QueryQueue() { while (!isEmpty()) dequeue(); }
    bool isEmpty() { return front == NULL; }
    void enqueue(const string& s) {
        QNode* n = new QNode;
        n->data = s;
        n->next = NULL;
        if (rear == NULL) front = rear = n;
        else { rear->next = n; rear = n; }
    }
    string dequeue() {
        QNode* t = front;
        string s = t->data;
        front = front->next;
        if (front == NULL) rear = NULL;
        delete t;
        return s;
    }
};

// ---------------- temporary sorted list (sorted by docId) ----------------
struct ScoreNode {
    int docId;
    int score;
    ScoreNode* next;
};

static ScoreNode* newScore(int d, int s, ScoreNode* nx) {
    ScoreNode* n = new ScoreNode;
    n->docId = d;
    n->score = s;
    n->next = nx;
    return n;
}

static void freeScores(ScoreNode* h) {
    while (h != NULL) { ScoreNode* t = h; h = h->next; delete t; }
}

// posting list of a word  ->  ScoreNode list
static ScoreNode* fromPostings(WordNode* w) {
    ScoreNode* h = NULL;
    ScoreNode* t = NULL;
    if (w == NULL) return NULL;
    for (PostingNode* p = w->postHead; p != NULL; p = p->next) {
        ScoreNode* n = newScore(p->docId, p->count, NULL);
        if (h == NULL) h = t = n;
        else { t->next = n; t = n; }
    }
    return h;
}

static ScoreNode* allDocs() {                    // every document with score 0
    ScoreNode* h = NULL;
    ScoreNode* t = NULL;
    for (int d = 0; d < getTotalDocs(); d++) {
        ScoreNode* n = newScore(d, 0, NULL);
        if (h == NULL) h = t = n;
        else { t->next = n; t = n; }
    }
    return h;
}

// ---------------- RECURSIVE merges of two sorted lists ----------------
static ScoreNode* copyList(ScoreNode* a) {
    if (a == NULL) return NULL;
    return newScore(a->docId, a->score, copyList(a->next));
}

// AND : documents present in both lists (scores are added)
static ScoreNode* mergeAnd(ScoreNode* a, ScoreNode* b) {
    if (a == NULL || b == NULL) return NULL;
    if (a->docId == b->docId)
        return newScore(a->docId, a->score + b->score, mergeAnd(a->next, b->next));
    if (a->docId < b->docId) return mergeAnd(a->next, b);
    return mergeAnd(a, b->next);
}

// OR : documents present in at least one list
static ScoreNode* mergeOr(ScoreNode* a, ScoreNode* b) {
    if (a == NULL) return copyList(b);
    if (b == NULL) return copyList(a);
    if (a->docId == b->docId)
        return newScore(a->docId, a->score + b->score, mergeOr(a->next, b->next));
    if (a->docId < b->docId)
        return newScore(a->docId, a->score, mergeOr(a->next, b));
    return newScore(b->docId, b->score, mergeOr(a, b->next));
}

// NOT : documents of a that are NOT in b
static ScoreNode* mergeNot(ScoreNode* a, ScoreNode* b) {
    if (a == NULL) return NULL;
    if (b == NULL) return copyList(a);
    if (a->docId < b->docId)
        return newScore(a->docId, a->score, mergeNot(a->next, b));
    if (a->docId == b->docId) return mergeNot(a->next, b->next);
    return mergeNot(a, b->next);
}

static ScoreNode* combine(const string& op, ScoreNode* a, ScoreNode* b) {
    if (op == "OR")  return mergeOr(a, b);
    if (op == "NOT") return mergeNot(a, b);
    return mergeAnd(a, b);
}

// ---------------- INSERTION SORT into the doubly linked list ----------------
// keeps the list sorted by score (highest first)
static void insertSorted(ResultNode*& head, ResultNode* n) {
    if (head == NULL) { head = n; return; }
    if (n->score > head->score) {                // new first node
        n->next = head;
        head->prev = n;
        head = n;
        return;
    }
    ResultNode* cur = head;
    while (cur->next != NULL && cur->next->score >= n->score) cur = cur->next;
    n->next = cur->next;
    n->prev = cur;
    if (cur->next != NULL) cur->next->prev = n;
    cur->next = n;
}

// ---------------- main function of this module ----------------
ResultNode* search(const string& query) {
    // 1) split the query into words and put them in the queue
    QueryQueue q;
    string tok = "";
    for (size_t i = 0; i <= query.size(); i++) {
        if (i == query.size() || query[i] == ' ' || query[i] == '\t') {
            if (tok != "") q.enqueue(tok);
            tok = "";
        } else tok += query[i];
    }

    // 2) take the words one by one and combine their posting lists
    ScoreNode* result = NULL;
    bool first = true;
    string op = "AND";                           // default operator
    while (!q.isEmpty()) {
        string t = q.dequeue();
        if (t == "AND" || t == "OR" || t == "NOT") { op = t; continue; }

        string w = cleanWord(t);
        if (w == "" || isStopWord(w)) continue;  // ignore stop words

        ScoreNode* list = fromPostings(findWord(w));   // binary search inside findWord
        if (first) {
            if (op == "NOT") {                   // query starts with NOT
                ScoreNode* all = allDocs();
                result = mergeNot(all, list);
                freeScores(all);
                freeScores(list);
            } else result = list;
            first = false;
        } else {
            ScoreNode* merged = combine(op, result, list);
            freeScores(result);
            freeScores(list);
            result = merged;
        }
        op = "AND";
    }

    // 3) move the result into a doubly linked list and sort it by score
    ResultNode* head = NULL;
    for (ScoreNode* s = result; s != NULL; s = s->next) {
        ResultNode* n = new ResultNode;
        n->docId = s->docId;
        n->score = s->score;
        n->next = n->prev = NULL;
        insertSorted(head, n);
    }
    freeScores(result);
    return head;
}

void freeResults(ResultNode* head) {
    while (head != NULL) { ResultNode* t = head; head = head->next; delete t; }
}
