#include "player_info.hh"
#include "position.hh"

void PlayerInfo::init_troupes(Map& map, etat_nid nid)
{
    auto dir = nid == JOUEUR_0 ? NORD : SUD;
    for (int i = 0; i < NB_TROUPES; i++, dir = clockwise_dir(dir))
    {
        auto head = map.get_spawn_toward(dir);
        std::vector<position> body = {head};
        for (int j = 1; j < TAILLE_DEPART; j++)
            canards_additionnels_[i].push(head);

        troupes_[i] = troupe{
            head, body, 1, inverse_dir(dir), 0, 0, i + 1,
        };

        map.mark_troupe(troupes_[i], *this);

        // Log event
        internal_action action;
        action.type = troupe_respawn;
        action.action.troupe_id = troupes_[i].id;
        action.action.action_pos = troupes_[i].maman;
        add_internal_action(action);
    }
}

PlayerInfo::PlayerInfo(std::shared_ptr<rules::Player> player, Map& map,
                       etat_nid player_nid_id)
    : rules_player_(std::move(player))
    , score_(0)
    , pains_(0)
    , tunnels_(0)
    , player_nid_id_(player_nid_id)
{
    rules_player_->score = 0;
    init_troupes(map, player_nid_id);
    reset_pts_actions();
}

int PlayerInfo::get_key() const
{
    return this->rules_player_->id;
}

rules::Player& PlayerInfo::rules_player() const
{
    return *rules_player_;
}

int PlayerInfo::get_score() const
{
    return score_;
}

etat_nid PlayerInfo::get_player_nid_id()
{
    return player_nid_id_;
}

void PlayerInfo::increase_score(uint delta)
{
    score_ += delta;
}

void PlayerInfo::decrease_score(uint delta)
{
    if (static_cast<unsigned>(score_) >= delta)
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

std::queue<position>* PlayerInfo::canards_additionnels(int troupe_id)
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

void PlayerInfo::sync_score()
{
    rules_player_->score = score_;
}

void PlayerInfo::spawn_canard(int troupe_id, Map& map)
{
    std::queue<position>* q = canards_additionnels(troupe_id);
    if (q == nullptr)
        FATAL("Bad troupe id");
    if (q->empty())
        return;
    auto last = q->front();
    q->pop();
    map.mark_canard(last, *this, troupe_id);
    get_troupe(troupe_id)->taille++;
    get_troupe(troupe_id)->canards.push_back(last);
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

bool PlayerInfo::cancel()
{
    if (internal_hist_.empty())
        return false;
    internal_hist_.pop_back();
    return true;
}
