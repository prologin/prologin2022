#include "player_info.hh"


PlayerInfo::PlayerInfo(std::shared_ptr<rules::Player> player)
    : rules_player_(std::move(player))
    , score_(0)
{
    rules_player_->score = 0;
}

int PlayerInfo::get_key() const
{
    return this->rules_player_->id;
}

int PlayerInfo::get_score() const
{
    return score_;
}

const std::string& PlayerInfo::get_name() const
{
    return rules_player_->name;
}

const std::vector<internal_action>& PlayerInfo::get_internal_history() const
{
    return internal_hist_;
}

void PlayerInfo::reset_internal_history()
{
    internal_hist_.clear();
}

void PlayerInfo::add_internal_action(internal_action action)
{
    internal_hist_.push_back(action);
}

void PlayerInfo::sync_score()
{
    rules_player_->score = score_;
}
