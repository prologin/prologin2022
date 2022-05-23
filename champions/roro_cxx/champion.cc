#include <iostream>

#include "api.hh"
#include "map.hh"
#include "state.hh"

/// Fonction appelée au début de la partie.
void partie_init() {}

std::ostream& operator<<(std::ostream& os, const position& pos)
{
    return os << "(" << pos.colonne << ", " << pos.ligne << ")";
}

/// Fonction appelée à chaque tour.
void jouer_tour()
{
    state::update_state();
    state::update_goals();

    int i = 0;
    for (const auto& trp : troupes_joueur(state::me))
    {
        auto& goal = state::current_goals[i];
        if (goal == std::nullopt)
        {
            i++;
            continue;
        }

        while (troupes_joueur(state::me)[i].pts_action > 0)
        {
            auto dirs =
                map::find_path(troupes_joueur(state::me)[i].maman, *goal);
            if (dirs == std::nullopt || dirs->empty())
            {
                state::update_goals(true);
                goal = state::current_goals[i];
                continue;
            }

            size_t j;
            for (j = 0; j < dirs->size() &&
                        troupes_joueur(state::me)[i].pts_action > 0;
                 j++)
                avancer(trp.id, (*dirs)[j]);

            // The goal has been reached
            if (j == dirs->size())
                state::update_goals(true);
        }

        i++;
    }
}

/// Fonction appelée à la fin de la partie.
void partie_fin() {}
