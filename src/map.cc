#include "map.hh"


const Cell& Map::get_cell(const position& pos) const
{
    int indice = (-pos.niveau) * LARGEUR * HAUTEUR
        + pos.ligne * LARGEUR
        + pos.colonne;
    return map_[indice];
}

Cell& Map::get_cell(const position& pos)
{
    int indice = (-pos.niveau) * LARGEUR * HAUTEUR
        + pos.ligne * LARGEUR
        + pos.colonne;
    return map_[indice];
}

void Map::load_map_cells(std::istream& stream)
{
    int niveau = 0;
    vector<position> spawns;
    for (int ligne = 0; y < HAUTEUR; y++)
    {
        for (int colonne = 0; x < LARGEUR; x++)
        {
            position pos(colonne, ligne, niveau);
            std::char cell;
            stream >> cell;
            switch (cell)
            {
                case ' ':
                    get_cell(pos).etat = etat_case(pos, VIDE, false, false);
                    break;
                case '.':
                    get_cell(pos).etat = etat_case(pos, VIDE, true, false);
                    break;
                case 'S':
                    get_cell(pos).etat = etat_case(pos, VIDE, false, false);
                    get_cell(pos).point_spawn = true;
                    if (colonne > 0 && colonne < LARGEUR-1 
                            && ligne > 0 && ligne < HAUTEUR-1)
                        FATAL("map: spawns should be at the border of the map"
                              "line %d column %d",
                              ligne + 1, colonne + 1);
                    if ((colonne == 0 || colonne == LARGEUR - 1)
                            && (ligne == 0 || ligne == HAUTEUR - 1))
                        FATAL("map: spawns should not be in a corner"
                              "line %d column %d",
                              ligne + 1, colonne + 1);
                    spawns.push_back(pos);
                    break;
                case 'N':
                    get_cell(pos).etat = etat_case(pos, NID, false, false);
                    get_cell(pos).nid = LIBRE;
                    break;
                case '#':
                    get_cell(pos).etat = etat_case(pos, BUISSON, false, false);
                    break;
                case 'B':
                    get_cell(pos).etat = etat_case(pos, BARRIERE, false, false);
                    get_cell(pos).barriere = FERMEE;
                    break;
                case 'b':
                    get_cell(pos).etat = etat_case(pos, BARRIERE, false, false);
                    get_cell(pos).barriere = OUVERTE;
                    break;
                case 'X':
                    get_cell(pos).etat = etat_case(pos, TROU, false, false);
                    break
                default:
                        int tours = cell - '0';
                        if (tours < 0 || tours > 9)
                            FATAL("map: invalid cell type '%c'"
                                    "line %d column %d",
                                    cell, ligne + 1, colonne + 1);
                        get_cell(pos).etat = etat_case(pos, PAPY, false, false);
                        get_cell(pos).papy_tours_restants = tours;
                        break;
            }
        }
        std::char check;
        stream >> check;
        if (check != '\n')
            FATAL("map: invalid line length"
                    "EoL expected line %d column %d",
                    ligne + 1, LARGEUR + 1);

    }
    // spawn points checks
    if (spawns.size() != 4)
        FATAL("map: invalid number of spawn points"
              "found %d expected 4",
              spawns.size());
    bool found[] = {false, false, false, false}
    for (position pos: spawns) {
        if (pos.ligne == 0)
            found[0] = true;
        if (pos.ligne == HAUTEUR-1)
            found[1] = true;
        if (pos.colonne == 0)
            found[2] = true;
        if (pos.colonne == LARGEUR-1)
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

Map::Map(std::istream& stream)
{
    INFO("Loading map");

    for (int niveau = -1; niveau <= 0; niveau++) {
        for (int ligne = 0; ligne < HAUTEUR; ligne++) {
            for (int colonne = 0; colonne < LARGEUR; colonne++) {
                position pos(colonne, ligne, niveau);
                if (niveau == -1)
                    get_cell(pos).etat = etat_case(pos, TERRE, false, false);
                else
                    get_cell(pos).etat = etat_case(pos, VIDE, false, false);
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

Map::Map(const Map& map) : map_(map.map_)
{
}
