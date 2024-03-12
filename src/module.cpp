
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


//functions used in IS_STABLE
//getHusband FUNCTION - identify the husband of a given women
std::string getHusband(const std::string woman, const std::vector<std::pair<std::string, std::string>>& matching) {
    for (const auto& couple : matching) {
        if (couple.second == woman)//loop through matching to find the wife and output the husband
            return couple.first;
    }
    return ""; // Return null
}

//getWIFE FUNCTION - identify the wife of a given man
std::string getWife(const std::string man, const std::vector<std::pair<std::string, std::string>>& matching) {
    for (const auto& couple : matching) {
        if (couple.first == man) //loop through matching to find the man and output the wife
            return couple.second;
    }
    return ""; // Return null
}

//INDEXER FUNCTION - find the index of a person from the values vector in the preference table
int Indexer(const std::vector<std::string>& preferenceList,std::string partner) {
    for(int i=0;i<preferenceList.size();i++){
        if(preferenceList[i]==partner) { //loop through all entries in preference list to find the index of the item that matches the partner
            return i;
        }
    }
    return -1; // Return null
    }

//SLICER FUNCTION - slice a person list of preferences at their partner, so only people who are prefered to their partner are listed
std::vector<std::string> Slicer(const std::vector<std::string>& preferenceList, int partnerIndex){
    std::vector<std::string> prefered;
    for(int i=0; i<partnerIndex; i++){ //loop through preference list and stop once partner reached
        prefered.push_back(preferenceList[i]); //get rid of partner from the end of the list
    }
    return prefered;
}

//IS_STABLE ALGORITHM - checks if a matching is stable given two preference tables and a matching
std::string IS_STABLE(const std::unordered_map<std::string, std::vector<std::string>>& MenP,
                      const std::unordered_map<std::string, std::vector<std::string>>& WomenP,
                      const std::vector<std::pair<std::string, std::string>>& matching) {
    for (const auto& couple : matching) { // for each couple
        std::string husband = couple.first; //husband is the first item in pair
        std::string wife = couple.second; //wide is the second item in pair
        auto& menPreferences = MenP.at(husband); //get the husbands preference list
        int wifeI = Indexer(menPreferences,wife); //gets the location of the wife in the preference list
        std::vector<std::string> ManPrefS = Slicer(menPreferences,wifeI); //get list of women preferred to wife
        for(int i=0;i<ManPrefS.size();i++){ //loop through each preferred women to see if the man from the loop above is preferred to their husband
            auto& womanPreferences = WomenP.at(ManPrefS[i]); //get the womens preference list
            int husbandI=Indexer(womanPreferences,getHusband(ManPrefS[i],matching)); //find location of their husband
            std::vector<std::string> WomanPrefS=Slicer(womanPreferences,husbandI); //get list of men preferred to their husband
            for(int j=0;j<WomanPrefS.size();j++){ //check if the man is in the womens sliced preference list
                if(WomanPrefS[j]==husband){
                    return "UNSTABLE"; //if man preferred to current husband and women preferred to current wife
                }
            }
        }
    }
        return "STABLE";
}


