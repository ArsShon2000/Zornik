#include "TARO.hpp"
#include "DECK.hpp"

TARO::TARO() 
{

}

void TARO::preloadAllCards()
{
    const std::filesystem::path rootPath = "./TARO_files";

    if (!std::filesystem::exists(rootPath))
    {
        std::cout << "Критическая ошибка: директория TARO_files не найдена!" << std::endl;
        return;
    }

    for (const auto& dirEntry : std::filesystem::directory_iterator(rootPath))
    {
        if (!dirEntry.is_directory())
            continue;

        const auto& folderPath = dirEntry.path();
        const std::string folderName = folderPath.filename().string();

        try
        {
            // Берем ID до первого пробела
            size_t spacePos = folderName.find(' ');
            std::string idStr = (spacePos == std::string::npos)
                ? folderName
                : folderName.substr(0, spacePos);

            int cardId = std::stoi(idStr);

            // Ищем JSON внутри папки, не надо уходить в другие функции
            for (const auto& fileEntry : std::filesystem::directory_iterator(folderPath))
            {
                if (fileEntry.path().extension() == ".json")
                {
                    std::ifstream file(fileEntry.path());
                    if (!file.is_open())
                    {
                        std::cout << "Ошибка открытия файла: " << fileEntry.path() << std::endl;
                        continue;
                    }

                    nlohmann::json data;
                    file >> data;

                    allCards[cardId] = data;

                    std::cout << "Карта " << cardId << " загружена." << std::endl;
                    break;
                }
            }
        }
        catch (const std::exception&)
        {
            // просто пропускаем мусорные папки
            continue;
        }
    }

    std::cout << "Загрузка всех карт завершена!" << std::endl;
}


nlohmann::json TARO::getCardInfo(int cardId) // функция получает id и заносит данные о карте в cards
{
    auto it = allCards.find(cardId);
    if (it != allCards.end())
    {
        return it->second;
    }

    std::cout << "Карта с ID " << cardId << " не найдена в кэше!" << std::endl;
    return nlohmann::json{};
}

std::string TARO::readFileCombinations(int firstcardID, int secondcardID)  // функция, которая будет возвращать комбинации строкой
{
    // Проверяем, есть ли данные по первой карте в контейнере
    if (TARO::allCards.find(firstcardID) == TARO::allCards.end())
    {
        return "Error: First card data not loaded";
    }

    // Берем JSON-объект первой карты
    const nlohmann::json& fullJson = TARO::allCards[firstcardID];

    // Проверяем, что JSON не пустой
    if (fullJson.empty()) return "Error: JSON is empty";

    // Заходим во внутренний объект (тот, что с именем карты, напр. "Fool")
    // Используем итератор, чтобы не прописывать имя карты строкой
    const nlohmann::json& cardBody = fullJson.begin().value();

    // Ищем комбинацию по ID второй карты (ключ в JSON — это строка)
    std::string secondKey = std::to_string(secondcardID);

    if (cardBody.contains("Combinations") && cardBody["Combinations"].contains(secondKey)) 
    {
        // Возвращаем текст комбинации
        return cardBody["Combinations"][secondKey].get<std::string>();
    }

    return "Combination not found";
}

nlohmann::json TARO::addCombination(std::vector<int>& ids)
{
    // Если в векторе 2 и более элемента — вычисляем все возможные пары
    if (ids.size() >= 2 && ids.size() <= 3)
    {
        std::vector<std::string> combinationsDatas;
        // Идем только по соседним парам: (0,1) и (1,10)
        for (size_t i = 0; i < ids.size() - 1; ++i) 
        {
            int first = ids[i];
            int second = ids[i + 1];

            // 1. Пара AB (например, 0 и 1)
            combinationsDatas.push_back(readFileCombinations(first, second));

            // 2. Пара BA (например, 1 и 0)
            /*combinationsDatas.push_back(readFileCombinations(second, first));*/ // изменяем для строго вывода информации сочетания только с одной карты
        }

        nlohmann::json result;
        result["combination"] = nlohmann::json::array();
        for (const auto& text : combinationsDatas)
        {
            result["combination"].push_back(nlohmann::json{ {"text", text} });
        }

        return result;
    }
    else return nlohmann::json{}; // Если карт меньше 2, возвращаем пустой JSON
}
