// base.h  база данных 


#ifndef BASE_H
#define BASE_H
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <fstream>
#include <regex>
#include <unordered_set>
#include <iomanip>
using namespace std;

struct magazine {

    string name;
    int year;
    int number;
    string article_name;
    int SCI;
    vector<string> authors;

    
    magazine(const string& _name, int _year, int _number, const string& _article_name, int _SCI, const vector<string>& _authors)
        : name(_name), year(_year), number(_number), article_name(_article_name), SCI(_SCI), authors(_authors) {
        authors.clear();

        for (string auth : _authors) {
            authors.push_back(auth);

        }
    }
    string get_field_value(const string& fieldname) const {
        if (fieldname == "name") {
            return name;
        }
        else if (fieldname == "year") {
            return to_string(year);
        }
        else if (fieldname == "number") {
            return to_string(number);
        }
        else if (fieldname == "article_name") {
            return article_name;
        }
        else if (fieldname == "SCI") {
            return to_string(SCI);
        }
        else {
            
            return "";
        }
    }
};

class library {
private:
    vector<magazine> magazines;
    int number_next;

public:
    library() : number_next(1) {};

    void addmag(const string& name, int year, const string& article_name, int SCI, const vector<string>& authors) {
        magazine mag = magazine(name, year, number_next++, article_name, SCI, authors);
        //if (!checkmag(mag)) {
            magazines.push_back(mag);
        //}

    }
    /*bool checkmag(const magazine& mag) {
        for (const auto& existingMag : magazines) {
            if (existingMag.name == mag.name && existingMag.year == mag.year && existingMag.article_name == mag.article_name && existingMag.SCI == mag.SCI) {
                return true;
            }
        }
        return false;
    }*/
    void deletemag(int number) {
        for (auto it = magazines.begin(); it != magazines.end(); it++) {
            if (it->number == number) {
                magazines.erase(it);
            }
        }
    }
    void deletebyname(const string& name) {
        
        for (auto it = magazines.begin(); it != magazines.end(); it++) {
            
            if (it->name == name) {
                
                magazines.erase(it);
                
                //cout << "aboba" << endl;
            }
        }
        //print_base();
    }
    void deletebyarticle_name(const string& article_name) {
        for (auto it = magazines.begin(); it != magazines.end(); it++) {
            if (it->article_name == article_name) {
                magazines.erase(it);
            }
        }
    }
    void deletebyauthor(const string& authorName) {
        
        auto it = magazines.begin();
        while (it != magazines.end()) {
            if (find(it->authors.begin(), it->authors.end(), authorName) != it->authors.end()) {
                it = magazines.erase(it); // Удаляем элемент и получаем следующий итератор
            }
            else {
                ++it; // Переходим к следующему элементу
            }
        }
        //print_base();
    }

    void editmag(const string& name, int year, int number, const string& article_name, int SCI, const vector<string>& authors) {
        for (auto& magazine : magazines) {
            if (magazine.number == number) {
                magazine.name = name;
                magazine.year = year;
                magazine.article_name = article_name;
                magazine.SCI = SCI;


                for (string auth : authors) {

                    
                    magazine.authors.push_back(auth);
                }
            }
        }
    }


    
    void print_base_file(const string& filename) {
        std::ofstream fout; 
        fout.open(filename);
        for (auto& magazine : magazines) {
            fout << magazine.name << " - " << magazine.year << " - " << magazine.number << " - " << magazine.article_name << " - " << magazine.SCI << " citations" << " - ";
            for (auto it = magazine.authors.begin(); it != magazine.authors.end(); ++it) {
                fout << *it;
                if (it + 1 != magazine.authors.end()) {
                    fout << ", ";
                }
            }
            
            
            fout << endl;
            
        }
        fout.close();
    }
    void print_base() {
        for (auto& magazine : magazines) {
            cout << magazine.name << " - " << magazine.year << " - " << magazine.number << " - " << magazine.article_name << " - " << magazine.SCI << " citations" << " - ";
            for (auto it = magazine.authors.begin(); it != magazine.authors.end(); ++it) {
                cout << *it;
                if (it + 1 != magazine.authors.end()) {
                    cout << ", ";
                }
            }
            

            cout << endl;

        }
    }
   
    void sortbyindex() {
        sort(magazines.begin(), magazines.end(), [](magazine a, magazine b) { return a.SCI > b.SCI; });
        print_base();
    }
    void sortbyyear() {
        sort(magazines.begin(), magazines.end(), [](magazine a, magazine b) { return a.year > b.year; });
        print_base();
    }
    
