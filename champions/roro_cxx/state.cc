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
    case VIDE:
    case BUISSON:
    case BARRIERE:
    case TROU:
    case TUNNEL:
    case TERRE:
        break;
    }
}
} // namespace

void state::fetch_state()
{
    map::apply_on_map(add_state);
}
