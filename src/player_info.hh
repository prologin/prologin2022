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

    // Id
    int get_key() const;

    // Score
    int get_score() const;
    void increase_score(int delta);

    // Name
    const std::string& get_name() const;
    void set_name(const std::string& name);

    // Troupes
    const std::array<troupe, NB_TROUPES> troupes() const;

    // Actions
    int mouvements() const;

    // Inventaire
    int get_pains() const;
    void increment_pains();
    bool remove_pain(int delta);
    void clear_pains();

    // History
    const std::vector<InternalAction>& get_internal_history() const;
    void add_internal_action(InternalAction action);
    void reset_internal_history();

    void sync_score();

private:
    std::shared_ptr<rules::Player> rules_player_;
    std::vector<InternalAction> internal_hist_;

    int score_;
    std::array<troupe, NB_TROUPES> troupes_;
    int pains_;
    int mouvements_;
};
