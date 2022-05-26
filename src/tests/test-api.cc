#include "../constant.hh"
#include "test-helpers.hh"

namespace
{
const etat_case etat_case_err = {.pos = {-1, -1, -1},
                                 .contenu = static_cast<type_case>(-1),
                                 .est_constructible = false,
                                 .nb_pains = -1};

void assert_eq_etat_case(etat_case etat_case_1, etat_case etat_case_2)
{
    ASSERT_EQ(etat_case_1.pos.colonne, etat_case_2.pos.colonne);
    ASSERT_EQ(etat_case_1.pos.ligne, etat_case_2.pos.ligne);
    ASSERT_EQ(etat_case_1.pos.niveau, etat_case_2.pos.niveau);
    ASSERT_EQ(etat_case_1.contenu, etat_case_2.contenu);
    ASSERT_EQ(etat_case_1.est_constructible, etat_case_2.est_constructible);
    ASSERT_EQ(etat_case_1.nb_pains, etat_case_2.nb_pains);
}
} // namespace

TEST_F(ApiTest, ApiInfoCase_EtatCaseInvalide)
{
    const position positions[] = {
        {0, HAUTEUR, 0}, {LARGEUR, 0, 0}, {0, 0, 1},
        {-1, 0, 0},      {0, -1, 0},      {0, 0, -2},
    };

    for (const auto& player : players)
    {
        for (const auto& position : positions)
        {
            auto etat_case = player.api->info_case(position);
            assert_eq_etat_case(etat_case, etat_case_err);
        }
    }
}

TEST_F(ApiTest, ApiInfoCase_EtatCaseOk)
{
    const position positions[] = {
        {2, 2, 0},   // ' '
        {1, 1, 0},   // '.'
        {14, 0, 0},  // 'S'
        {13, 2, 0},  // 'N'
        {0, 0, 0},   // '#'
        {29, 25, 0}, // 'B'
        {26, 26, 0}, // 'b'
        {16, 1, 0},  // 'X'
        {7, 2, 0}    // 0-9
    };

    const size_t n_positions = sizeof(positions) / sizeof(*positions);

    const etat_case etat_cases[] = {
        {positions[0], GAZON, false, false},    // ' '
        {positions[1], GAZON, true, false},     // '.'
        {positions[2], GAZON, false, false},    // 'S'
        {positions[3], NID, false, false},      // 'N'
        {positions[4], BUISSON, false, false},  // '#'
        {positions[5], BARRIERE, false, false}, // 'B'
        {positions[6], BARRIERE, false, false}, // 'b'
        {positions[7], TROU, false, false},     // 'X'
        {positions[8], PAPY, false, false}      // 0-9
    };

    for (const auto& player : players)
    {
        for (size_t i = 0; i < n_positions; i++)
        {
            auto etat_case = player.api->info_case(positions[i]);
            assert_eq_etat_case(etat_case, etat_cases[i]);
        }
    }
}

TEST_F(ApiTest, ApiInfoBarriere_PosInvalide)
{
    const position positions[] = {
        {0, HAUTEUR, 0}, // invalid pos
        {LARGEUR, 0, 0}, // invalid pos
        {0, 0, 1},       // invalid pos
        {-1, 0, 0},      // invalid pos
        {0, -1, 0},      // invalid pos
        {0, 0, -2},      // invalid pos
        {2, 2, 0},       // ' '
        {1, 1, 0},       // '.'
        {14, 0, 0},      // 'S'
        {13, 2, 0},      // 'N'
        {0, 0, 0},       // '#'
        {16, 1, 0},      // 'X'
        {7, 2, 0}        // 0-9
    };

    for (const auto& player : players)
    {
        for (const auto& position : positions)
        {
            auto etat_barriere = player.api->info_barriere(position);
            ASSERT_EQ(PAS_DE_BARRIERE, etat_barriere);
        }
    }
}

TEST_F(ApiTest, ApiInfoBarriere_PosOk)
{
    const position positions_barriere_fermee[] = {
        {29, 25, 0}, // 'B'
        {31, 6, 0},  // 'B'
        {4, 8, 0},   // 'B'
        {33, 9, 0},  // 'B'
        {32, 18, 0}, // 'B'
        {4, 21, 0},  // 'B'
        {29, 25, 0}, // 'B'
        {10, 29, 0}, // 'B'
        {38, 29, 0}, // 'B'
    };

    const position positions_barriere_ouverte[] = {
        {31, 3, 0},  // 'b'
        {13, 6, 0},  // 'b'
        {35, 15, 0}, // 'b'
        {27, 18, 0}, // 'b'
        {15, 20, 0}, // 'b'
        {13, 26, 0}, // 'b'
        {26, 26, 0}, // 'b'
        {2, 29, 0},  // 'b'
        {1, 35, 0},  // 'b'
    };

    for (const auto& player : players)
    {
        for (const auto& position : positions_barriere_fermee)
        {
            auto etat_barriere = player.api->info_barriere(position);
            ASSERT_EQ(FERMEE, etat_barriere);
        }

        for (const auto& position : positions_barriere_ouverte)
        {
            auto etat_barriere = player.api->info_barriere(position);
            ASSERT_EQ(OUVERTE, etat_barriere);
        }
    }
}

