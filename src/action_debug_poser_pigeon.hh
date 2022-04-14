// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#pragma once

#include <rules/action.hh>

#include "actions.hh"
#include "constant.hh"
#include "game_state.hh"

class ActionDebugPoserPigeon : public rules::Action<GameState>
{
public:
    ActionDebugPoserPigeon(position pos, pigeon_debug pigeon, int player_id)
        : pos_(pos)
        , pigeon_(pigeon)
        , player_id_(player_id)
    {
    }
    ActionDebugPoserPigeon() {} // for register_action()

    int check(const GameState& st) const override;
    void apply_on(GameState* st) const override;

    void handle_buffer(utils::Buffer& buf) override
    {
        buf.handle(pos_);
        buf.handle(pigeon_);
        buf.handle(player_id_);
    }

    uint32_t player_id() const override { return player_id_; };
    uint32_t id() const override { return ID_ACTION_DEBUG_POSER_PIGEON; }

private:
    position pos_;
    pigeon_debug pigeon_;
    int player_id_;
};

