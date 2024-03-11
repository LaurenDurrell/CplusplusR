
#include "Rcpp.h"
using namespace Rcpp;

// [[Rcpp::plugins("cpp17")]]

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <list>
#include <cctype>
#include <chrono>
#include <random>


//NEW EXTRA CODE TO HANDLE CHARS
class RChar {
public:
    RChar(char c) : value(c) {}
    operator char() const { return value; } // Convert RChar to char when needed
private:
    char value;
};


//TASK 1
//functions used in IS_STABLE
//getHusband FUNCTION - identify the husband of a given women
char getHusband(const char woman, const std::vector<std::pair<char, char>>& matching) {
    for (const auto& couple : matching) {
        if (couple.second == woman)
            return couple.first;
    }
    return '\0'; // Return null
}

//getWIFE FUNCTION - identify the wife of a given man
char getWife(const char man, const std::vector<std::pair<char, char>>& matching) {
    for (const auto& couple : matching) {
        if (couple.first == man)
            return couple.second;
    }
    return '\0'; // Return null
}

//INDEXER FUNCTION - find the index of a person from the values vector in the preference table
int Indexer(const std::vector<char>& preferenceList,char partner) {
    for(int i=0;i<preferenceList.size();i++){
        if(preferenceList[i]==partner) {
            return i;
        }
    }
    return -1; // Return null
    }

//SLICER FUNCTION - slice a person list of preferences at their partner, so only people who are prefered to their partner are listed
std::vector<char> Slicer(const std::vector<char>& preferenceList, int partnerIndex){
    std::vector<char> prefered;
    for(int i=0; i<partnerIndex; i++){
        prefered.push_back(preferenceList[i]);
    }
    return prefered;
}

//IS_STABLE ALGORITHM - checks if a matching is stable given two preference tables and a matching
std::string IS_STABLE(const std::unordered_map<char, std::vector<char>>& MenP,
                      const std::unordered_map<char, std::vector<char>>& WomenP,
                      const std::vector<std::pair<char, char>>& matching) {
    for (const auto& couple : matching) {
        char husband = couple.first;
        char wife = couple.second;
        auto& menPreferences = MenP.at(husband);
        int wifeI = Indexer(menPreferences,wife);
        std::vector<char> ManPrefS = Slicer(menPreferences,wifeI);
        for(int i=0;i<ManPrefS.size();i++){
            auto& womanPreferences = WomenP.at(ManPrefS[i]);
            int husbandI=Indexer(womanPreferences,getHusband(ManPrefS[i],matching));
            std::vector<char> WomanPrefS=Slicer(womanPreferences,husbandI);
            for(int j=0;j<WomanPrefS.size();j++){
                if(WomanPrefS[j]==husband){
                    return "UNSTABLE";
                }
            }
        }
    }
        return "STABLE";
}


//FUNDERMENTAL ALGORITHM - find a stable matching given two preference tables
std::vector<std::pair<char,char>> FUNDAMENTAL_ALG(const std::unordered_map<char,
                                               std::vector<char>>& MenP,
                                               const std::unordered_map<char,
                                               std::vector<char>>& WomenP) {
    char w = '\0'; // needs w to be a special character that doesn't occur in MenP and WomenP
    int k = 0;
    int n = MenP.size();
    std::vector<char> Men;
    std::vector<char> Women;
    std::unordered_map<char, char> engagements;

    for (const auto& pair : MenP) {
        Men.push_back(pair.first);
    }

    for (const auto& pair : WomenP) {
        Women.push_back(pair.first);
        engagements[pair.first] = w;
    }

    std::unordered_map<char, std::vector<char>> tempMenP = MenP; // Make an editiable copy of MenP
    std::unordered_map<char, std::vector<char>> tempWomenP = WomenP; // Make editiable copy of WomenP

    for (auto& pair : tempWomenP) {
        pair.second.push_back(w);
    }

    while (k < n) {
        char X = Men[k];
        while (X != w) {
            char x = tempMenP.at(X)[0];
            if (std::find(tempWomenP.at(x).begin(), tempWomenP.at(x).end(), X)
                < std::find(tempWomenP.at(x).begin(), tempWomenP.at(x).end(), engagements.at(x))) {
                char prevFiance = engagements.at(x);
                engagements.at(x) = X;
                X = prevFiance;
            }
            if (X != w) {
                tempMenP.at(X).erase(tempMenP.at(X).begin());
            }
        }
        k = k + 1;
    }
        // Convert engagements mapping into vector of pairs
        //code reused from previous runAll func
        std::vector<std::pair<char,char>> engagementVec;
        for (const auto& pair : engagements){
            engagementVec.push_back(pair);
        }
        // Swap the order of men and women
        for (auto& pair : engagementVec) {
            std::swap(pair.first, pair.second);
        }
    return engagementVec;
}