TEST_F(ApiTest, ApiInfoNid_PosInvalide)
{
    const position positions[] = {
        {.colonne = 0, .ligne = HAUTEUR, .niveau = 0}, // invalid pos
        {.colonne = LARGEUR, .ligne = 0, .niveau = 0}, // invalid pos
        {.colonne = 0, .ligne = 0, .niveau = 1},       // invalid pos
        {.colonne = -1, .ligne = 0, .niveau = 0},      // invalid pos
        {.colonne = 0, .ligne = -1, .niveau = 0},      // invalid pos
        {.colonne = 0, .ligne = 0, .niveau = -2},      // invalid pos

        {.colonne = 2, .ligne = 2, .niveau = 0},   // ' '
        {.colonne = 1, .ligne = 1, .niveau = 0},   // '.'
        {.colonne = 14, .ligne = 0, .niveau = 0},  // 'S'
        {.colonne = 0, .ligne = 0, .niveau = 0},   // '#'
        {.colonne = 29, .ligne = 25, .niveau = 0}, // 'B'
        {.colonne = 26, .ligne = 26, .niveau = 0}, // 'b'
        {.colonne = 16, .ligne = 1, .niveau = 0},  // 'X'
        {.colonne = 7, .ligne = 2, .niveau = 0}    // 0-9
    };

    for (const auto& player : players)
    {
        for (const auto& position : positions)
        {
            auto etat_nid = player.api->info_nid(position);
            ASSERT_EQ(PAS_DE_NID, etat_nid);
        }
    }
}

TEST_F(ApiTest, ApiInfoNid_PosOk)
{
    const position pos_nids_libres[] = {
        {.colonne = 13, .ligne = 2, .niveau = 0},  // 'N'
        {.colonne = 14, .ligne = 1, .niveau = 0},  // 'N'
        {.colonne = 33, .ligne = 1, .niveau = 0},  // 'N'
        {.colonne = 11, .ligne = 11, .niveau = 0}, // 'N'
        {.colonne = 12, .ligne = 16, .niveau = 0}, // 'N'
        {.colonne = 1, .ligne = 27, .niveau = 0},  // 'N'
    };

    for (const auto& player : players)
    {
        for (const auto& position : pos_nids_libres)
        {
            auto etat_nid = player.api->info_nid(position);
            ASSERT_EQ(LIBRE, etat_nid);
        }
    }
}

TEST_F(ApiTest, ApiPapyToursRestants_PosInvalide)
{
    const position pos_invalides[] = {
        {.colonne = 0, .ligne = HAUTEUR, .niveau = 0}, // invalid pos
        {.colonne = LARGEUR, .ligne = 0, .niveau = 0}, // invalid pos
        {.colonne = 0, .ligne = 0, .niveau = 1},       // invalid pos
        {.colonne = -1, .ligne = 0, .niveau = 0},      // invalid pos
        {.colonne = 0, .ligne = -1, .niveau = 0},      // invalid pos
        {.colonne = 0, .ligne = 0, .niveau = -2},      // invalid pos

        {.colonne = 2, .ligne = 2, .niveau = 0},   // ' '
        {.colonne = 1, .ligne = 1, .niveau = 0},   // '.'
        {.colonne = 14, .ligne = 0, .niveau = 0},  // 'S'
        {.colonne = 13, .ligne = 2, .niveau = 0},  // 'N'
        {.colonne = 0, .ligne = 0, .niveau = 0},   // '#'
        {.colonne = 29, .ligne = 25, .niveau = 0}, // 'B'
        {.colonne = 26, .ligne = 26, .niveau = 0}, // 'b'
        {.colonne = 16, .ligne = 1, .niveau = 0},  // 'X'
    };

    for (const auto& player : players)
    {
        for (const auto& pos_invalide : pos_invalides)
        {
            auto papy_tours_restants =
                player.api->papy_tours_restants(pos_invalide);
            ASSERT_EQ(-1, papy_tours_restants);
        }
    }
}

