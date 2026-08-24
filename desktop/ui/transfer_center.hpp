#pragma once
#include <string>
#include <vector>
#include "tgcloud/model/domain.hpp"
#include "tgcloud/transfer/progress.hpp"
namespace tgcloud::desktop { struct TransferRow{model::TransferTask task;transfer::ProgressSnapshot progress;std::vector<std::string> active_bots;}; class TransferCenterModel{std::vector<TransferRow> rows_;public:void add(TransferRow r){rows_.push_back(std::move(r));}const std::vector<TransferRow>& rows()const{return rows_;}}; }
