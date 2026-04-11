#pragma once
#include <map>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;                    

                                       // создаем Deck, в котором будем хранить данные карт для фронтенда
inline std::map<int, fs::path> Deck =
{
    // Старшие Арканы (0-21)
    {0, "TARO_files/0 Шут/Combinations_hariola.txt"},
    {1, "TARO_files/1 Маг/Combinations_hariola.txt"},
    {2, "TARO_files/2 Жрица/Combinations_hariola.txt"},
    {3, "TARO_files/3 Императрица/Combinations_hariola.txt"},
    {4, "TARO_files/4 Император/Combinations_hariola.txt"},
    {5, "TARO_files/5 Иерофант/Combinations_hariola.txt"},
    {6, "TARO_files/6 Влюбленные/Combinations_hariola.txt"},
    {7, "TARO_files/7 Колесница/Combinations_hariola.txt"},
    {8, "TARO_files/8 Сила/Combinations_hariola.txt"},
    {9, "TARO_files/9 Отшельник/Combinations_hariola.txt"},
    {10, "TARO_files/10 Колесо Фортуны/Combinations_hariola.txt"},
    {11, "TARO_files/11 Справедливость/Combinations_hariola.txt"},
    {12, "TARO_files/12 Повешенный/Combinations_hariola.txt"},
    {13, "TARO_files/13 Смерть/Combinations_hariola.txt"},
    {14, "TARO_files/14 Умеренность/Combinations_hariola.txt"},
    {15, "TARO_files/15 Дьявол/Combinations_hariola.txt"},
    {16, "TARO_files/16 Башня/Combinations_hariola.txt"},
    {17, "TARO_files/17 Звезда/Combinations_hariola.txt"},
    {18, "TARO_files/18 Луна/Combinations_hariola.txt"},
    {19, "TARO_files/19 Солнце/Combinations_hariola.txt"},
    {20, "TARO_files/20 Суд/Combinations_hariola.txt"},
    {21, "TARO_files/21 Мир/Combinations_hariola.txt"},

    // Жезлы (22-35)
    {22, "TARO_files/22 AceOfWands/Combinations_hariola.txt"},
    {23, "TARO_files/23 TwoOfWands/Combinations_hariola.txt"},
    {24, "TARO_files/24 ThreeOfWands/Combinations_hariola.txt"},
    {25, "TARO_files/25 FourOfWands/Combinations_hariola.txt"},
    {26, "TARO_files/26 FiveOfWands/Combinations_hariola.txt"},
    {27, "TARO_files/27 SixOfWands/Combinations_hariola.txt"},
    {28, "TARO_files/28 SevenOfWands/Combinations_hariola.txt"},
    {29, "TARO_files/29 EightOfWands/Combinations_hariola.txt"},
    {30, "TARO_files/30 NineOfWands/Combinations_hariola.txt"},
    {31, "TARO_files/31 TenOfWands/Combinations_hariola.txt"},
    {32, "TARO_files/32 PageOfWands/Combinations_hariola.txt"},
    {33, "TARO_files/33 KnightOfWands/Combinations_hariola.txt"},
    {34, "TARO_files/34 QueenOfWands/Combinations_hariola.txt"},
    {35, "TARO_files/35 KingOfWands/Combinations_hariola.txt"},

    // Кубки (36-49)
    {36, "TARO_files/36 AceOfCups/Combinations_hariola.txt"},
    {37, "TARO_files/37 TwoOfCups/Combinations_hariola.txt"},
    {38, "TARO_files/38 ThreeOfCups/Combinations_hariola.txt"},
    {39, "TARO_files/39 FourOfCups/Combinations_hariola.txt"},
    {40, "TARO_files/40 FiveOfCups/Combinations_hariola.txt"},
    {41, "TARO_files/41 SixOfCups/Combinations_hariola.txt"},
    {42, "TARO_files/42 SevenOfCups/Combinations_hariola.txt"},
    {43, "TARO_files/43 EightOfCups/Combinations_hariola.txt"},
    {44, "TARO_files/44 NineOfCups/Combinations_hariola.txt"},
    {45, "TARO_files/45 TenOfCups/Combinations_hariola.txt"},
    {46, "TARO_files/46 PageOfCups/Combinations_hariola.txt"},
    {47, "TARO_files/47 KnightOfCups/Combinations_hariola.txt"},
    {48, "TARO_files/48 QueenOfCups/Combinations_hariola.txt"},
    {49, "TARO_files/49 KingOfCups/Combinations_hariola.txt"},

    // Мечи (50-63)
    {50, "TARO_files/50 AceOfSwords/Combinations_hariola.txt"},
    {51, "TARO_files/51 TwoOfSwords/Combinations_hariola.txt"},
    {52, "TARO_files/52 ThreeOfSwords/Combinations_hariola.txt"},
    {53, "TARO_files/53 FourOfSwords/Combinations_hariola.txt"},
    {54, "TARO_files/54 FiveOfSwords/Combinations_hariola.txt"},
    {55, "TARO_files/55 SixOfSwords/Combinations_hariola.txt"},
    {56, "TARO_files/56 SevenOfSwords/Combinations_hariola.txt"},
    {57, "TARO_files/57 EightOfSwords/Combinations_hariola.txt"},
    {58, "TARO_files/58 NineOfSwords/Combinations_hariola.txt"},
    {59, "TARO_files/59 TenOfSwords/Combinations_hariola.txt"},
    {60, "TARO_files/60 PageOfSwords/Combinations_hariola.txt"},
    {61, "TARO_files/61 KnightOfSwords/Combinations_hariola.txt"},
    {62, "TARO_files/62 QueenOfSwords/Combinations_hariola.txt"},
    {63, "TARO_files/63 KingOfSwords/Combinations_hariola.txt"},

    // Пентакли (64-77)
    {64, "TARO_files/64 AceOfPentacles/Combinations_hariola.txt"},
    {65, "TARO_files/65 TwoOfPentacles/Combinations_hariola.txt"},
    {66, "TARO_files/66 ThreeOfPentacles/Combinations_hariola.txt"},
    {67, "TARO_files/67 FourOfPentacles/Combinations_hariola.txt"},
    {68, "TARO_files/68 FiveOfPentacles/Combinations_hariola.txt"},
    {69, "TARO_files/69 SixOfPentacles/Combinations_hariola.txt"},
    {70, "TARO_files/70 SevenOfPentacles/Combinations_hariola.txt"},
    {71, "TARO_files/71 EightOfPentacles/Combinations_hariola.txt"},
    {72, "TARO_files/72 NineOfPentacles/Combinations_hariola.txt"},
    {73, "TARO_files/73 TenOfPentacles/Combinations_hariola.txt"},
    {74, "TARO_files/74 PageOfPentacles/Combinations_hariola.txt"},
    {75, "TARO_files/75 KnightOfPentacles/Combinations_hariola.txt"},
    {76, "TARO_files/76 QueenOfPentacles/Combinations_hariola.txt"},
    {77, "TARO_files/77 KingOfPentacles/Combinations_hariola.txt"}
};

