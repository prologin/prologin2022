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
    int spawns = 0;
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
                    spawns++;
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
    if (spawns != 4)
        FATAL("map: invalid number of spawn points"
              "found %d expected 4",
              spawns);



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
