#pragma once

#include "../Card.hpp"

#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>

class CardRepository
{
public:
    bool AddOrUpdate(Card card)
    {
        std::scoped_lock lock(mutex_);
        const bool inserted = !cards_.contains(card.id);
        cards_[card.id] = std::move(card);
        return inserted;
    }

    bool Remove(const int& id)
    {
        std::scoped_lock lock(mutex_);
        return cards_.erase(id) > 0;
    }

    std::optional<Card> GetById(const int& id) const
    {
        std::scoped_lock lock(mutex_);
        auto it = cards_.find(id);
        if (it == cards_.end())
            return std::nullopt;

        return it->second;
    }

private:
    mutable std::mutex mutex_;
    std::unordered_map<int, Card> cards_;
};