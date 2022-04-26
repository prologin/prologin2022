#include "player_info.hh"

PlayerInfo::PlayerInfo(std::shared_ptr<rules::Player> player, const Map& map)
    : rules_player_(std::move(player))
    , score_(0)
    , troupes_(map.spawns)
    , pains_(0)
    , mouvements_(PTS_MOUVEMENTS)
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

void PlayerInfo::increase_score(int delta)
{
    score_ += delta;
}

const std::string& PlayerInfo::get_name() const
{
    return rules_player_->name;
}

void PlayerInfo::set_name(const std::string& name)
{
    rules_player_->name = name;
}

const std::vector<position> PlayerInfo::troupes() const
{
    return troupes_;
}

int PlayerInfo::mouvements() const
{
    return mouvements_;
}

int PlayerInfo::get_pains() const
{
    return pains_;
}

void PlayerInfo::increment_pains()
{
    pains_++;
}

bool PlayerInfo::remove_pain(int delta)
{
    if (pains_ <= 0)
        return false;

    pains_--;
    return true;
}

void PlayerInfo::clear_pains()
{
    pains_ = 0;
}

const std::vector<InternalAction>& PlayerInfo::get_internal_history() const
{
    return internal_hist_;
}

void PlayerInfo::reset_internal_history()
{
    internal_hist_.clear();
}

void PlayerInfo::add_internal_action(InternalAction action)
{
    internal_hist_.push_back(action);
}

void PlayerInfo::sync_score()
{
    rules_player_->score = score_;
}