TEST_F(ApiTest, ApiPapyToursRestants_PosOk)
{
    const position pos_papys[] = {
        {.colonne = 7, .ligne = 2, .niveau = 0},   // 2
        {.colonne = 19, .ligne = 2, .niveau = 0},  // 4
        {.colonne = 20, .ligne = 11, .niveau = 0}, // 1
        {.colonne = 11, .ligne = 15, .niveau = 0}, // 4
        {.colonne = 27, .ligne = 15, .niveau = 0}, // 0
    };

    const int pos_papy_tours_restants[] = {2, 4, 1, 4, 0};

    const size_t n_papys = sizeof(pos_papys) / sizeof(*pos_papys);

    for (const auto& player : players)
    {
        for (size_t i = 0; i < n_papys; ++i)
        {
            auto pos_papy = pos_papys[i];
            auto expect_papy_tours_restants = pos_papy_tours_restants[i];

            auto papy_tours_restants =
                player.api->papy_tours_restants(pos_papy);
            ASSERT_EQ(expect_papy_tours_restants, papy_tours_restants);
        }
    }
}

TEST_F(ApiTest, ApiGain)
{
    const int nb_pains_array[] = {1, 3, 5, 8, 13, 29, 37};

    for (const auto& player : players)
    {
        int previous_ratio = 0;

        for (const auto& nb_pains : nb_pains_array)
        {
            int new_ratio = player.api->gain(nb_pains) / nb_pains;
            ASSERT_TRUE(new_ratio > previous_ratio);
            previous_ratio = new_ratio;
        }
    }
}

TEST_F(ApiTest, ApiInventaire)
{
    const int taille_troupe_array[] = {-10, -1, 0, 1, 5, 8, 11, 14, 18, 20};

    for (const auto& player : players)
    {
        int previous = 0;

        for (const auto& taille : taille_troupe_array)
        {
            int now = player.api->inventaire(taille);
            ASSERT_TRUE(now >= previous);
            ASSERT_TRUE(now >= 0);
            previous = now;
        }
    }
}

TEST_F(ApiTest, ApiTrouverChemin)
{
	
	position todig[] = {
		{.colonne = 19, .ligne = 6, .niveau = -1},
		{.colonne = 20, .ligne = 6, .niveau = -1},
		{.colonne = 20, .ligne = 7, .niveau = -1},
		{.colonne = 20, .ligne = 8, .niveau = -1},
		{.colonne = 20, .ligne = 9, .niveau = -1}
		};
	for (position dig : todig) {
		for (const auto& player : players) {
			player.api->game_state().get_map().get_cell(dig).etat.contenu = TUNNEL;
		} 
	}

	std::pair<position, position> tests[] = {
		{ {.colonne = 23, .ligne = 16, .niveau = 0},
		  {.colonne = 28, .ligne = 14, .niveau = 0} },
		{ {.colonne = 12, .ligne = 23, .niveau = 0},
		  {.colonne = 13, .ligne = 21, .niveau = 0} },
		{ {.colonne = 5 , .ligne = 1 , .niveau = 0},
		  {.colonne = 11, .ligne = 1 , .niveau = 0} },
		{ {.colonne = 26, .ligne = 39, .niveau = 0},
		  {.colonne = 26, .ligne = 40, .niveau = 0} },
		{ {.colonne = 18, .ligne = 5 , .niveau = 0},
		  {.colonne = 21, .ligne = 6 , .niveau = 0} }
	};

	std::vector<direction> expectations[] = {	
		{ NORD, NORD, NORD, NORD, EST, EST, SUD, EST, EST, SUD, SUD, SUD, SUD, SUD, EST },
		{ OUEST, SUD, SUD, EST, EST},
		{},
		{},
		{ NORD, EST, BAS, EST, NORD, NORD, NORD, HAUT, EST, SUD, EST, EST, SUD, SUD, OUEST, OUEST}
	};
    for (const auto& player : players)
    {
		for (int t = 0; t < 5; t++) {
			std::vector<direction> got = player.api->trouver_chemin(tests[t].first, tests[t].second);
			std::vector<direction> expected = expectations[t];
        	ASSERT_EQ(got.size(), expected.size());
        	for (int i = 0; i < got.size(); i++) {
            	ASSERT_EQ(got[i], expected[i]);
        	}
		}
    }
}

TEST_F(ApiTest, ApiMoi)
{
    ASSERT_EQ(static_cast<size_t>(2), players.size());
    ASSERT_EQ(PLAYER_ID_1, players[0].api->moi());
    ASSERT_EQ(PLAYER_ID_2, players[1].api->moi());
}

