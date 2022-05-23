#include "state.hh"
#include "map.hh"

namespace
{
void add_state(position pos)
{
    auto cell = info_case(pos);
    switch (cell.contenu)
    {
    case PAPY:
    {
        auto turns_left = papy_tours_restants(pos);
        state::grandpa grandpa{pos, turns_left};
        state::grandpas.emplace_back(grandpa);
        break;
    }
    case NID:
    {
        auto nest_state = info_nid(pos);
        state::nest nest{pos, nest_state};
        state::nests.emplace_back(nest);
        break;
    }
    case GAZON:
    case BUISSON:
    case BARRIERE:
    case TROU:
    case TUNNEL:
    case TERRE:
        break;
    }
}
} // namespace

void state::update_state()
{
    map::apply_on_map(add_state);

    if (state::me == -1)
        state::me = moi();
    if (state::other == -1)
        state::other = adversaire();
}

void state::update_goals(bool force)
{
    // Get data about the nests
    int nb_my_nests = 0;
    int total_nests = 0;
    etat_nid my_nest_state = state::me < state::other ? JOUEUR_0 : JOUEUR_1;
    std::vector<position> nests;
    map::apply_on_map([&nests, &nb_my_nests, &total_nests,
                       my_nest_state](const auto& position) {
        if (info_case(position).contenu != NID)
            return;

        total_nests++;
        etat_nid nest_state = info_nid(position);
        if (nest_state == my_nest_state)
            nb_my_nests++;
        else if (nest_state == LIBRE)
            nests.push_back(position);
    });

    auto troupes = troupes_joueur(state::me);
    int i = 0;
    for (const auto& trp : troupes)
    {
        auto& goal = state::current_goals[i++];
        // add random to prevent infinite loops in case the map state is updated
        if (!force && goal != std::nullopt && rand() > RAND_MAX / PTS_ACTION)
            continue;

        if (4 * nb_my_nests < total_nests &&
            nests.size() >= static_cast<size_t>(total_nests - nb_my_nests))
            goal = [](const auto& pos) { return info_nid(pos) == LIBRE; };
        else if (trp.inventaire < 1) // TODO: compute this value dynamically
            goal = [](const auto& pos) { return info_case(pos).nb_pains > 0; };
        else
            goal = [my_nest_state](const auto& pos) {
                etat_nid nest_state = info_nid(pos);
                return nest_state == my_nest_state || nest_state == LIBRE;
            };
    }
}
