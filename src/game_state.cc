// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2015 Association Prologin <association@prologin.org>

#include "game_state.hh"

namespace
{
std::array<PlayerInfo, 2> init_players(const rules::Players& players,
                                       const Map& map)
{
    std::vector<PlayerInfo> result;

    for (const auto& player : players)
    {
        if (player->type != rules::PLAYER)
            continue;

        auto playerInfo = PlayerInfo(player, map);
        result.emplace_back(playerInfo);
    }

    return {result[0], result[1]};
}
} // namespace

GameState::GameState(std::istream& map_stream, const rules::Players& players)
    : rules::GameState(players)
    , map_(map_stream)
    , players_(init_players(players, map_))
{
    // FIXME
}

GameState::GameState(const GameState& st)
    : rules::GameState(st)
    , map_(st.map_)
    , players_(st.players_)
    , round_(st.round_)
{
    // FIXME
}

GameState::~GameState()
{
    // FIXME
}

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
