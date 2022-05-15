// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "rules.hh"

#include <memory>
#include <sstream>
#include <utility>

#include "actions.hh"

Rules::Rules(const rules::Options opt)
    : TurnBasedRules(opt)
    , sandbox_(opt.time)
{
    if (!opt.champion_lib.empty())
    {
        champion_dll_ = std::make_unique<utils::DLL>(opt.champion_lib);

        champion_partie_init_ =
            champion_dll_->get<f_champion_partie_init>("partie_init");
        champion_jouer_tour_ =
            champion_dll_->get<f_champion_jouer_tour>("jouer_tour");
        champion_partie_fin_ =
            champion_dll_->get<f_champion_partie_fin>("partie_fin");
    }

    auto streamed_map_content = std::istringstream(opt.map_content);
    auto game_state =
        std::make_unique<GameState>(streamed_map_content, opt.players);
    api_ = std::make_unique<Api>(std::move(game_state), opt.player);
    register_actions();
}

void Rules::register_actions()
{
    api_->actions()->register_action(
        ID_ACTION_AVANCER, [] { return std::make_unique<ActionAvancer>(); });
    api_->actions()->register_action(
        ID_ACTION_GRANDIR, [] { return std::make_unique<ActionGrandir>(); });
    api_->actions()->register_action(
        ID_ACTION_CONSTRUIRE_BUISSON,
        [] { return std::make_unique<ActionConstruireBuisson>(); });
    api_->actions()->register_action(
        ID_ACTION_CREUSER_TUNNEL,
        [] { return std::make_unique<ActionCreuserTunnel>(); });
    api_->actions()->register_action(
        ID_ACTION_DEBUG_POSER_PIGEON,
        [] { return std::make_unique<ActionDebugPoserPigeon>(); });
}

rules::Actions* Rules::get_actions()
{
    return api_->actions();
}

void Rules::apply_action(const rules::IAction& action)
{
    // When receiving an action, the API should have already checked that it
    // is valid. We recheck that for the current gamestate here to avoid weird
    // client/server desynchronizations and make sure the gamestate is always
    // consistent across the clients and the server.

    int err = api_->game_state_check(action);
    if (err)
        FATAL("Synchronization error: received action %d from player %d, but "
              "check() on current gamestate returned %d.",
              action.id(), action.player_id(), err);
    api_->game_state_apply(action);
}

bool Rules::is_finished()
{
    return api_->game_state().is_finished();
}

void Rules::start_of_player_turn(unsigned int player_key)
{
    api_->game_state().get_player(player_key).reset_pts_actions();
    api_->game_state().set_init(true);
}

void Rules::end_of_player_turn(unsigned int player_key)
{
    api_->game_state().set_init(false);
    api_->clear_old_game_states();
}

void Rules::start_of_round() {}

void Rules::end_of_round() {}
