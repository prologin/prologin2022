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
    for (int i = 0; i < NB_TROUPES; i++, dir = clockwise_dir(dir))
    {
        auto head = map.get_spawn_toward(dir);
        std::vector<position> body;
        for (int j = 0; j < TAILLE_DEPART; j++)
            body.push_back(head);

        troupes[i] = troupe{
            head, body, TAILLE_DEPART, inverse_dir(dir), 0, 0, i + 1,
        };
    }

    return troupes;
}
} // namespace

PlayerInfo::PlayerInfo(std::shared_ptr<rules::Player> player, const Map& map,
				etat_nid player_nid_id)
    : rules_player_(std::move(player))
    , score_(0)
    , pains_(0)
    , tunnels_(0)
    , troupes_(init_troupes(*rules_player_, map))
	, player_nid_id_(player_nid_id)
{
    rules_player_->score = 0;
    reset_pts_actions();
}

int PlayerInfo::get_key() const
{
    return this->rules_player_->id;
}

int PlayerInfo::get_score() const
{
    return score_;
}

void PlayerInfo::increase_score(uint delta)
{
    score_ += delta;
}

void PlayerInfo::decrease_score(uint delta)
{
    if (score_ >= delta)
        score_ -= delta;
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

int PlayerInfo::pts_actions(int troupe_id) const
{
    for (auto& trp : troupes_)
        if (trp.id == troupe_id)
            return trp.pts_actions;

    return -1;
}

void PlayerInfo::reset_pts_actions()
{
    for (auto& trp : troupes_)
        trp.pts_actions = PTS_ACTION;
}

bool PlayerInfo::remove_pts_actions(int troupe_id, int delta)
{
    for (auto& trp : troupes_)
    {
        if (trp.id == troupe_id)
        {
            if (trp.pts_actions < delta)
                return false;

            trp.pts_actions -= delta;
            return true;
        }
    }

    return false;
}

troupe* PlayerInfo::get_troupe(int troupe_id)
{
    for (auto& trp : troupes_)
    {
        if (trp.id == troupe_id)
            return &trp;
    }
    return nullptr;
}

void PlayerInfo::enfiler_canard(int troupe_id)
{
    for (size_t i = 0; i < troupes_.size(); ++i)
    {
        if (troupes_[i].id == troupe_id)
        {
            auto back = troupes_[i].canards.back();
            canards_additionnels_[i].push(back);
        }
    }
}

const std::queue<position>*
PlayerInfo::canards_additionnels(int troupe_id) const
{
    for (size_t i = 0; i < troupes_.size(); ++i)
        if (troupes_[i].id == troupe_id)
            return &canards_additionnels_[i];

    return nullptr;
}

std::queue<position> *PlayerInfo::canards_additionnels(int troupe_id)
{
    for (size_t i = 0; i < troupes_.size(); ++i)
        if (troupes_[i].id == troupe_id)
            return &canards_additionnels_[i];

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

void PlayerInfo::clear_pains()
{
    pains_ = 0;
}

int PlayerInfo::get_tunnels() const
{
    return tunnels_;
}

void PlayerInfo::increment_tunnels()
{
    tunnels_++;
}

void PlayerInfo::clear_tunnels()
{
    // Devrait être appelé une fois au début de chaque tour
    tunnels_ = 0;
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

void PlayerInfo::spawn_canard(int troupe_id, Map& map)
{
    std::queue<position> *q = canards_additionnels(troupe_id);
    if (q == nullptr)
        FATAL("Bad troupe id");
    if (q->empty())
        return;
    auto last = q->front();
    q->pop();
    map.mark_canard(last); 
    get_troupe(troupe_id)->canards.push_back(last);
}
