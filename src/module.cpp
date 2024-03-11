
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
                    std::cout<< "husband-wife:" << husband <<"-"<<wife << std::endl;
                    std::cout << "better match:" << husband <<"-"<<ManPrefS[i] <<std::endl;
                    return "UNSTABLE";
                }
            }
        }
    }
        return "STABLE";
}

//FUNCDERMENTAL ALGORITHM - find a stable matching given two preference tables
std::unordered_map<char, char> FUNDAMENTAL_ALG(const std::unordered_map<char,
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
    return engagements;
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

    // Print the data
    std::cout << filename <<std::endl;
    for (const auto& pair : data) {
        std::cout << "Key: " << pair.first << ", Values:";
        for (char c : pair.second) {
            std::cout << " " << c;
        }
        std::cout << std::endl;
    }
    return data;
}

//TASK 3
//RUN ALL ALGORITHMS AND TIMETHE FUNCDERMENTAL ALGORITHM
void runAll(std::string womenFilename, std::string menFilename, int number){
    //read csv pref tables
    auto WomenPcsv = openFile(womenFilename);
    auto MenPcsv = openFile(menFilename);
    //run FUNDERMENTAL ALG on the csv data
    auto startTime = std::chrono::high_resolution_clock::now();// start timeer
    for(int q=0; q<number; q++){
        auto CSVengagements = FUNDAMENTAL_ALG(MenPcsv, WomenPcsv);
    }
    auto endTime = std::chrono::high_resolution_clock::now();// Stop timer
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    auto AvTime = duration/number;
    auto CSVengagements = FUNDAMENTAL_ALG(MenPcsv, WomenPcsv);
    std::cout << "Engagements: " << std::endl;
        for (const auto& pair : CSVengagements) {
            std::cout << pair.second << " is engaged to " << pair.first << std::endl;
        }
    std::cout << "Average time: " << AvTime.count() << " microseconds" << std::endl;
        //run IS STABLE on the output to double check it
        //convert engagements mapping into vector of pairs
        std::vector<std::pair<char,char>> CSVengagementVec;
        for (const auto& pair : CSVengagements){
            CSVengagementVec.push_back(pair);
        }
            //swap the order or men and women
        for (auto& pair : CSVengagementVec) {
            std::swap(pair.first, pair.second);
        }
             //run is stable on the vector
        std::cout << IS_STABLE(MenPcsv, WomenPcsv, CSVengagementVec) << std::endl;
    return;
}

std::string marshall_string(const std::string& X)
{
    Rcout << X << std::endl;
    std::string Y {" blady blah ..."};
    return X + Y;
}


//////NEW CODE TBD///////////////////
std::unordered_map<char,std::vector<char>> df_to_map(DataFrame& df){
    //loop through vectors to give a a map similar to open file func
    std::unordered_map<char, std::vector<char>> dfmap;
    Rcpp::CharacterVector col_names = df.names();
    for (int i = 0; i <df.size(); i++){
        char col_name = Rcpp::as<char>(col_names[i]);
        Rcpp::CharacterVector = df[col_name];
        dfmap[col_name] = Rcpp::as<std::vector<char>>(column);
    }
    return dfmap;
}

Rcpp::DataFrame map_to_df(std::unordered_map<char,char>& unord_map){
    //loop through map entries to give vectors
    int n = unord_map.size()
    std::vector<char> names(n);
    std::vector<char> values(n);
    int i = 0;
    for (cont auto& pair : unord_map){
        names[i] = pair.first;
        values[i] = pair.second;
        i++;
    }
    Rcpp::DataFrame df = Rcpp::DataFrame::create(Rcpp::Named("names")=names, Rcpp::Named("values")=values);
    return df;
}

Rcpp::DataFrame fundamental_wrapper(DataFrame df1, DataFrame df2) {
    //call df_to_map for each df
    //do fundermental alg
    //do map_to_df for output
    std::unordered_map<char,std::vector<char>> PrefTab1 = df_to_map(df1);
    std::unordered_map<char,std::vector<char>> PrefTab2 = df_to_map(df2);
    std::unordered_map<char,char> Matched_list = FUNDAMENTAL_ALG(PrefTab1, PrefTab2);
    Rcpp::DataFrame engagement_output= map_to_df(Matched_list);
    return engagement_output;
}

Rcpp::DataFrame is_stable_check_wrapper(DataFrame df1, DataFrame df2) {
    //call df_to_map twice for each df
    //do fundermental alg
    //do map_to_df for output
    std::unordered_map<char,std::vector<char>> PrefTab1 = df_to_map(df1);
    std::unordered_map<char,std::vector<char>> PrefTab2 = df_to_map(df2);
    std::unordered_map<char,char> Matched_list = FUNDAMENTAL_ALG(PrefTab1, PrefTab2);
    std::string stable_check = IS_STABLE(IS_STABLE, PrefTab2, Matched_list)
    return stable_check;
}


RCPP_MODULE(marshalling)
{
function("rcpp_marshall_string", &marshall_string);
function("fundamental_wrapper", &fundamental_wrapper);
function("is_stable_check_wrapper", &is_stable_check_wrapper);
}