//FUNDERMENTAL ALGORITHM - find a stable matching given two preference tables
std::vector<std::pair<std::string,std::string>> FUNDAMENTAL_ALG(const std::unordered_map<std::string,
                                               std::vector<std::string>>& MenP,
                                               const std::unordered_map<std::string,
                                               std::vector<std::string>>& WomenP) {
    std::string w = ""; // needs w to be a special std::stringacter that doesn't occur in MenP and WomenP
    int k = 0;
    int n = MenP.size(); //number of men/women
    std::vector<std::string> Men; //create men vector
    std::vector<std::string> Women; //create women vector
    std::unordered_map<std::string, std::string> engagements; // create unordered map for engagements

    //add the least desirable man and engage all women to man the imaginary least desirable man w/omega
    for (const auto& pair : MenP) {
        Men.push_back(pair.first);
    }

    for (const auto& pair : WomenP) {
        Women.push_back(pair.first);
        engagements[pair.first] = w;
    }

    std::unordered_map<std::string, std::vector<std::string>> tempMenP = MenP; // Make an editiable copy of MenP
    std::unordered_map<std::string, std::vector<std::string>> tempWomenP = WomenP; // Make editiable copy of WomenP

    for (auto& pair : tempWomenP) {
        pair.second.push_back(w);
    }

    //loops whilst number of couples formed is less than the number of men/women
    while (k < n) {
        std::string X = Men[k];//make man k the suiter
        while (X != w) {//until you get to the least desirable man
            std::string x = tempMenP.at(X)[0];//best choice remaining on list
            if (std::find(tempWomenP.at(x).begin(), tempWomenP.at(x).end(), X) //if x preferes X to current fiance
                < std::find(tempWomenP.at(x).begin(), tempWomenP.at(x).end(), engagements.at(x))) { //then engage them
                std::string prevFiance = engagements.at(x);
                engagements.at(x) = X;
                X = prevFiance; // set the suiter to be the displaced previous fiance of x
            }
            if (X != w) { //if the suiter isn't the least desirable man, take x off X's list
                tempMenP.at(X).erase(tempMenP.at(X).begin());
            }
        }
        k = k + 1; //increase k and repeat process
    }
        // Convert engagements mapping into vector of pairs
        //code reused from previous runAll func
        std::vector<std::pair<std::string,std::string>> engagementVec;
        for (const auto& pair : engagements){
            engagementVec.push_back(pair);
        }
        // Swap the order of men and women
        for (auto& pair : engagementVec) {
            std::swap(pair.first, pair.second);
        }
    return engagementVec;
}

////MASHALLING TEST CODE///////
std::string marshall_string(const std::string& X)
{
    Rcout << X << std::endl;
    std::string Y {" blady blah ..."};
    return X + Y;
}


//////NEW CODE FOR MARSHALLING///////////////////
std::unordered_map<std::string,std::vector<std::string>> df_to_map(Rcpp::DataFrame& df){
    //loop through vectors to give a a map similar to open file func
    std::unordered_map<std::string, std::vector<std::string>> dfmap;
    Rcpp::CharacterVector col_names = df.names();
    for (int i = 0; i <df.size(); i++){
        std::string col_name = Rcpp::as<std::string>(col_names[i]);
        Rcpp::CharacterVector column = df[col_name];
        dfmap[col_name] = Rcpp::as<std::vector<std::string>>(column);
    }
    return dfmap;
}

//Rcpp::DataFrame map_to_df(std::unordered_map<std::string,std::string>& unord_map){
//    //loop through map entries to give vectors
//    std::vector<std::string> names;
//    std::vector<std::string> values;
//    for (const auto& pair : unord_map) {
//        names.push_back(pair.first);
//        values.push_back(pair.second);
//    }
//    Rcpp::DataFrame df = Rcpp::DataFrame::create(
//        Rcpp::Named("names") = Rcpp::wrap(names),
//        Rcpp::Named("values") = Rcpp::wrap(values));
//    return df;
//}

DataFrame marshall_engagements(const std::vector<std::pair<std::string,std::string>>& engagements) {
    // vectors to store names and values
    std::vector<std::string> names;
    std::vector<std::string> values;
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
    std::unordered_map<std::string,std::vector<std::string>> PrefTab1 = df_to_map(df1);
    std::unordered_map<std::string,std::vector<std::string>> PrefTab2 = df_to_map(df2);
    std::vector<std::pair<std::string,std::string>> Matched_list = FUNDAMENTAL_ALG(PrefTab1, PrefTab2);
    Rcpp::DataFrame engagement_output = marshall_engagements(Matched_list);
    return engagement_output;
}


std::string is_stable_check_wrapper(DataFrame df1, DataFrame df2) {
    //call df_to_map twice for each df
    //do fundermental alg
    //do map_to_df for output
    std::unordered_map<std::string,std::vector<std::string>> PrefTab1 = df_to_map(df1);
    std::unordered_map<std::string,std::vector<std::string>> PrefTab2 = df_to_map(df2);
    std::vector<std::pair<std::string,std::string>> Matched_list = FUNDAMENTAL_ALG(PrefTab1, PrefTab2);
    std::string stable_check = IS_STABLE(PrefTab1, PrefTab2, Matched_list);
    return stable_check;
}


RCPP_MODULE(marshalling)
{
function("rcpp_marshall_string", &marshall_string);
function("fundamental_wrapper", &fundamental_wrapper);
function("is_stable_check_wrapper", &is_stable_check_wrapper);
}
