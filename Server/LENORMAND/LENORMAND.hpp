#pragma once
#include <iostream>
#include <sstream>
#include <map>
#include <vector> // Обязательно для combinations и combinationsDatas
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;

class LENORMAND
{
public:

	LENORMAND();

	nlohmann::json getCardInfo(int cardId);

	void preloadAllCards();

	std::string readFileCombinations(int firstcardID, int secondcardID);

	nlohmann::json addCombination(std::vector<int>& ids);

private:

	std::map<int, nlohmann::json> allCards{};

};
