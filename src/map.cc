#include <utils/log.hh>

#include "map.hh"
#include "position.hh"
#include "troupe.hh"

bool Case::case_praticable() const
{
    return !(etat.contenu == BUISSON || etat.contenu == TERRE ||
             (etat.contenu == BARRIERE && barriere == FERMEE));
}

void Map::mark_canard(const position& pos, PlayerInfo& player, int troupe_id)
{
    Case& cell = get_cell(pos);
    cell.troupe_id = troupe_id;
    cell.player_info = &player;
    cell.canard_sur_case = true;
}

void Map::unmark_canard(const position& pos)
{
    Case& cell = get_cell(pos);
    cell.troupe_id = -1;
    cell.player_info = nullptr;
    cell.canard_sur_case = false;
}

bool Map::case_praticable(const position& pos) const
{
    return get_cell(pos).case_praticable() && inside_map(pos);
}

const Case& Map::get_cell(const position& pos) const
{
    int indice =
        (-pos.niveau) * LARGEUR * HAUTEUR + pos.ligne * LARGEUR + pos.colonne;
    return map_[indice];
}

Case& Map::get_cell(const position& pos)
{
    int indice =
        (-pos.niveau) * LARGEUR * HAUTEUR + pos.ligne * LARGEUR + pos.colonne;
    return map_[indice];
}

const position& Map::get_spawn_toward(const direction& dir) const
{
    switch (dir)
    {
    case HAUT:
    case NORD:
        for (const auto& pos : spawns)
            if (pos.ligne == HAUTEUR - 1)
                return pos;
        break;
    case BAS:
    case SUD:
        for (const auto& pos : spawns)
            if (pos.ligne == 0)
                return pos;
        break;
    case EST:
        for (const auto& pos : spawns)
            if (pos.colonne == LARGEUR - 1)
                return pos;
        break;
    case OUEST:
        for (const auto& pos : spawns)
            if (pos.colonne == 0)
                return pos;
    }
    __builtin_unreachable();
}

void Map::load_map_cells(std::istream& stream)
{
    int niveau = 0;
    for (int ligne = 0; ligne < HAUTEUR; ligne++)
    {
        std::string line;
        if (!std::getline(stream, line))
            FATAL("map: couldn't read from the input stream (line %d)",
                  ligne + 1);
        // if (line.size() != LARGEUR)
        //     FATAL("map: line %d has %d columns, expected %d", ligne,
        //           line.size(), LARGEUR);
        for (int colonne = 0; colonne < LARGEUR; colonne++)
        {
            bool border = (colonne == 0 || colonne == LARGEUR - 1) ||
                          (ligne == 0 || ligne == HAUTEUR - 1);
            bool corner = (colonne == 0 || colonne == LARGEUR - 1) &&
                          (ligne == 0 || ligne == HAUTEUR - 1);
            position pos{colonne, ligne, niveau};
            char cell = line[colonne];
            switch (cell)
            {
            case ' ':
                get_cell(pos).etat = etat_case{pos, GAZON, false, false};
                break;
            case '.':
                get_cell(pos).etat = etat_case{pos, GAZON, true, false};
                break;
            case 'S':
                get_cell(pos).etat = etat_case{pos, GAZON, false, false};
                get_cell(pos).point_spawn = true;
                if (not border)
                    FATAL("map: spawns should be at the border of the map"
                          "line %d column %d",
                          ligne + 1, colonne + 1);
                if (corner)
                    FATAL("map: spawns should not be in a corner"
                          "line %d column %d",
                          ligne + 1, colonne + 1);
                spawns.push_back(pos);
                break;
            case 'N':
                get_cell(pos).etat = etat_case{pos, NID, false, false};
                get_cell(pos).nid = LIBRE;
                break;
            case '#':
                get_cell(pos).etat = etat_case{pos, BUISSON, false, false};
                break;
            case 'B':
                get_cell(pos).etat = etat_case{pos, BARRIERE, false, false};
                get_cell(pos).barriere = FERMEE;
                barrieres_.push_back(pos);
                break;
            case 'b':
                get_cell(pos).etat = etat_case{pos, BARRIERE, false, false};
                get_cell(pos).barriere = OUVERTE;
                barrieres_.push_back(pos);
                break;
            case 'X':
                get_cell(pos).etat = etat_case{pos, TROU, false, false};
                break;
            default:
                int tours = cell - '0';
                if (tours < 0 || tours > 9)
                    FATAL("map: invalid cell type '%c'"
                          "line %d column %d",
                          cell, ligne + 1, colonne + 1);
                get_cell(pos).etat = etat_case{pos, PAPY, false, false};
                get_cell(pos).papy_tours_restants = tours;
                papys_.push_back(pos);
                break;
            }
        }
    }
    // spawn points checks
    if (spawns.size() != 4)
        FATAL("map: invalid number of spawn points"
              "found %d expected 4",
              spawns.size());
    bool found[] = {false, false, false, false};
    for (const auto& pos : spawns)
    {
        if (pos.ligne == 0)
            found[0] = true;
        if (pos.ligne == HAUTEUR - 1)
            found[1] = true;
        if (pos.colonne == 0)
            found[2] = true;
        if (pos.colonne == LARGEUR - 1)
            found[3] = true;
    }
    if (not found[0])
        FATAL("map: expected one spawner at the top of the map");
    if (not found[1])
        FATAL("map: expected one spawner at the bottom of the map");
    if (not found[2])
        FATAL("map: expected one spawner at the left of the map");
    if (not found[3])
        FATAL("map: expected one spawner at the right of the map");
}

