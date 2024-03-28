//getHusband FUNCTION - identify the husband of a given women
std::string getHusband(const std::string woman, const std::vector<std::pair<std::string, std::string>>& matching)
//getWIFE FUNCTION - identify the wife of a given man
std::string getWife(const std::string man, const std::vector<std::pair<std::string, std::string>>& matching)
//INDEXER FUNCTION - find the index of a person from the values vector in the preference table
int Indexer(const std::vector<std::string>& preferenceList,std::string partner)
//SLICER FUNCTION - slice a person list of preferences at their partner, so only people who are prefered to their partner are listed
std::vector<std::string> Slicer(const std::vector<std::string>& preferenceList, int partnerIndex)
//IS_STABLE ALGORITHM - checks if a matching is stable given two preference tables and a matching
std::string IS_STABLE(const std::unordered_map<std::string, std::vector<std::string>>& MenP,
                      const std::unordered_map<std::string, std::vector<std::string>>& WomenP,
                      const std::vector<std::pair<std::string, std::string>>& matching)
//FUNDERMENTAL ALGORITHM - find a stable matching given two preference tables
std::vector<std::pair<std::string,std::string>> FUNDAMENTAL_ALG(const std::unordered_map<std::string,
                                               std::vector<std::string>>& MenP,
                                               const std::unordered_map<std::string,
                                               std::vector<std::string>>& WomenP)
