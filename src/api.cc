// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "api.hh"
#include "constant.hh"
#include "position.hh"
#include "utils.hh"

#include <memory>
#include <utility>

// global used in interface.cc
Api* api;

Api::Api(std::unique_ptr<GameState> game_state,
         std::shared_ptr<rules::Player> player)
    : rules::Api<GameState, erreur>(std::move(game_state), player)
{
    api = this;
}

etat_case Api::info_case(position pos)
{
    if (!inside_map(pos))
    {
        const etat_case etatCase = {
            .pos = {-1, -1, -1},
            .contenu = static_cast<type_case>(-1),
            .est_constructible = false,
            .nb_pains = -1,
        };

        return etatCase;
    }
    return game_state_->get_map().get_cell(pos).etat;
}
etat_barriere Api::info_barriere(position pos)
{
    if (!inside_map(pos))
        return PAS_DE_BARRIERE;

    return game_state_->get_map().get_cell(pos).barriere;
}
etat_nid Api::info_nid(position pos)
{
    if (!inside_map(pos))
        return PAS_DE_NID;

    return game_state_->get_map().get_cell(pos).nid;
}
int Api::papy_tours_restants(position pos)
{
    if (!inside_map(pos))
        return -1;

    return game_state_->get_map().get_cell(pos).papy_tours_restants;
}
std::vector<troupe> Api::troupes_joueur(int id_joueur)
{
    auto player = game_state_->get_player_ptr(id_joueur);
    std::vector<troupe> troupes;
    if (player == nullptr)
        return {INVALID_TROUPE};
    std::array<troupe, NB_TROUPES> troupesArr = player->troupes();
    for (auto troupe : troupesArr)
        troupes.push_back(troupe);
    return troupes;
}
std::vector<position> Api::pains()
{
    std::vector<position> positions;
    for (int x = 0; x < LARGEUR; x++)
    {
        for (int y = 0; y < HAUTEUR; y++)
        {
            for (int z = -1; z <= 0; z++)
            {
                position pos{x, y, z};
                int nb = game_state_->get_map().get_cell(pos).etat.nb_pains;
                for (int i = 0; i < nb; i++)
                    positions.push_back(pos);
            }
        }
    }
    return positions;
}
std::vector<action_hist> Api::historique()
{
    std::vector<action_hist> hist;
    std::vector<internal_action> internal_hist =
        game_state_->get_player_ptr(adversaire())->get_internal_history();

    for (auto action : internal_hist)
        if (action.type == standard_action)
            hist.push_back(action.action);

    return hist;
}

int Api::gain(int nb_pains)
{
    return ::gain(nb_pains);
}

int Api::inventaire(int taille)
{
    if (taille < 0)
        return 0;
    return taille/3;
}

std::vector<direction> Api::trouver_chemin(position depart, position arrivee)
{
    std::vector<direction> result;
    if (!inside_map(depart) || !inside_map(arrivee))
        return result;
    auto map = game_state_->get_map();
    bool visite[2][HAUTEUR][LARGEUR];
    direction back[2][HAUTEUR][LARGEUR];
    for (int niveau = -1; niveau <= 0; niveau++) {
        for (int ligne = 0; ligne < HAUTEUR; ligne++) {
            for (int colonne = 0; colonne < LARGEUR; colonne++) {
                position pos = {
                    .colonne = colonne,
                    .ligne = ligne,
                    .niveau = niveau };
                visite[niveau+1][ligne][colonne] = map.case_mortelle(pos);
            }
        }
    }
    std::queue<position> queue;
    queue.push(arrivee);
    bool found = false;
    while (!queue.empty()) {
        position point = queue.front();
        queue.pop();
        if (point == depart) {
            found = true;
            break;
        }
        visite[point.niveau+1][point.ligne][point.colonne] = true;
        std::vector<direction> nexts = map.directions_non_mortelles(point);
        for (direction nextdir : nexts) {
            position nextpos = point + get_delta_pos(nextdir);
            if (!visite[nextpos.niveau+1][nextpos.ligne][nextpos.colonne]) {
				back[nextpos.niveau+1][nextpos.ligne][nextpos.colonne] = inverse_dir(nextdir);
				visite[nextpos.niveau+1][nextpos.ligne][nextpos.colonne] = true;
            	queue.push(nextpos);
			}
        }
    }
    
    for (int y = 0; y < HAUTEUR; y++) {
		for (int x = 0; x < LARGEUR; x++) {
			if (y == arrivee.ligne && x == arrivee.colonne)
				std::cout << 'X';
			else 
			{
				direction thing = back[1][y][x];
				switch (thing) {
					case NORD:
						std::cout << 'v';
						break;
					case EST:
						std::cout << '>';
						break;
					case SUD:
						std::cout << '^';
						break;
					case OUEST:
						std::cout << '<';
						break;
				}
			}
		}
		std::cout << std::endl;
	}
	    

    if (!found)
        return result;
    position traverser = depart;
    while (traverser != arrivee) {
        direction way = back[traverser.niveau+1][traverser.ligne][traverser.colonne];
        result.push_back(way);
        traverser = traverser + get_delta_pos(way);
    }
    return result;
}

int gain(int nb_pains)
{
    return nb_pains * nb_pains;
}

int Api::moi()
{
    return player_->id;
}

int Api::adversaire()
{
    auto other_player = game_state_->get_other(player_->id);
    return other_player.get_key();
}

int Api::score(int id_joueur)
{
    auto player = game_state_->get_player_ptr(id_joueur);
    return player == nullptr ? -1 : player->get_score();
}

bool Api::annuler()
{
    return game_state_->get_player(player_->id).cancel() && cancel();
}

int Api::tour_actuel()
{
    return game_state_->get_round();
}

std::ostream& operator<<(std::ostream& os, erreur v);
void Api::afficher_erreur(erreur v)
{
    std::cerr << v << std::endl;
}

std::ostream& operator<<(std::ostream& os, direction v);
void Api::afficher_direction(direction v)
{
    std::cerr << v << std::endl;
}

std::ostream& operator<<(std::ostream& os, type_case v);
void Api::afficher_type_case(type_case v)
{
    std::cerr << v << std::endl;
}

std::ostream& operator<<(std::ostream& os, etat_barriere v);
void Api::afficher_etat_barriere(etat_barriere v)
{
    std::cerr << v << std::endl;
}

std::ostream& operator<<(std::ostream& os, etat_nid v);
void Api::afficher_etat_nid(etat_nid v)
{
    std::cerr << v << std::endl;
}

std::ostream& operator<<(std::ostream& os, pigeon_debug v);
void Api::afficher_pigeon_debug(pigeon_debug v)
{
    std::cerr << v << std::endl;
}

std::ostream& operator<<(std::ostream& os, type_action v);
void Api::afficher_type_action(type_action v)
{
    std::cerr << v << std::endl;
}

std::ostream& operator<<(std::ostream& os, position v);
void Api::afficher_position(position v)
{
    std::cerr << v << std::endl;
}

std::ostream& operator<<(std::ostream& os, troupe v);
void Api::afficher_troupe(troupe v)
{
    std::cerr << v << std::endl;
}

std::ostream& operator<<(std::ostream& os, etat_case v);
void Api::afficher_etat_case(etat_case v)
{
    std::cerr << v << std::endl;
}

std::ostream& operator<<(std::ostream& os, action_hist v);
void Api::afficher_action_hist(action_hist v)
{
    std::cerr << v << std::endl;
}
