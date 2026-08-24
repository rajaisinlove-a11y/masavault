#pragma once
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
namespace tgcloud::transfer { class TransferQueue { std::queue<std::function<void()>> q_; std::vector<std::thread> workers_; std::mutex m_; std::condition_variable cv_; bool stop_{false}; void run(); public: explicit TransferQueue(std::size_t n=1); ~TransferQueue(); void submit(std::function<void()> job); }; }
