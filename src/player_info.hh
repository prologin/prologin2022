#pragma once

#include <array>
#include <queue>
#include <vector>

#include <rules/player.hh>

#include "constant.hh"
#include "history.hh"
#include "map.hh"

class PlayerInfo
{
    public:
        PlayerInfo(std::shared_ptr<rules::Player> player, const Map& map,
                etat_nid player_nid_id);

        // Id
        int get_key() const;

        // Name
        const std::string& get_name() const;
        void set_name(const std::string& name);

        // Troupes
        const std::array<troupe, NB_TROUPES>& troupes() const;
        std::array<troupe, NB_TROUPES>& troupes();
        troupe* get_troupe(int troupe_id);

        void enfiler_canard(int troupe_id);
        void spawn_canard(int troupe_id, Map& map);
        const std::queue<position>* canards_additionnels(int troupe_id) const;
        std::queue<position>* canards_additionnels(int troupe_id);

        // Actions
        int pts_actions(int troupe_id) const;
        void reset_pts_actions();
        bool remove_pts_actions(int troupe_id, int delta);

        // Inventaire
        int get_pains() const;
        void increment_pains();
        void clear_pains();

        int get_tunnels() const;
        void increment_tunnels();
        void clear_tunnels();

        // History
        const std::vector<InternalAction>& get_internal_history() const;
        void add_internal_action(InternalAction action);
        void reset_internal_history();

        // Score
        int get_score() const;
        void increase_score(uint delta);
        void decrease_score(uint delta);

        etat_nid get_player_nid_id();

        void sync_score();

    private:
        std::shared_ptr<rules::Player> rules_player_;
        std::vector<InternalAction> internal_hist_;

        int score_;
        int pains_;
        int tunnels_; // Nombre de tunnels creusés ce tour
        std::array<troupe, NB_TROUPES> troupes_;
        std::array<std::queue<position>, NB_TROUPES> canards_additionnels_;
        etat_nid player_nid_id_;
};