//TASK2
//openFile FUNCTION - reads csv preference tables
std::unordered_map<char, std::vector<char>> openFile(std::string filename) {
    std::ifstream file(filename); // Open the CSV file for reading
    std::unordered_map<char, std::vector<char>> data; // Map to store CSV data
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file." << std::endl;
        return data; // Return error code if file cannot be opened
    }
    std::string line;
    while (std::getline(file, line)) { // Read each line of the file
        std::istringstream iss(line);
        char key;
        if (iss >> key) { // Get the key from the first character of the line
            std::string cell;
            while (std::getline(iss, cell, ',')) { // Read cells from subsequent columns
                for (char c : cell) { // Store each character in the vector
                    data[key].push_back(c); // Add character to the vector associated with the key
                }
            }
        }
    }
    return data;
}

////MASHALLING TEST CODE///////
std::string marshall_string(const std::string& X)
{
    Rcout << X << std::endl;
    std::string Y {" blady blah ..."};
    return X + Y;
}


//////NEW CODE FOR MARSHALLING///////////////////
std::unordered_map<Rchar,std::vector<Rchar>> df_to_map(DataFrame& df){
    //loop through vectors to give a a map similar to open file func
    std::unordered_map<Rchar, std::vector<Rchar>> dfmap;
    Rcpp::CharacterVector col_names = df.names();
    for (int i = 0; i <df.size(); i++){
        Rchar col_name = Rcpp::as<Rchar>(col_names[i]);
        Rcpp::CharacterVector column = df[col_name];
        dfmap[col_name] = Rcpp::as<std::vector<Rchar>>(column);
    }
    return dfmap;
}

//Rcpp::DataFrame map_to_df(std::unordered_map<char,char>& unord_map){
//    //loop through map entries to give vectors
//    std::vector<char> names;
//    std::vector<char> values;
//    for (const auto& pair : unord_map) {
//        names.push_back(pair.first);
//        values.push_back(pair.second);
//    }
//    Rcpp::DataFrame df = Rcpp::DataFrame::create(
//        Rcpp::Named("names") = Rcpp::wrap(names),
//        Rcpp::Named("values") = Rcpp::wrap(values));
//    return df;
//}

DataFrame marshall_engagements(const std::vector<std::pair<Rchar,Rchar>>& engagements) {
    // vectors to store names and values
    std::vector<Rchar> names;
    std::vector<Rchar> values;
    // update vectors with engagements
    for (const auto& pair : engagements) {
        names.push_back(pair.first);
        values.push_back(pair.second);
    }
    // make rcpp df
    DataFrame df = DataFrame::create(
        Named("names") = wrap(names),
        Named("values") = wrap(values)
    );
    return df;
}


Rcpp::DataFrame fundamental_wrapper(DataFrame df1, DataFrame df2) {
    //call df_to_map for each df
    //do fundermental alg
    //do map_to_df for output
    std::unordered_map<Rchar,std::vector<Rchar>> PrefTab1 = df_to_map(df1);
    std::unordered_map<Rchar,std::vector<Rchar>> PrefTab2 = df_to_map(df2);
    std::vector<std::pair<Rchar,Rchar>> Matched_list = FUNDAMENTAL_ALG(PrefTab1, PrefTab2);
    Rcpp::DataFrame engagement_output = marshall_engagements(Matched_list);
    return engagement_output;
}


//std::string is_stable_check_wrapper(DataFrame df1, DataFrame df2) {
//    //call df_to_map twice for each df
//    //do fundermental alg
//    //do map_to_df for output
//    std::unordered_map<char,std::vector<char>> PrefTab1 = df_to_map(df1);
//    std::unordered_map<char,std::vector<char>> PrefTab2 = df_to_map(df2);
//    std::unordered_map<char,char> Matched_list = FUNDAMENTAL_ALG(PrefTab1, PrefTab2);
//    std::string stable_check = IS_STABLE(IS_STABLE, PrefTab2, Matched_list);
//    return stable_check;
//}


RCPP_MODULE(marshalling)
{
function("rcpp_marshall_string", &marshall_string);
function("fundamental_wrapper", &fundamental_wrapper);
//function("is_stable_check_wrapper", &is_stable_check_wrapper);
}
