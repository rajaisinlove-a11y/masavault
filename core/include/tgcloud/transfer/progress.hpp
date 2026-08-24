#pragma once
#include <chrono>
#include <cstdint>
#include <mutex>
namespace tgcloud::transfer { struct ProgressSnapshot{std::uint64_t completed{},total{};double bytes_per_second{},eta_seconds{};}; class Progress{std::uint64_t total_{},done_{}; std::chrono::steady_clock::time_point start_; mutable std::mutex m_; public: explicit Progress(std::uint64_t total):total_(total),start_(std::chrono::steady_clock::now()){} void update(std::uint64_t completed); ProgressSnapshot snapshot()const;}; }
