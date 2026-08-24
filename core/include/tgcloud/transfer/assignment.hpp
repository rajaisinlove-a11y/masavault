#pragma once
#include <vector>
#include "tgcloud/model/domain.hpp"
#include "tgcloud/scheduler/bot_pool.hpp"
namespace tgcloud::transfer { std::vector<int> assign_chunks(std::vector<model::ChunkRecord>& chunks, scheduler::BotPool& pool); }
