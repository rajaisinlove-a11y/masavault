#pragma once
#include <string>
#include "log_store.hpp"
namespace tgcloud::sync { class DeviceSync { std::string device_id_; LogStore& logs_; std::string head_; public: DeviceSync(std::string id,LogStore& l):device_id_(std::move(id)),logs_(l){} const std::string& device_id()const{return device_id_;} void set_head(std::string h){head_=std::move(h);} const std::string& head()const{return head_;} void record(model::SyncLog log){log.device_id=device_id_;logs_.append(std::move(log));} }; }
