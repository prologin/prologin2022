// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2015 Association Prologin <association@prologin.org>

#include "game_state.hh"
#include "constant.hh"

namespace
{
std::array<PlayerInfo, 2> init_players(const rules::Players& players, Map& map)
{
    std::vector<PlayerInfo> result;
    std::array<etat_nid, 2> joueurs = {JOUEUR_0, JOUEUR_1};
    int count = 0;

    for (const auto& player : players)
    {
        if (player->type != rules::PLAYER)
            continue;

        auto playerInfo = PlayerInfo(player, map, joueurs[count]);
        result.emplace_back(playerInfo);
        count++;
        if (count == 2)
            break;
    }

    return {result[0], result[1]};
}
} // namespace

GameState::GameState(std::istream& map_stream, const rules::Players& players)
    : rules::GameState(players)
    , map_(map_stream)
    , players_(init_players(players, map_))
    , round_(0)
    , turn_(0)
    , init_(false)
{
}

GameState::GameState(const GameState& st)
    : rules::GameState(st)
    , map_(st.map_)
    , players_(st.players_)
    , round_(st.round_)
    , turn_(st.turn_)
    , init_(st.init_)
{
}

GameState::~GameState() {}

GameState* GameState::copy() const
{
    return new GameState(*this);
}

const PlayerInfo& GameState::get_player(int id) const
{
    for (const auto& player : players_)
    {
        if (player.get_key() == id)
            return player;
    }
    FATAL("player_info: no player with id %d", id);
}

PlayerInfo& GameState::get_player(int id)
{
    const auto& const_this = *this;
    return const_cast<PlayerInfo&>(const_this.get_player(id));
}

PlayerInfo* GameState::get_player_ptr(int id)
{
    for (int i = 0; i < 2; ++i)
    {
        auto player = players_.data() + i;
        if (player->get_key() == id)
            return player;
    }

    WARN("player_info: no player with id %d", id);
    return nullptr;
}

const PlayerInfo& GameState::get_other(int id) const
{
    for (const auto& player : players_)
    {
        if (player.get_key() != id)
            return player;
    }
    FATAL("player_info: no other id than %d", id);
}

PlayerInfo& GameState::get_other(int id)
{
    const auto& const_this = *this;
    return const_cast<PlayerInfo&>(const_this.get_other(id));
}

const Map& GameState::get_map() const
{
    return map_;
}

Map& GameState::get_map()
{
    return map_;
}

int GameState::get_round() const
{
    return round_;
}

int GameState::get_turn() const
{
    return turn_;
}

void GameState::next_round()
{
    if (round_ == ROUND_FERMETURE)
        map_.changer_barrieres();
    round_++;
}

void GameState::next_turn()
{
    turn_++;
}

void GameState::reset_turn()
{
    turn_ = 0;
}

int GameState::turn_player_id() const
{
    return (turn_ + 1) % player_count();
}

bool GameState::is_finished() const
{
    return round_ >= NB_TOURS / 2;
}

bool GameState::is_init() const
{
    return init_;
}

void GameState::set_init(bool init)
{
    init_ = init;
}
