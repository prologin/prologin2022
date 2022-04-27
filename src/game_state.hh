// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2015 Association Prologin <association@prologin.org>

#pragma once

#include <rules/game-state.hh>
#include <rules/player.hh>

#include "constant.hh"
#include "map.hh"
#include "player_info.hh"

class GameState final : public rules::GameState
{
public:
    // FIXME
    // additional parameters? for instance map
    GameState(std::istream& map_stream, const rules::Players& players);
    GameState(const GameState& st);
    ~GameState();

    GameState* copy() const override;

    // Player infos
    const PlayerInfo& get_player(int id) const;
    PlayerInfo& get_player(int id);
    const PlayerInfo& get_other(int id) const;
    PlayerInfo& get_other(int id);

    // Map
    const Map& get_map() const;

private:
    Map map_;
    std::array<PlayerInfo, 2> players_; // Troupes, scores, etc.
    int round_;                         // Note : 1 round = 2 tours
};
