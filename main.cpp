// Mini Search Engine - Data Structures Project
// Run:  ./search            (reads the folder "docs")
//       ./search myfolder   (reads another folder)
#include <iostream>
#include "index.h"
#include "ui.h"
using namespace std;

int main(int argc, char* argv[]) {
    string folder = (argc > 1) ? argv[1] : "docs";

    cout << "Building index from folder \"" << folder << "\" ...\n";
    buildIndex(folder);

    if (getTotalDocs() == 0) {
        cout << "No .txt documents found. Put some .txt files in the folder and run again.\n";
        return 1;
    }
    cout << "Done: " << getTotalDocs() << " documents, " << getWordCount() << " different words.\n";

    runMenu();
    return 0;
}