TEST_F(ApiTest, ApiAdversaire)
{
    ASSERT_EQ(static_cast<size_t>(2), players.size());
    ASSERT_EQ(PLAYER_ID_2, players[0].api->adversaire());
    ASSERT_EQ(PLAYER_ID_1, players[1].api->adversaire());
}

TEST_F(ApiTest, ApiScore_IdJoueurInvalide)
{
    const int id_players_invalides[] = {2,  3,  5,  7,  11, 13, 17, 19, 23,
                                        29, 31, 37, 41, 43, 47, 53, 59, 61,
                                        67, 71, 73, 79, 83, 89, 97};

    for (const auto& player : players)
    {
        for (const auto& id_joueur_invalide : id_players_invalides)
        {
            ASSERT_EQ(-1, player.api->score(id_joueur_invalide));
        }
    }
}

TEST_F(ApiTest, ApiScore_IdJoueurOk)
{
    for (const auto& player : players)
    {
        for (const auto& other_player : players)
        {
            ASSERT_EQ(0, player.api->score(other_player.id));
        }
    }
}

TEST_F(ApiTest, ApiAnnuler_CanNotCancel)
{
    for (const auto& player : players)
    {
        auto is_cancel = player.api->annuler();
        ASSERT_EQ(false, is_cancel);
    }
}

TEST_F(ApiTest, ApiAnnuler_Cancel)
{
    for (const auto& player : players)
    {
        const position position = {0, 0, 0};

        player.api->debug_poser_pigeon(position, PAS_DE_PIGEON);

        auto pigeon_before = player.api->game_state()
                                 .get_player(player.id)
                                 .get_internal_history()
                                 .back()
                                 .flag.ctype;
        auto pigeon_after = PIGEON_BLEU;

        auto err = player.api->debug_poser_pigeon(position, pigeon_after);
        ASSERT_EQ(OK, err);
        ASSERT_EQ(pigeon_after, player.api->game_state()
                                    .get_player(player.id)
                                    .get_internal_history()
                                    .back()
                                    .flag.ctype);
        auto is_cancel = player.api->annuler();
        ASSERT_TRUE(is_cancel);
        EXPECT_EQ(pigeon_before, player.api->game_state()
                                     .get_player(player.id)
                                     .get_internal_history()
                                     .back()
                                     .flag.ctype);
    }
}

TEST_F(ApiTest, ApiTourActuel)
{
    for (const auto& player : players)
    {
        ASSERT_EQ(0, player.api->tour_actuel());
        player.api->game_state().next_round();
        ASSERT_EQ(1, player.api->tour_actuel());
    }
}

TEST_F(ApiTest, ApiTroupesJoueursInvalide)
{
    const int id_players_invalides[] = {2,  3,  5,  7,  11, 13, 17, 19, 23,
                                        29, 31, 37, 41, 43, 47, 53, 59, 61,
                                        67, 71, 73, 79, 83, 89, 97};

    for (const auto& player : players)
    {
        for (const auto& id_joueur_invalide : id_players_invalides)
        {
            ASSERT_EQ(
                -1, player.api->troupes_joueur(id_joueur_invalide).front().id);
        }
    }
}

TEST_F(ApiTest, ApiPains)
{
    const position positions[] = {
        {2, 2, 0},   // ' '
        {1, 1, 0},   // '.'
        {14, 0, 0},  // 'S'
        {29, 25, 0}, // 'B'
        {26, 26, 0}, // 'b'
        {16, 1, 0},  // 'X'
        {7, 2, 0}    // 0-9
    };

    for (const auto& player : players)
        ASSERT_EQ(static_cast<size_t>(0), player.api->pains().size());

    size_t expected = 0; // Nombre total de pains sur la map
    for (const auto& position : positions)
    {
        int count = 0; // Nombre total de pains sur cette position
        for (const auto& player : players)
        {
            // Rajoute 1 pain sur la case, pour les deux joueurs
            // Je sais pas si c'est la bonne méthode pour que le pain soit
            // visible par les deux joueurs mais ça marche :)
            for (const auto& p : players)
                p.api->game_state()
                    .get_map()
                    .get_cell(position)
                    .etat.nb_pains++;

            expected++;
            count++;

            int got = 0; // Nombre de pains détectés sur cette position
            for (const auto& pain_pos : player.api->pains())
            {
                if (pain_pos.colonne == position.colonne &&
                    pain_pos.ligne == position.ligne &&
                    pain_pos.niveau == position.niveau)
                    got++;
            }
            ASSERT_EQ(count, got);
            ASSERT_EQ(expected, player.api->pains().size());
        }
    }
}
