#pragma once
#include <cstdint>
#include <string>
#include <vector>
namespace tgcloud::desktop { enum class Page{home,cloud,recent,favorites,gallery,shared,transfers,backups,settings}; struct NavItem{Page page;std::string label;}; struct DashboardStats{std::uint64_t logical_bytes{},file_count{},active_transfers{};bool sync_enabled{false};}; class NavigationModel{Page current_{Page::home};public:static std::vector<NavItem> items();void navigate(Page p){current_=p;}Page current()const{return current_;}}; }
