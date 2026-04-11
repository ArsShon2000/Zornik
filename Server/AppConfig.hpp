#pragma once

#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <string>
#include <nlohmann/json.hpp>

struct AppConfig
{
    std::string host = "0.0.0.0";
    std::uint16_t port = 8080;
    std::size_t threads = 4;
};

inline AppConfig LoadConfig(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open config file: " + filePath);
    }

    nlohmann::json j;
    file >> j;

    AppConfig cfg;

    if (j.contains("host"))    j.at("host").get_to(cfg.host);
    if (j.contains("port"))    j.at("port").get_to(cfg.port);
    if (j.contains("threads")) j.at("threads").get_to(cfg.threads);

    if (cfg.host.empty())
        throw std::runtime_error("Config error: host is empty");

    if (cfg.port == 0)
        throw std::runtime_error("Config error: port must be > 0");

    if (cfg.threads == 0)
        cfg.threads = 1;

    return cfg;
}