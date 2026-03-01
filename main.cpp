// Winter'24
// Instructor: Diba Mirza
// Student name: Arun Ghoshal
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <cstring>
#include <algorithm>
#include <limits.h>
#include <iomanip>
#include <set>
#include <queue>
#include <sstream>
#include <unordered_map>
using namespace std;

#include "utilities.h"
#include "movies.h"

bool parseLine(string &line, string &movieName, double &movieRating);

int main(int argc, char** argv){
    if (argc < 2){
        cerr << "Not enough arguments provided (need at least 1 argument)." << endl;
        cerr << "Usage: " << argv[ 0 ] << " moviesFilename prefixFilename " << endl;
        exit(1);
    }

    ifstream movieFile (argv[1]);
 
    if (movieFile.fail()){
        cerr << "Could not open file " << argv[1];
        exit(1);
    }
  
    string line, movieName;
    double movieRating;

    if (argc == 2){
            // Create an object of a STL data-structure to store all the movies

            set<pair<string,double>> storage;
            // Read each file and store the name and rating
            while (getline (movieFile, line) && parseLine(line, movieName, movieRating)){
                    // Use std::string movieName and double movieRating
                    // to construct your Movie objects

                    pair<string,double> p(movieName,movieRating);
                    storage.insert(p);
                    //cout << movieName << " has rating " << movieRating << endl;
                    // insert elements into your data structure
            }

            movieFile.close();

            //print all the movies in ascending alphabetical order of movie names

            for (pair<string,double> p : storage){
                cout << p.first << ", " << p.second << endl;
            }

            return 0;
    }

    ifstream prefixFile (argv[2]);

    if (prefixFile.fail()) {
        cerr << "Could not open file " << argv[2];
        exit(1);
    }

    unordered_map<string,set<pair<double,string>, greater<pair<double,string>> >> storage;
    vector<string> prefixes;
    while (getline (prefixFile, line)) {
        if (!line.empty()) {
            prefixes.push_back(line);
            set< pair<double,string>, greater<pair<double,string>>> emptyset;
            storage[line] = emptyset;
        }
    }


    while (getline (movieFile, line) && parseLine(line, movieName, movieRating)){
            // Use std::string movieName and double movieRating
            // to construct your Movie objects
            pair<double,string> p(movieRating, movieName);

            string substr;
            if (storage.contains(movieName.substr(0,3))) {
                substr = movieName.substr(0,3);
                storage[substr].insert(p);
                
                //cout << movieName << " inserted into the " << substr << " bucket \n";

            }

            if (storage.contains(movieName.substr(0,2))) {
                substr = movieName.substr(0,2);
                storage[substr].insert(p);

                //cout << movieName << " inserted into the " << substr << " bucket \n";
            }

            if (storage.contains(movieName.substr(0,1))) {
                substr = movieName.substr(0,1);
                storage[substr].insert(p);

                //cout << movieName << " inserted into the " << substr << " bucket \n";
            }

        }

    movieFile.close();
    //storage is a "data structure" with all the movies in it
    //prefixes is a vector with all prefixes

    //so... can we just make storage a hashtable using prefixes as a keys and write it off as our initial data structure :3 ?


    //IF I DO THIS, IT BECOMES VERY EASY LOL, I REALLY HOPE THAT WHAT I DID WITH STORAGE WAS ALLOWED
    

    //this is all incorrect, naive first idea
    //implementing a movie class is not a requirement for this (lowk pair<> is good enouch) so i guess implement helper funcs there

    //  Initial setup:
    //  Create a hashmap
    //  For each prefix, create a key-value pair where each key is a prefix as a string and value is a BST
    //  insert all movie pairs in the storage BST that start with a certain prefix into the associated BST of the prefix within the hashmap. 

    //  On query of a prefix:
    //  Copy all the movie pairs in the query prefix BST into a max priority queue, iterating backwards through the BST. Also reverse the pair order.
    //  If instead the query prefix BST's is empty, print the following message

    //for each prefix...

    //listing them out
    for (int i = 0; i < prefixes.size(); i++){
        string prefix = prefixes[i];

        set<pair<double,string>, greater<pair<double,string>> > movieSet = storage[prefix];

        if (movieSet.empty() == true) {
            cout << "No movies found with prefix "<< prefix << "\n";
        } else {
            
            set<pair<string,double>> alpha;
            double lastRating = -1.0;

            for (pair<double,string> p : movieSet){
                
                if (lastRating != p.first){
                    //printall
                    for (pair<string,double> p2 : alpha){
                        cout << p2.first << ", " << p2.second << "\n";
                    }
                    alpha.clear();
                }

                pair<string,double> p2(p.second,p.first);
                alpha.insert(p2);

                lastRating = p.first;
            }

            for (pair<string,double> p2 : alpha){
                cout << p2.first << ", " << p2.second << "\n";
            }
            cout << "\n";
        }
        
    }

    //the best movie
    for (int i = 0; i < prefixes.size(); i++){
        string prefix = prefixes[i];

        set<pair<double,string>, greater<pair<double,string>> > movieSet = storage[prefix];

        if (movieSet.empty() == false) {

            set<pair<string,double>> alpha;
            double lastRating = (*movieSet.begin()).first;

            for (pair<double,string> p : movieSet){
                
                if (lastRating != p.first){
                    break;
                }

                pair<string,double> p2(p.second,p.first);
                alpha.insert(p2);

                lastRating = p.first;
            }

            cout << "Best movie with prefix " << prefix << " is: " << (*alpha.begin()).first << " with rating " << std::fixed << std::setprecision(1) << (*alpha.begin()).second << "\n";
            
        }
        
    }

    return 0;
}

/* Add your run time analysis for part 3 of the assignment here as commented block*/

bool parseLine(string &line, string &movieName, double &movieRating) {
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex+1));
    if (movieName[0] == '\"') {
        movieName = movieName.substr(1, movieName.length() - 2);
    }
    return true;
}