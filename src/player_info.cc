#include "player_info.hh"
#include "position.hh"

namespace
{
std::array<troupe, NB_TROUPES> init_troupes(const rules::Player& player,
                                            const Map& map)
{
    std::array<troupe, NB_TROUPES> troupes;

    rules::Player other;

    auto dir = player.id < other.id ? NORD : SUD;
    for (int i = 1; i < NB_TROUPES; i++, dir = clockwise_dir(dir))
    {
        auto head = map.get_spawn_toward(dir);
        std::vector<position> body;
        for (int j = 1; j < TAILLE_DEPART; j++)
            body.push_back(head);

        troupes[i - 1] = troupe{head, body, TAILLE_DEPART, inverse_dir(dir), i};
    }

    return troupes;
}
} // namespace

PlayerInfo::PlayerInfo(std::shared_ptr<rules::Player> player, const Map& map)
    : rules_player_(std::move(player))
    , score_(0)
    , pains_(0)
    , troupes_(init_troupes(*rules_player_, map))
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

std::array<troupe, NB_TROUPES>& PlayerInfo::troupes()
{
    return troupes_;
}

const std::array<troupe, NB_TROUPES>& PlayerInfo::troupes() const
{
    return troupes_;
}

int PlayerInfo::mouvements(int troupe_id) const
{
    for (int i = 0; i < troupes_.size(); ++i)
        if (troupes_[i].id == troupe_id)
            return mouvements_[i];
}

void PlayerInfo::reset_mouvements()
{
    for (auto& mv : mouvements_)
        mv = PTS_MOUVEMENTS;
}

bool PlayerInfo::remove_mouvements(int troupe_id, int delta)
{
    for (int i = 0; i < troupes_.size(); ++i)
    {
        if (troupes_[i].id == troupe_id)
        {
            if (mouvements_[i] < delta)
                return false;

            mouvements_[i] -= delta;
            return true;
        }
    }

    return false;
}

std::shared_ptr<troupe> PlayerInfo::get_troupe(int troupe_id)
{
    for (auto trp : troupes_)
    {
        if (trp.id == troupe_id)
            return std::make_shared<troupe>(trp);
    } 
    return nullptr;
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
    if (pains_ < delta)
        return false;

    pains_ -= delta;
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
