#include <utils/log.hh>

#include "map.hh"
#include "position.hh"

bool Case::case_praticable() const
{
    return !(etat.contenu == BUISSON || etat.contenu == TERRE ||
             (etat.contenu == BARRIERE && barriere == FERMEE));
}

void Map::mark_canard(const position& pos)
{
    get_cell(pos).canard_sur_case = true; 
}

void Map::unmark_canard(const position &pos)
{
    get_cell(pos).canard_sur_case = false;
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
            if (pos.ligne == 0)
                return pos;
        break;
    case BAS:
    case SUD:
        for (const auto& pos : spawns)
            if (pos.ligne == HAUTEUR - 1)
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
        if (line.size() != LARGEUR)
            FATAL("map: line %d has %d columns, expected %d", ligne,
                  line.size(), LARGEUR);
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
                if (border)
                    FATAL("map: empty cell found at the border of the map"
                          "'%c' found line %d column %d",
                          cell, ligne + 1, colonne + 1);
                get_cell(pos).etat = etat_case{pos, GAZON, false, false};
                break;
            case '.':
                if (border)
                    FATAL("map: empty cell found at the border of the map"
                          "'%c' found line %d column %d",
                          cell, ligne + 1, colonne + 1);
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
                if (border)
                    FATAL("map: empty cell found at the border of the map"
                          "'%c' found line %d column %d",
                          cell, ligne + 1, colonne + 1);
                get_cell(pos).etat = etat_case{pos, NID, false, false};
                get_cell(pos).nid = LIBRE;
                break;
            case '#':
                get_cell(pos).etat = etat_case{pos, BUISSON, false, false};
                break;
            case 'B':
                if (border)
                    FATAL("map: empty cell found at the border of the map"
                          "'%c' found line %d column %d",
                          cell, ligne + 1, colonne + 1);
                get_cell(pos).etat = etat_case{pos, BARRIERE, false, false};
                get_cell(pos).barriere = FERMEE;
                break;
            case 'b':
                if (border)
                    FATAL("map: empty cell found at the border of the map"
                          "'%c' found line %d column %d",
                          cell, ligne + 1, colonne + 1);
                get_cell(pos).etat = etat_case{pos, BARRIERE, false, false};
                get_cell(pos).barriere = OUVERTE;
                break;
            case 'X':
                if (border)
                    FATAL("map: empty cell found at the border of the map"
                          "'%c' found line %d column %d",
                          cell, ligne + 1, colonne + 1);
                get_cell(pos).etat = etat_case{pos, TROU, false, false};
                break;
            default:
                if (border)
                    FATAL("map: empty cell found at the border of the map"
                          "'%c' found line %d column %d",
                          cell, ligne + 1, colonne + 1);
                int tours = cell - '0';
                if (tours < 0 || tours > 9)
                    FATAL("map: invalid cell type '%c'"
                          "line %d column %d",
                          cell, ligne + 1, colonne + 1);
                get_cell(pos).etat = etat_case{pos, PAPY, false, false};
                get_cell(pos).papy_tours_restants = tours;
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
    return get_cell(pos).canard_sur_case || !case_praticable(pos);
}

Map::Map(std::istream& stream)
{
    INFO("Loading map");

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
                get_cell(pos).pigeon = PAS_DE_PIGEON;
                get_cell(pos).papy_tours_restants = -1;
                get_cell(pos).point_spawn = false;
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

void Map::mark_troupe(const troupe& trp)
{
    for (auto& canard : trp.canards)
        mark_canard(canard);
}

Map::Map(const Map& map)
    : map_(map.map_)
{
}
