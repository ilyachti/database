#include "base.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>

using namespace std;


int main() {
    library mags;

    mags.addfromfile("output.txt");
    mags.iscorrect_base();
    mags.print_base();
    //mags.searchbyauthor("Madison White");
    //mags.searchbyyear(2011, 2024);
    //mags.searchbyfields("Madison White", 1000, 7000, 2001, 2011);
    //mags.searchbyindexandyear(3000, 7900, 2001, 2011);
    mags.query();
    //mags.deletebyname("Science");
    //mags.deletebyarticle_name("\"The Future of Robotics in Healthcare\"");
    mags.print_base();
    return 0;
}