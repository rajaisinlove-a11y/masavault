#pragma once
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <string>
#include <vector>
namespace tgcloud::scheduler { struct Bot { std::string alias; bool enabled{true}; std::size_t active{0}; std::uint64_t assigned{0}; std::uint64_t failures{0}; }; class BotPool { std::vector<Bot> bots_; mutable std::mutex mutex_; std::size_t cursor_{0}; public: explicit BotPool(std::vector<Bot> bots):bots_(std::move(bots)){} std::size_t size() const {return bots_.size();} int acquire(); void release(std::size_t i,bool success); std::vector<Bot> snapshot() const; }; }
