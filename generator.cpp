#include "base.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>

using namespace std;
string getrandomstring(const vector<string>& strings) {
    int randomindex = rand() % strings.size();
    return strings[randomindex];
};
void filetovector(const string& filename, vector<string>& words) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        words.push_back(line);
    }
    file.close();
};

int main() {
    library mags;

    srand(static_cast<unsigned int>(time(0)));
    int n = 0;

    vector<string> magazines;
    vector<string> articles;
    vector<string> names;
    vector<string> surnames;
    filetovector("magazines.txt", magazines);
    filetovector("articles.txt", articles);
    filetovector("names.txt", names);
    filetovector("surnames.txt", surnames);
    printf("Data base size: ");
    cin >> n;
    cout << "\n";
    for (int i = 0; i < n; i++) {
        string magazineName = getrandomstring(magazines);
        int year = rand() % 75 + 1950;
        string articleName = getrandomstring(articles);

        vector<string> authors;
        int num = rand() % 7 + 1;
        for (int i = 0; i < num; i++) {
            authors.push_back(getrandomstring(names) + " " + getrandomstring(surnames));

        }
        int index = rand() % 10000;

        mags.addmag(magazineName, year, articleName, index, authors);


    }
    //mags.sortbyindex();
    mags.print_base_file("example.txt");


    return 0;
}