bool Map::case_mortelle(const position& pos) const
{
    return !case_praticable(pos) || get_cell(pos).canard_sur_case;
}

std::vector<direction> Map::directions_non_mortelles(const position& pos) const
{
    std::vector<direction> directions;
    if (!inside_map(pos))
        return directions;

    direction dirs[] = {EST, OUEST, NORD, SUD};

    for (direction dir : dirs)
    {
        if (!case_mortelle(pos + get_delta_pos(dir)))
            directions.push_back(dir);
    }

    if (pos.niveau == 0)
    {
        if (get_cell(pos).etat.contenu == TROU &&
            get_cell(pos + get_delta_pos(BAS)).etat.contenu == TUNNEL)
            directions.push_back(BAS);
    }
    else
    {
        if (get_cell(pos + get_delta_pos(HAUT)).etat.contenu == TROU)
            directions.push_back(HAUT);
    }
    return directions;
}

Map::Map(std::istream& stream)
{
    for (int niveau = -1; niveau <= 0; niveau++)
    {
        for (int ligne = 0; ligne < HAUTEUR; ligne++)
        {
            for (int colonne = 0; colonne < LARGEUR; colonne++)
            {
                position pos{colonne, ligne, niveau};
                if (niveau == -1)
                    get_cell(pos).etat = etat_case{pos, TERRE, false, false};
                else
                    get_cell(pos).etat = etat_case{pos, GAZON, false, false};
                get_cell(pos).barriere = PAS_DE_BARRIERE;
                get_cell(pos).nid = PAS_DE_NID;
                get_cell(pos).papy_tours_restants = -1;
                get_cell(pos).point_spawn = false;
                get_cell(pos).canard_sur_case = false;
            }
        }
    }

    load_map_cells(stream);
}

void Map::delete_troupe(const troupe& trp)
{
    for (auto& canard : trp.canards)
        unmark_canard(canard);
}

void Map::mark_troupe(const troupe& trp, PlayerInfo& player)
{
    for (auto& canard : trp.canards)
        mark_canard(canard, player, trp.id);
}

void Map::changer_barrieres()
{
    etat_barriere status;
    for (auto& pos : barrieres_)
    {
        if (get_cell(pos).barriere == FERMEE)
            status = OUVERTE;
        else
        {
            status = FERMEE;
            Case& cell = get_cell(pos);
            if (cell.player_info != nullptr)
                troupe_split_at(cell.troupe_id, *(cell.player_info), pos,
                                *this);
        }
        get_cell(pos).barriere = status;
    }
}

void Map::decrementer_papy(PlayerInfo& player)
{
    for (const auto& papy : papys_)
    {
        auto& cell = get_cell(papy);
        cell.papy_tours_restants -= 1;

        if (cell.papy_tours_restants <= 0)
        {
            cell.etat.nb_pains += 1;
            cell.papy_tours_restants = INTERVALLE_DISTRIB;

            // Log event
            internal_action action;
            action.type = add_bread;
            action.action.action_pos = papy;
            player.add_internal_action(action);
        }
    }
}

Map::Map(const Map& map)
    : spawns(map.spawns)
    , map_(map.map_)
    , papys_(map.papys_)
    , barrieres_(map.barrieres_)
{
}