    void addfromfile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error opening file." << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            const string delimiter = " - ";
            size_t pos = 0;
            string temp;
            vector<string> parts;

            while ((pos = line.find(delimiter)) != string::npos) {
                temp = line.substr(0, pos);
                parts.push_back(temp);
                line.erase(0, pos + delimiter.length());
            }
            parts.push_back(line);
            parts[4].erase(remove_if(parts[4].begin(), parts[4].end(), ::isalpha), parts[4].end());
            

            string name = parts[0];
            int year = stoi(parts[1]);
            int number = stoi(parts[2]);
            string article_name = parts[3];
            int SCI = stoi(parts[4]);

            vector<string> authors;
            if (parts.size() > 5) {
                stringstream authorStream(parts[5]);
                string author;
                while (getline(authorStream, author, ',')) {
                    
                    size_t start = author.find_first_not_of(" ");
                    size_t end = author.find_last_not_of(" ");
                    author = author.substr(start, end - start + 1);
                    authors.push_back(author);
                }
            }

            magazines.push_back(magazine(name, year, number, article_name, SCI, authors));
        }

        file.close();
 
    }
    int iscorrect(magazine mag) {
        if (mag.year < 1950 || mag.year > 2024) {
            cout << mag.article_name << "'s year is incorrect\n" << endl;
            return 1;
        }
        if (mag.number < 1) {
            cout << mag.article_name << "'s number is incorrect\n" << endl;
            return 2;
        }
        if (mag.SCI < 0) {
            cout << mag.article_name << "'s SCI is incorrect\n" << endl;
            return 3;
        }
        return 0;
    }
    void iscorrect_base() {
        for (auto& mg : magazines) {
            if (iscorrect(mg) > 0) {
                deletemag(mg.number);
            }
        }
    }
    vector<int> select_author(const string& order, const string& value) const {
        vector<int> result;
        for (unsigned int i = 0; i < magazines.size(); ++i) {
            const vector<string>& authors = magazines[i].authors;
            if (find(authors.begin(), authors.end(), value) != authors.end()) {
                result.push_back(i);
            }
        }

        return result;
    }
    /*void searchbyauthor(const string& authorName) {
        vector<int> foundIndexes;

        for (unsigned int i = 0; i < magazines.size(); ++i) {
            const vector<string>& authors = magazines[i].authors;
            if (find(authors.begin(), authors.end(), authorName) != authors.end()) {
                foundIndexes.push_back(i);
            }
        }
        ofstream fout3("to_send.txt");
        if (foundIndexes.empty()) {
            cout << "No articles found with author: " << authorName << endl;
        }
        else {
            //cout << "Articles found with author " << authorName << ":" << endl;
            for (int index : foundIndexes) {
                const magazine& mag = magazines[index];
                fout3 << mag.name << " - " << mag.year << " - " << mag.number << " - " << mag.article_name << " - " << mag.SCI << " citations - ";
                for (unsigned int j = 0; j < mag.authors.size(); ++j) {
                    fout3 << mag.authors[j];
                    if (j + 1 != mag.authors.size()) {
                        fout3 << ", ";
                    }
                }
                fout3 << endl;
            }
        }
        fout3.close();
    }*/


    vector<int> select_SCI(const string& order, const string& value1) {
        
        vector<int> result;
        
        int value = stoi(value1);
        if (order == "<") {
            for (unsigned int i = 0; i < magazines.size(); ++i) {
                const magazine& mag = magazines[i];
                if (mag.SCI < value) {
                    result.push_back(i);
                }
            }
        }
        if (order == "<=") {
            for (unsigned int i = 0; i < magazines.size(); ++i) {
                const magazine& mag = magazines[i];
                if (mag.SCI <= value) {
                    result.push_back(i);
                }
            }
        }
        if (order == ">") {
            for (unsigned int i = 0; i < magazines.size(); ++i) {
                const magazine& mag = magazines[i];
                if (mag.SCI > value) {
                    result.push_back(i);
                }
            }
        }
        if (order == ">=") {
            for (unsigned int i = 0; i < magazines.size(); ++i) {
                const magazine& mag = magazines[i];
                if (mag.SCI >= value) {
                    result.push_back(i);
                }
            }
        }
        if (order == "==") {
            for (unsigned int i = 0; i < magazines.size(); ++i) {
                const magazine& mag = magazines[i];
                if (mag.SCI == value) {
                    result.push_back(i);
                }
            }
        }
        return result;

    }
    /*void searchbyindex(int min, int max) {
        vector<int> foundIndexes;

        for (unsigned int i = 0; i < magazines.size(); ++i) {
            const magazine& mag = magazines[i];
            if (mag.SCI >= min && mag.SCI <= max) {
                foundIndexes.push_back(i);
            }
        }
        ofstream fout3("to_send.txt");
        if (foundIndexes.empty()) {
            cout << "No articles found with SCI in the range [" << min << ", " << max << "]" << endl;
        }
        else {
            //cout << "Articles found with SCI in the range [" << min << ", " << max << "]:" << endl;
            for (int index : foundIndexes) {
                const magazine& mag = magazines[index];
                fout3 << mag.name << " - " << mag.year << " - " << mag.number << " - " << mag.article_name << " - " << mag.SCI << " citations - ";
                for (unsigned int j = 0; j < mag.authors.size(); ++j) {
                    fout3 << mag.authors[j];
                    if (j + 1 != mag.authors.size()) {
                        fout3 << ", ";
                    }
                }
                fout3 << endl;
            }
        }
        fout3.close();
    }*/

    vector<int> select_year(const string& order, const string& value2) {

        vector<int> result;
        
        int value = stoi(value2);
        if (order == "<") {
            for (unsigned int i = 0; i < magazines.size(); ++i) {
                const magazine& mag = magazines[i];
                if (mag.year < value) {
                    result.push_back(i);
                }
            }
        }
        if (order == "<=") {
            for (unsigned int i = 0; i < magazines.size(); ++i) {
                const magazine& mag = magazines[i];
                if (mag.year <= value) {
                    result.push_back(i);
                }
            }
        }
        if (order == ">") {
            for (unsigned int i = 0; i < magazines.size(); ++i) {
                const magazine& mag = magazines[i];
                if (mag.year > value) {
                    result.push_back(i);
                }
            }
        }
        if (order == ">=") {
            for (unsigned int i = 0; i < magazines.size(); ++i) {
                const magazine& mag = magazines[i];
                if (mag.year >= value) {
                    result.push_back(i);
                }
            }
        }
        if (order == "==") {
            for (unsigned int i = 0; i < magazines.size(); ++i) {
                const magazine& mag = magazines[i];
                if (mag.year == value) {
                    result.push_back(i);
                }
            }
        }
        return result;

    }

    /*void searchbyyear(int min, int max) {
        vector<int> foundIndexes;

        for (unsigned int i = 0; i < magazines.size(); ++i) {
            const magazine& mag = magazines[i];
            if (mag.year >= min && mag.year <= max) {
                foundIndexes.push_back(i);
            }
        }
        ofstream fout3("to_send.txt");
        if (foundIndexes.empty()) {
            cout << "No articles found with year in the range [" << min << ", " << max << "]" << endl;
        }
        else {
            //cout << "Articles found with year in the range [" << min << ", " << max << "]:" << endl;
            for (int index : foundIndexes) {
                const magazine& mag = magazines[index];
                fout3 << mag.name << " - " << mag.year << " - " << mag.number << " - " << mag.article_name << " - " << mag.SCI << " citations - ";
                for (unsigned int j = 0; j < mag.authors.size(); ++j) {
                    fout3 << mag.authors[j];
                    if (j + 1 != mag.authors.size()) {
                        fout3 << ", ";
                    }
                }
                fout3 << endl;
            }
        }
        fout3.close();
    }*/
    vector<string> split(const string& input, char delim) {
        vector<string> tokens;
        istringstream stream(input);
        string token;

        while (getline(stream, token, delim)) {
            tokens.push_back(token);
        }
        return tokens;
    }
    vector<int> intersection(vector<int> v1, vector<int> v2) {
        vector<int> v3;

        sort(v1.begin(), v1.end());
        sort(v2.begin(), v2.end());

        set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v3));
        return v3;
    }


    vector<int> global_select(const string& query) {
        vector<string> tokens = split(query, '_');
        vector<int> mid, res;
        for (int k = 0; k <= magazines.size(); k++) {
            res.push_back(k);
        }
        for (int i = 1; i < tokens.size(); i += 3) {
            if (tokens[i] == "author") {
                mid = select_author(tokens[i + 1], tokens[i + 2]);
            }
            if (tokens[i] == "SCI") {
                mid = select_SCI(tokens[i + 1], tokens[i + 2]);

            }
            if (tokens[i] == "year") {
                mid = select_year(tokens[i + 1], tokens[i + 2]);
            }
            res = intersection(mid, res);



        }

        return res;
    }
    int query(const string& input) {
        //string input;

        //getline(cin, input);
        vector<string> tokens = split(input, '_');
        vector<int> result;


        if (tokens[0] == "select") {

            result = global_select(input);

            ofstream fout3("to_send.txt");
            for (int j = 0; j < result.size(); j++) {
                magazine mag = magazines[result[j]];
                fout3 << mag.name << " - " << mag.year << " - " << mag.number << " - " << mag.article_name << " - " << mag.SCI << " citations - ";
                for (unsigned int j = 0; j < mag.authors.size(); ++j) {
                    fout3 << mag.authors[j];
                    if (j + 1 != mag.authors.size()) {
                        fout3 << ", ";
                    }
                }
                fout3 << endl;
            }
            //fout3 << endl;
            fout3.close();
            return 0;
        }

        stringstream ss(input);
        string line;

        if (tokens[0] == "add") {
            string fout("to_send.txt");
            
            line = tokens[1];

            const string delimiter = " - ";
            size_t pos = 0;
            string temp;
            vector<string> parts;

            while ((pos = line.find(delimiter)) != string::npos) {
                temp = line.substr(0, pos);
                parts.push_back(temp);
                line.erase(0, pos + delimiter.length());
            }
            parts.push_back(line);
            parts[4].erase(remove_if(parts[4].begin(), parts[4].end(), ::isalpha), parts[4].end());


            string name = parts[0];
            int year = stoi(parts[1]);
            int number = stoi(parts[2]);
            string article_name = parts[3];
            int SCI = stoi(parts[4]);

            vector<string> authors;
            if (parts.size() > 5) {
                stringstream authorStream(parts[5]);
                string author;
                while (getline(authorStream, author, ',')) {

                    size_t start = author.find_first_not_of(" ");
                    size_t end = author.find_last_not_of(" ");
                    author = author.substr(start, end - start + 1);
                    authors.push_back(author);
                }
            }
            addmag(name, year, article_name, SCI, authors);

            print_base_file(fout);
            //fout.close();
            return 0;
        }

        if (tokens[0] == "delete") {
            if (tokens[1] == "name") {
                deletebyname(tokens[2]);
                
            }
            if (tokens[1] == "author") {
                deletebyauthor(tokens[2]);
                string fout("to_send.txt");
                print_base_file(fout);
                return 0;
            }
            if (tokens[1] == "article") {
                deletebyarticle_name(tokens[2]);
                
            }
            string fout("to_send.txt");
            print_base_file(fout);      
            //fout.close();
            return 0;
            
        }

        printf("Incorrent query\n");
        return 1;
    }

    
   
    

};

