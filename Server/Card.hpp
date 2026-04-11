#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

struct Card
{
    int id;
    std::string name;
};

inline void to_json(nlohmann::json& j, const Card& c)
{
    j = nlohmann::json{
        {"id", c.id},
        { "name", c.name }
    };
}

inline void from_json(const nlohmann::json& j, Card& c)
{
    if (j.contains("id"))        j.at("id").get_to(c.id);
    if (j.contains("name"))      j.at("name").get_to(c.name);
}