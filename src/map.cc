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
    niveau = 0;
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
                    get_cell(pos).barriere = PAS_DE_BARRIERE;
                    get_cell(pos).nid = PAS_DE_NID;
                    get_cell(pos).pigeon = PAS_DE_PIGEON;
                    get_cell(pos).papy_tours_restants = -1;
                    get_cell(pos).point_spawn = false;
                    break;
                case '.':
                    get_cell(pos).etat = etat_case(pos, VIDE, true, false);
                    get_cell(pos).barriere = PAS_DE_BARRIERE;
                    get_cell(pos).nid = PAS_DE_NID;
                    get_cell(pos).pigeon = PAS_DE_PIGEON;
                    get_cell(pos).papy_tours_restants = -1;
                    get_cell(pos).point_spawn = false;
                    break;
                case 'S':
                    get_cell(pos).etat = etat_case(pos, VIDE, false, false);
                    get_cell(pos).barriere = PAS_DE_BARRIERE;
                    get_cell(pos).nid = PAS_DE_NID;
                    get_cell(pos).pigeon = PAS_DE_PIGEON;
                    get_cell(pos).papy_tours_restants = -1;
                    get_cell(pos).point_spawn = true;
                    break;
                case 'N':
                    get_cell(pos).etat = etat_case(pos, NID, false, false);
                    get_cell(pos).barriere = PAS_DE_BARRIERE;
                    get_cell(pos).nid = LIBRE;
                    get_cell(pos).pigeon = PAS_DE_PIGEON;
                    get_cell(pos).papy_tours_restants = -1;
                    get_cell(pos).point_spawn = false;
                    break;
                case '#':
                    get_cell(pos).etat = etat_case(pos, BUISSON, false, false);
                    get_cell(pos).barriere = PAS_DE_BARRIERE;
                    get_cell(pos).nid = PAS_DE_NID;
                    get_cell(pos).pigeon = PAS_DE_PIGEON;
                    get_cell(pos).papy_tours_restants = -1;
                    get_cell(pos).point_spawn = false;
                    break;
                case 'B':
                    get_cell(pos).etat = etat_case(pos, BARRIERE, false, false);
                    get_cell(pos).barriere = FERMEE;
                    get_cell(pos).nid = PAS_DE_NID;
                    get_cell(pos).pigeon = PAS_DE_PIGEON;
                    get_cell(pos).papy_tours_restants = -1;
                    get_cell(pos).point_spawn = false;
                    break;
                case 'b':
                    get_cell(pos).etat = etat_case(pos, BARRIERE, false, false);
                    get_cell(pos).barriere = OUVERTE;
                    get_cell(pos).nid = PAS_DE_NID;
                    get_cell(pos).pigeon = PAS_DE_PIGEON;
                    get_cell(pos).papy_tours_restants = -1;
                    get_cell(pos).point_spawn = false;
                    break;
                case 'X':
                    get_cell(pos).etat = etat_case(pos, TROU, false, false);
                    get_cell(pos).barriere = OUVERTE;
                    get_cell(pos).nid = PAS_DE_NID;
                    get_cell(pos).pigeon = PAS_DE_PIGEON;
                    get_cell(pos).papy_tours_restants = -1;
                    get_cell(pos).point_spawn = false;
                    break
                default:
                    int tours = cell - '0';
                    if (tours < 0 || tours > 9)
                        FATAL("map: invalid cell type"
                              "line %d column %d",
                              ligne + 1, colonne + 1);
                    get_cell(pos).etat = etat_case(pos, PAPY, false, false);
                    get_cell(pos).barriere = PAS_DE_BARRIERE;
                    get_cell(pos).nid = PAS_DE_NID;
                    get_cell(pos).pigeon = PAS_DE_PIGEON;
                    get_cell(pos).papy_tours_restants = tours;
                    get_cell(pos).point_spawn = false;
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

    for (int ligne = 0; ligne < HAUTEUR; ligne++) {
        for (int colonne = 0; colonne < LARGEUR; colonne++) {
            position pos(colonne, ligne, -1);
            get_cell(pos).etat = etat_case(pos, TERRE, false, false);
            get_cell(pos).barriere = PAS_DE_BARRIERE;
            get_cell(pos).nid = PAS_DE_NID;
            get_cell(pos).pigeon = PAS_DE_PIGEON;
            get_cell(pos).papy_tours_restants = -1;
            get_cell(pos).point_spawn = false;
        }
    }

}
