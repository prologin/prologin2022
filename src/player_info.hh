#pragma once

#include <array>
#include <vector>

#include <rules/player.hh>

#include "constant.hh"
#include "history.hh"
#include "map.hh"

class PlayerInfo
{
public:
    PlayerInfo(std::shared_ptr<rules::Player> player, const Map& map);

    // Getters
    int get_key() const;
    int get_score() const;
    const std::string& get_name() const;
    const std::vector<position> troupes() const;

    // History
    const std::vector<InternalAction>& get_internal_history() const;
    void add_internal_action(InternalAction action);
    void reset_internal_history();

    void sync_score();

private:
    std::shared_ptr<rules::Player> rules_player_;
    std::vector<InternalAction> internal_hist_;

    int score_;
    std::vector<position> troupes_;
};
