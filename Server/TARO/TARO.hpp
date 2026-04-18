#pragma once
#include <iostream>
#include <sstream>
#include <map> //бинарное дерево
#include <filesystem>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;

class TARO
{
public:

	TARO();

	nlohmann::json getCardInfo(int cardId);												     // функция получает id и заносит данные о карте в cards

	void preloadAllCards();																	 // функция которая будет заносить в map cards сразу все данные при запуске программы

	std::string readFileCombinations(int firstcardID, int secondcardID);					 // функция, которая будет возвращать комбинации строкой
	// по firstcardID получаем информацию из allCards, временно эту информацию храним в переменной(nlohmann::json), по 
	// secondCardID получаем информацию из объекта по ключу combinations, потом информацию возвращаем

	nlohmann::json addCombination(std::vector<int>& ids); // заносит карту в вектор combinations
	// проверка на индивидуальность 
	// после добавления id проверяем vector combinations и если >=2 вызываем readFileCombinations
	// первый раз в качестве firstCardID передается элемент под индексом 0 из вектора combinations
	// в качестве secondCardID эленмент под индексом 1. Второй раз наоборот. Если в векторе 3 элемента, функция readFileCombinations вызывается 4 раза.
	
private:

	std::map<int, nlohmann::json> allCards{}; // поменять на allCards
	//std::vector<int> combinations; // минимальный размер должен быть 2 для поиска комбинаций
	//std::vector<std::string> combinationsDatas; // сюда будет попадать вся информация, возвращенная из readFileCombinations
	
	// при каждом удалении или добавлении ID, мы каждый раз очищаем combinationsDatas
};


