#include "LENORMAND.hpp"

LENORMAND::LENORMAND() 
{

}

void LENORMAND::preloadAllCards()
{
    const std::filesystem::path rootPath = "./LENORMAND_files"; // Папка для Ленорман

    if (!std::filesystem::exists(rootPath)) 
    {
        std::cout << "Ошибка: директория LENORMAND_files не найдена!" << std::endl;
        return;
    }

    for (const auto& dirEntry : std::filesystem::directory_iterator(rootPath)) 
    {
        if (!dirEntry.is_directory()) continue;
        const auto& folderPath = dirEntry.path();
        const std::string folderName = folderPath.filename().string();

        try {
            size_t spacePos = folderName.find(' ');
            int cardId = std::stoi((spacePos == std::string::npos) ? folderName : folderName.substr(0, spacePos));

            for (const auto& fileEntry : std::filesystem::directory_iterator(folderPath)) 
            {
                if (fileEntry.path().extension() == ".json") 
                {
                    std::ifstream file(fileEntry.path());
                    if (file.is_open()) 
                    {
                        nlohmann::json data;
                        file >> data;
                        allCards[cardId] = data;
                        break;
                    }
                }
            }
        }
        catch (...) { continue; }
    }
    std::cout << "Загрузка карт Ленорман завершена!" << std::endl;
}

nlohmann::json LENORMAND::getCardInfo(int cardId) {
    if (allCards.count(cardId)) return allCards[cardId];
    return nlohmann::json{};
}

std::string LENORMAND::readFileCombinations(int firstcardID, int secondcardID) 
{
    if (allCards.find(firstcardID) == allCards.end()) return "Error: Not loaded";

    const nlohmann::json& fullJson = allCards[firstcardID];
    if (fullJson.empty()) return "Error: Empty";

    const nlohmann::json& cardBody = fullJson.begin().value();
    std::string secondKey = std::to_string(secondcardID);

    if (cardBody.contains("Combinations") && cardBody["Combinations"].contains(secondKey)) 
    {
        return cardBody["Combinations"][secondKey].get<std::string>();
    }
    return "Combination not found";
}

nlohmann::json LENORMAND::addCombination(std::vector<int>& ids)
{
    if (ids.size() >= 2 && ids.size() <= 3)
    {
        std::vector<std::string> combinationsDatas;
        for (size_t i = 0; i < ids.size() - 1; ++i) 
        {
            combinationsDatas.push_back(readFileCombinations(ids[i], ids[i + 1]));
            /*combinationsDatas.push_back(readFileCombinations(ids[i + 1], ids[i]));*/ // изменяем для строго вывода информации сочетания только с одной карты 
        }
        nlohmann::json result;
        for (const auto& text : combinationsDatas) 
        {
            result["combination"].push_back({ {"text", text} });
        }
        return result;
    }
    return nlohmann::json{};
}