enum class TaroCards 
{
    //  Старшие Арканы (Major Arcana) (0-21)
    Fool = 0,             // 00 Шут
    Magician = 1,         // 01 Маг
    HighPriestess = 2,    // 02 Жрица
    Empress = 3,          // 03 Императрица
    Emperor = 4,          // 04 Император
    Hierophant = 5,       // 05 Иерофант (Жрец)
    Lovers = 6,           // 06 Влюбленные
    Chariot = 7,          // 07 Колесница
    Strength = 8,         // 08 Сила
    Hermit = 9,           // 09 Отшельник
    WheelOfFortune = 10,  // 10 Колесо Фортуны
    Justice = 11,         // 11 Справедливость
    HangedMan = 12,       // 12 Повешенный
    Death = 13,           // 13 Смерть
    Temperance = 14,      // 14 Умеренность
    Devil = 15,           // 15 Дьявол
    Tower = 16,           // 16 Башня
    Star = 17,            // 17 Звезда
    Moon = 18,            // 18 Луна
    Sun = 19,             // 19 Солнце
    Judgement = 20,       // 20 Суд
    World = 21,           // 21 Мир

    //  Младшие Арканы (Minor Arcana) (22-77)

    // --- Жезлы / Wands (22-35) ---
    AceOfWands = 22, TwoOfWands = 23, ThreeOfWands = 24, FourOfWands = 25,
    FiveOfWands = 26, SixOfWands = 27, SevenOfWands = 28, EightOfWands = 29,
    NineOfWands = 30, TenOfWands = 31, PageOfWands = 32, KnightOfWands = 33,
    QueenOfWands = 34, KingOfWands = 35,

    // --- Кубки / Cups (36-49) ---
    AceOfCups = 36, TwoOfCups = 37, ThreeOfCups = 38, FourOfCups = 40, // Ошибка в вашем прошлом списке (пропуск 39), здесь исправлено:
    FiveOfCups = 40, SixOfCups = 41, SevenOfCups = 42, EightOfCups = 43,
    NineOfCups = 44, TenOfCups = 45, PageOfCups = 46, KnightOfCups = 47,
    QueenOfCups = 48, KingOfCups = 49,

    // --- Мечи / Swords (50-63) ---
    AceOfSwords = 50, TwoOfSwords = 51, ThreeOfSwords = 52, FourOfSwords = 53,
    FiveOfSwords = 54, SixOfSwords = 55, SevenOfSwords = 56, EightOfSwords = 57,
    NineOfSwords = 58, TenOfSwords = 59, PageOfSwords = 60, KnightOfSwords = 61,
    QueenOfSwords = 62, KingOfSwords = 63,

    // --- Пентакли / Pentacles (64-77) ---
    AceOfPentacles = 64, TwoOfPentacles = 65, ThreeOfPentacles = 66, FourOfPentacles = 67,
    FiveOfPentacles = 68, SixOfPentacles = 69, SevenOfPentacles = 70, EightOfPentacles = 71,
    NineOfPentacles = 72, TenOfPentacles = 73, PageOfPentacles = 74, KnightOfPentacles = 75,
    QueenOfPentacles = 76, KingOfPentacles = 77

};