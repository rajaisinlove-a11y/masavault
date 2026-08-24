#pragma once
#include <chrono>
#include <cstdint>
namespace tgcloud::retry { struct Decision{bool retry;std::chrono::milliseconds delay;}; class Policy{std::uint32_t max_,base_; public:Policy(std::uint32_t max_attempts=5,std::uint32_t base_ms=250):max_(max_attempts),base_(base_ms){} Decision decide(std::uint32_t attempt,bool transient,std::uint32_t retry_after=0)const;}; }