#endif





































/*void searchbyfields(const string& authorName, int minSCI, int maxSCI, int minYear, int maxYear) {
        vector<int> foundIndexes;

        for (unsigned int i = 0; i < magazines.size(); ++i) {
            const magazine& mag = magazines[i];
            bool authormatch = (authorName.empty() || find(mag.authors.begin(), mag.authors.end(), authorName) != mag.authors.end());
            bool scimatch = (mag.SCI >= minSCI && mag.SCI <= maxSCI);
            bool yearmatch = (mag.year >= minYear && mag.year <= maxYear);

            if (authormatch && scimatch && yearmatch) {
                foundIndexes.push_back(i);
            }
        }
        ofstream fout3("to_send.txt");
        if (foundIndexes.empty()) {
            cout << "No articles found with specified fields." << endl;
        }
        else {
            //cout << "Articles found with the specified fields:" << endl;
            for (int index : foundIndexes) {
                const magazine& mag = magazines[index];
                fout3 << mag.name << " - " << mag.year << " - " << mag.number << " - " << mag.article_name << " - " << mag.SCI << " citations - ";
                for (unsigned int j = 0; j < mag.authors.size(); ++j) {
                    fout3 << mag.authors[j];
                    if (j + 1 != mag.authors.size()) {
                        fout3 << ", ";
                    }
                }
                fout3 << endl;
            }
        }
        fout3.close();
    }*/

    //void searchbyindexandyear(int minSCI, int maxSCI, int minYear, int maxYear) {
    //    vector<int> foundIndexes;

    //    for (unsigned int i = 0; i < magazines.size(); ++i) {
    //        const magazine& mag = magazines[i];
    //        bool scimatch = (mag.SCI >= minSCI && mag.SCI <= maxSCI);
    //        bool yearmatch = (mag.year >= minYear && mag.year <= maxYear);

    //        if (scimatch && yearmatch) {
    //            foundIndexes.push_back(i);
    //        }
    //    }
    //    ofstream fout3("to_send.txt");
    //    if (foundIndexes.empty()) {
    //        cout << "No articles found with specified SCI and years." << endl;
    //    }
    //    else {
    //        //cout << "Articles found with the specified SCI and years:" << endl;
    //        for (int index : foundIndexes) {
    //            const magazine& mag = magazines[index];
    //            fout3 << mag.name << " - " << mag.year << " - " << mag.number << " - " << mag.article_name << " - " << mag.SCI << " citations - ";
    //            for (unsigned int j = 0; j < mag.authors.size(); ++j) {
    //                fout3 << mag.authors[j];
    //                if (j + 1 != mag.authors.size()) {
    //                    fout3 << ", ";
    //                }
    //            }
    //            fout3 << endl;
    //        }
    //    }
    //    fout3.close();
    //}

    //void searchbyauthorandindex(const string& authorName, int minSCI, int maxSCI) {
    //    vector<int> foundIndexes;

    //    for (unsigned int i = 0; i < magazines.size(); ++i) {
    //        const magazine& mag = magazines[i];
    //        bool authormatch = (authorName.empty() || find(mag.authors.begin(), mag.authors.end(), authorName) != mag.authors.end());
    //        bool scimatch = (mag.SCI >= minSCI && mag.SCI <= maxSCI);
    //        

    //        if (authormatch && scimatch) {
    //            foundIndexes.push_back(i);
    //        }
    //    }
    //    ofstream fout3("to_send.txt");
    //    if (foundIndexes.empty()) {
    //        cout << "No articles found with specified author name and SCI." << endl;
    //    }
    //    else {
    //        //cout << "Articles found with the specified author name and SCI:" << endl;
    //        for (int index : foundIndexes) {
    //            const magazine& mag = magazines[index];
    //            fout3 << mag.name << " - " << mag.year << " - " << mag.number << " - " << mag.article_name << " - " << mag.SCI << " citations - ";
    //            for (unsigned int j = 0; j < mag.authors.size(); ++j) {
    //                fout3 << mag.authors[j];
    //                if (j + 1 != mag.authors.size()) {
    //                    fout3 << ", ";
    //                }
    //            }
    //            fout3 << endl;
    //        }
    //    }
    //    fout3.close();
    //}

    //void searchbyauthorandyear(const string& authorName, int minYear, int maxYear) {
    //    vector<int> foundIndexes;

    //    for (unsigned int i = 0; i < magazines.size(); ++i) {
    //        const magazine& mag = magazines[i];
    //        bool authormatch = (authorName.empty() || find(mag.authors.begin(), mag.authors.end(), authorName) != mag.authors.end());
    //        bool yearmatch = (mag.year >= minYear && mag.year <= maxYear);

    //        if (authormatch && yearmatch) {
    //            foundIndexes.push_back(i);
    //        }
    //    }
    //    ofstream fout3("to_send.txt");
    //    if (foundIndexes.empty()) {
    //        cout << "No articles found with specified author name and years." << endl;
    //    }
    //    else {
    //        //cout << "Articles found with the specified author name and years:" << endl;
    //        for (int index : foundIndexes) {
    //            const magazine& mag = magazines[index];
    //            fout3 << mag.name << " - " << mag.year << " - " << mag.number << " - " << mag.article_name << " - " << mag.SCI << " citations - ";
    //            for (unsigned int j = 0; j < mag.authors.size(); ++j) {
    //                fout3 << mag.authors[j];
    //                if (j + 1 != mag.authors.size()) {
    //                    fout3 << ", ";
    //                }
    //            }
    //            fout3 << endl;
    //        }
    //    }
    //    fout3.close();
    //}
    /*
    int readq(){//const string& input
        string input;

        getline(cin, input);

        stringstream ss(input);
        string symbol, p1, p2, p3;
        string firstName, lastName;
        int sci1, sci2;
        int year1, year2;
        string firstn, lastn;




        ss >> symbol;

        if (symbol == "search") {
            string tag;
            ss >> tag;
            if(tag == author)
        }
        if (symbol == "search_author") {
            ss >> firstName >> lastName;
            if (ss.fail()) {
                cout << "Invalid input format. Please enter author's full name and SCI index separated by space." << endl;
                return 1;
            }
            string authorName = firstName + " " + lastName;
            searchbyauthor(authorName);
            return 0;
        }
        if (symbol == "search_sci") {
            ss >> sci1 >> sci2;
            if (ss.fail()) {
                cout << "Invalid input format. Please enter author's full name and SCI index separated by space." << endl;
                return 1;
            }
            searchbyindex(sci1, sci2);
            return 0;
        }
        if (symbol == "search_year") {
            ss >> year1 >> year2;
            if (ss.fail()) {
                cout << "Invalid input format. Please enter author's full name and SCI index separated by space." << endl;
                return 1;
            }
            searchbyyear(year1, year2);
            return 0;
        }
        if (symbol == "search_author_sci_year") {
            ss >> firstName >> lastName >> sci1 >> sci2 >> year1 >> year2;
            if (ss.fail()) {
                cout << "Invalid input format. Please enter author's full name and SCI index separated by space." << endl;
                return 1;
            }
            string authorName = firstName + " " + lastName;
            searchbyfields(authorName, sci1, sci2, year1, year2);
            return 0;
        }
        if(symbol == "search_author_and_sci"){
            ss >> firstName >> lastName >> sci1 >> sci2;
            if (ss.fail()) {
                cout << "Invalid input format. Please enter author's full name and SCI index separated by space." << endl;
                return 1;
            }
            string authorName = firstName + " " + lastName;
            searchbyauthorandindex(authorName, sci1, sci2);
            return 0;
        }


        if (symbol == "search_author_and_year") {
            ss >> firstName >> lastName >> year1 >> year2;
            if (ss.fail()) {
                cout << "Invalid input format. Please enter author's full name and SCI index separated by space." << endl;
                return 1;
            }
            string authorName = firstName + " " + lastName;
            searchbyauthorandyear(authorName, year1, year2);
            return 0;
        }

        if (symbol == "search_sci_and_year") {
            ss >> sci1 >> sci2 >> year1 >> year2;
            if (ss.fail()) {
                cout << "Invalid input format. Please enter author's full name and SCI index separated by space." << endl;
                return 1;
            }

            searchbyindexandyear(sci1, sci2, year1, year2);
            return 0;
        }
        string str;
        if (symbol == "sort") {
            ss >> str;
            if (ss.fail()) {
                cout << "Invalid input format. Please enter author's full name and SCI index separated by space." << endl;
                return 1;
            }
            if (str == "year") {
                sortbyyear();
                return 0;
            }
            if (str == "SCI") {
                sortbyindex();
                return 0;
            }
        }
        string line;

        if (symbol == "add") {
            //ss >> line;
            //getline(file >> std::ws, line);
            size_t posi = input.find(' ');
            line = input.substr(posi + 1);

                const string delimiter = " - ";
                size_t pos = 0;
                string temp;
                vector<string> parts;

                while ((pos = line.find(delimiter)) != string::npos) {
                    temp = line.substr(0, pos);
                    parts.push_back(temp);
                    line.erase(0, pos + delimiter.length());
                }
                parts.push_back(line);
                parts[4].erase(remove_if(parts[4].begin(), parts[4].end(), ::isalpha), parts[4].end());


                string name = parts[0];
                int year = stoi(parts[1]);
                int number = stoi(parts[2]);
                string article_name = parts[3];
                int SCI = stoi(parts[4]);

                vector<string> authors;
                if (parts.size() > 5) {
                    stringstream authorStream(parts[5]);
                    string author;
                    while (getline(authorStream, author, ',')) {

                        size_t start = author.find_first_not_of(" ");
                        size_t end = author.find_last_not_of(" ");
                        author = author.substr(start, end - start + 1);
                        authors.push_back(author);
                    }
                }
                addmag(name, year, article_name, SCI, authors);
                return 0;
                //magazines.push_back(magazine(name, year, number, article_name, SCI, authors));


        }
        if (symbol == "delete") {

            ss >> str;
            if (str == "author") {
                ss >> firstName >> lastName;
                string authorName = firstName + " " + lastName;
                deletebyauthor(authorName);
                return 0;
            }
            if (str == "name") {
                string magname;
                ss.ignore(1);
                getline(ss, magname);
                deletebyname(magname);
                return 0;
            }
            if (str == "number") {
                string num;
                ss >> num;
                int n{ stoi(num) };
                deletemag(n);
                return 0;
            }
            if (str == "article") {
                string artname;
                ss.ignore(1);
                getline(ss, artname);
                deletebyarticle_name(artname);
                return 0;
            }
        }
        return 1;




    }*/