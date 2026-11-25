# MiniSearchEngine_DSA_Project
# Mini Search Engine

## Project Overview

The **Mini Search Engine** is a console-based search application developed in C++ as a Data Structures and Algorithms (DSA) project. Its main purpose is to demonstrate how a basic search engine works by efficiently indexing, searching, and retrieving information from multiple text documents.

The project uses an **Inverted Indexing** technique to connect words with the documents in which they appear. Instead of scanning every document whenever a user performs a search, the application searches through the pre-built index to find relevant documents more efficiently.

It also implements different searching, sorting, ranking, and data management techniques to demonstrate the practical applications of fundamental data structures and algorithms.

## Key Features

- **Document Indexing:** Reads `.txt` files from a specified folder and builds an inverted index of their contents.
- **Text Processing:** Converts words to lowercase and filters out common stop words and short tokens.
- **Boolean Search:** Supports `AND`, `OR`, and `NOT` operators to perform different types of searches.
- **Default Search Operation:** Uses `AND` when no Boolean operator is specified.
- **Search Ranking:** Ranks matching documents according to word occurrence frequency, displaying higher-scoring results first.
- **Word Suggestions:** Suggests words from the index based on the prefix entered by the user.
- **Search History:** Allows users to return to their previous search.
- **Recent Searches:** Maintains and displays the five most recent searches.
- **Search Algorithm Comparison:** Compares the number of word comparisons performed by linear and binary search.
- **Index Statistics:** Displays information such as the total number of indexed documents and unique words.
- **Paginated Results:** Displays search results in pages, with navigation between previous and next pages.

## How It Works

The application follows a simple search engine workflow:

1. **Document Collection:** The program reads `.txt` files from the selected folder.
2. **Index Creation:** It extracts and processes words from the documents, then creates an inverted index containing unique words and their corresponding document IDs and occurrence counts.
3. **Word Organization:** The indexed words are stored in a sorted array of pointers to support efficient binary searching.
4. **Query Processing:** When a user enters a search query, the search module processes its words and Boolean operators.
5. **Document Retrieval:** The search engine retrieves document posting lists from the inverted index and combines them according to the selected Boolean operation.
6. **Ranking:** Matching documents are scored based on word occurrences and arranged in descending order of their scores.
7. **Result Display:** The interface displays the ranked documents and provides additional features such as suggestions, search history, and pagination.

## Data Structures and Algorithms Used

This project demonstrates the practical implementation of several DSA concepts:

| Data Structure / Algorithm | Application |
|---|---|
| Singly Linked List | Stores indexed words and document posting lists |
| Doubly Linked List | Stores and displays ranked search results |
| Stack | Maintains search history and supports previous searches |
| Circular Queue | Stores the five most recent searches |
| Queue | Processes search query terms |
| Array of Pointers | Maintains sorted word references for searching |
| Insertion Sort | Sorts document filenames |
| Merge Sort | Sorts indexed words |
| Binary Search | Finds words efficiently in the sorted index |
| Linear Search | Provides a comparison with binary search |
| Recursion | Used in searching and merging operations |
| Boolean Operations | Implements AND, OR, and NOT document retrieval |

## Project Modules and Team Contributions

The project is divided into three main modules, with each team member responsible for a specific part of the system.

### Module 1: Indexing Engine
**Files:** `index.cpp` and `index.h`

Responsible for collecting text documents, processing their contents, and creating the inverted index. It manages the word linked list, posting lists, document information, and sorted word array. It also provides functions for linear and binary searching.

### Module 2: Search and Ranking
**Files:** `search.cpp` and `search.h`

Responsible for processing user queries, handling Boolean operators (`AND`, `OR`, `NOT`), retrieving matching documents, calculating relevance scores, and arranging search results according to their scores.

### Module 3: Suggestions and User Interface
**Files:** `ui.cpp` and `ui.h`

Responsible for the interactive console menu and user experience. It handles word suggestions, previous searches, recent search history, search statistics, and paginated result display.

### Main Program
**File:** `main.cpp`

Connects the project modules, initializes the indexing engine, and launches the user interface.

## Technologies Used

- **Programming Language:** C++
- **Standard:** C++17
- **Compiler:** GCC / MinGW-w64
- **Library:** C++ Standard Library, including `<filesystem>`

## Project Structure

```text
MiniSearchEngine/
│
├── main.cpp
│
├── index.cpp
├── index.h
│
├── search.cpp
├── search.h
│
├── ui.cpp
├── ui.h
│
├── docs/
│   ├── linked_list.txt
│   ├── queue.txt
│   ├── recursion.txt
│   ├── searching.txt
│   ├── sorting.txt
│   └── stack.txt
│
└── README.md
```

## Sample Search Queries

```text
stack queue
```
Searches for documents containing both words (`AND` by default).

```text
list OR array
```
Searches for documents containing either word.

```text
recursion NOT stack
```
Searches for documents containing `recursion` but not `stack`.

**Note:** Boolean operators must be written in uppercase (`AND`, `OR`, `NOT`).

## How to Compile and Run

Make sure you have a C++17-compatible compiler installed.

**Compile:**

```bash
g++ -std=c++17 -Wall -Wextra -g main.cpp index.cpp search.cpp ui.cpp -o search
```

**Run using the default `docs` folder:**

```bash
./search
```

**Run using a custom folder:**

```bash
./search myfolder
```

On Windows, run the generated executable:

```bash
.\search.exe
```

## Project Objective

The primary objective of this project is to understand how fundamental data structures and algorithms can be combined to develop a practical search application. By implementing an inverted index, Boolean query processing, document ranking, sorting, searching, and search history management, the project demonstrates how core DSA concepts contribute to building an efficient information retrieval system.
