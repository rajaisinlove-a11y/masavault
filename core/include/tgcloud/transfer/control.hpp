#pragma once
#include <atomic>
#include <string>
namespace tgcloud::transfer { enum class ErrorKind{none,offline,timeout,api,rate_limited,permission,corrupt,missing_chunk,cancelled}; struct Error{ErrorKind kind{ErrorKind::none};std::string message;}; class Cancellation{std::atomic_bool cancelled_{false};public:void cancel()noexcept{cancelled_=true;}bool is_cancelled()const noexcept{return cancelled_.load();} }; }
