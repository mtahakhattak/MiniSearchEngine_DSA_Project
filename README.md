# Mini Search Engine (Data Structures Project)

## Files
| File | Module | Owner |
|---|---|---|
| index.h / index.cpp | Module 1 - Indexing Engine | Member 1 |
| search.h / search.cpp | Module 2 - Search & Ranking | Member 2 |
| ui.h / ui.cpp | Module 3 - Suggestions & Interface | Member 3 |
| main.cpp | Starts the program | all |
| docs/ | Sample .txt documents (add your own) | |

## Run in VS Code
1. File > Open Folder... > select `MiniSearchEngine`.
2. Press Ctrl+Shift+B (build task) or in the terminal:
   `g++ -std=c++17 -Wall -Wextra -g main.cpp index.cpp search.cpp ui.cpp -o search`
3. Run: `./search` (Windows: `.\search.exe`). To use another folder: `./search myfolder`

Needs g++ with C++17 (MinGW-w64 / GCC 8 or newer) because the folder is read with `<filesystem>`.

## Query examples
`stack queue` (AND by default) | `list OR array` | `recursion NOT stack`
Operators AND / OR / NOT must be in CAPITAL letters.
