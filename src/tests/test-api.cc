#include "test-helpers.hh"
#include "../constant.hh"

namespace
{
    const etat_case etat_case_err = {
        .pos = {-1, -1, -1},
        .contenu = static_cast<type_case>(-1),
        .est_constructible = false,
        .nb_pains = false
    };

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
        { 1, 1, 0 },   // ' '
        { 5, 2, 0 },   // '.'
        { 36, 0, 0 },  // 'S'
        { 57, 11, 0 }, // 'N'
        { 0, 0, 0 },   // '#'
        { 30, 28, 0 }, // 'B'
        { 43, 36, 0 }, // 'b'
        { 32, 66, 0 }, // 'X'
        { 65, 7, 0}    // 0-9
    };

    const size_t n_positions = sizeof(positions) / sizeof(*positions);

    const etat_case etat_cases[] = {
         { positions[0], GAZON, false, false },    // ' '
         { positions[1], GAZON, true, false },     // '.'
         { positions[2], GAZON, false, false },    // 'S'
         { positions[3], NID, false, false },      // 'N'
         { positions[4], BUISSON, false, false },  // '#'
         { positions[5], BARRIERE, false, false }, // 'B'
         { positions[6], BARRIERE, false, false }, // 'b'
         { positions[7], TROU, false, false },     // 'X'
         { positions[8], PAPY, false, false }      // 0-9
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
        { 1, 1, 0 },     // ' '
        { 5, 2, 0 },     // '.'
        { 36, 0, 0 },    // 'S'
        { 57, 11, 0 },   // 'N'
        { 0, 0, 0 },     // '#'
        { 32, 66, 0 },   // 'X'
        { 65, 7, 0}      // 0-9
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
        { 30, 28, 0 }, // 'B'
        { 30, 29, 0 }, // 'B'
        { 30, 30, 0 }, // 'B'
        { 30, 31, 0 }, // 'B'
        { 30, 32, 0 }, // 'B'
        { 30, 33, 0 }, // 'B'
        { 30, 34, 0 }, // 'B'
        { 30, 35, 0 }, // 'B'
        { 30, 36, 0 }, // 'B'
    };

    const position positions_barriere_ouverte[] = {
        { 43, 28, 0 }, // 'b'
        { 43, 29, 0 }, // 'b'
        { 43, 30, 0 }, // 'b'
        { 43, 31, 0 }, // 'b'
        { 43, 32, 0 }, // 'b'
        { 43, 33, 0 }, // 'b'
        { 43, 34, 0 }, // 'b'
        { 43, 35, 0 }, // 'b'
        { 43, 36, 0 }, // 'b'
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
        { .colonne = 0, .ligne = HAUTEUR, .niveau = 0 }, // invalid pos
        { .colonne = LARGEUR, .ligne = 0, .niveau = 0 }, // invalid pos
        { .colonne = 0,  .ligne = 0,  .niveau = 1 },  // invalid pos
        { .colonne = -1, .ligne = 0,  .niveau = 0 },  // invalid pos
        { .colonne = 0,  .ligne = -1, .niveau = 0 },  // invalid pos
        { .colonne = 0,  .ligne = 0,  .niveau = -2 }, // invalid pos

        { .colonne = 1,  .ligne = 1,  .niveau = 0 },  // ' '
        { .colonne = 5,  .ligne = 2,  .niveau = 0 },  // '.'
        { .colonne = 36, .ligne = 0,  .niveau = 0 },  // 'S'
        { .colonne = 0,  .ligne = 0,  .niveau = 0 },  // '#'
        { .colonne = 30, .ligne = 28, .niveau = 0 },  // 'B'
        { .colonne = 43, .ligne = 36, .niveau = 0 },  // 'b'
        { .colonne = 32, .ligne = 66, .niveau = 0 },  // 'X'
        { .colonne = 65, .ligne = 7,  .niveau = 0 }   // 0-9
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
        { .colonne = 36, .ligne = 1,  .niveau = 0 }, // 'N'
        { .colonne = 57, .ligne = 11, .niveau = 0 }, // 'N'
        { .colonne = 19, .ligne = 31, .niveau = 0 }, // 'N'
        { .colonne = 66, .ligne = 34, .niveau = 0 }, // 'N'
        { .colonne = 41, .ligne = 66, .niveau = 0 }, // 'N'
        { .colonne = 49, .ligne = 78, .niveau = 0 }, // 'N'
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
        { .colonne = 0, .ligne = HAUTEUR, .niveau = 0 }, // invalid pos
        { .colonne = LARGEUR, .ligne = 0, .niveau = 0 }, // invalid pos
        { .colonne = 0,  .ligne = 0,  .niveau = 1 },  // invalid pos
        { .colonne = -1, .ligne = 0,  .niveau = 0 },  // invalid pos
        { .colonne = 0,  .ligne = -1, .niveau = 0 },  // invalid pos
        { .colonne = 0,  .ligne = 0,  .niveau = -2 }, // invalid pos

        { .colonne = 1,  .ligne = 1,  .niveau = 0 }, // ' '
        { .colonne = 5,  .ligne = 2,  .niveau = 0 }, // '.'
        { .colonne = 36, .ligne = 0,  .niveau = 0 }, // 'S'
        { .colonne = 57, .ligne = 11, .niveau = 0 }, // 'N'
        { .colonne = 0,  .ligne = 0,  .niveau = 0 }, // '#'
        { .colonne = 30, .ligne = 28, .niveau = 0 }, // 'B'
        { .colonne = 43, .ligne = 36, .niveau = 0 }, // 'b'
        { .colonne = 32, .ligne = 66, .niveau = 0 }, // 'X'
    };

    for (const auto& player : players)
    {
        for (const auto& pos_invalide : pos_invalides)
        {
            auto papy_tours_restants = player.api->papy_tours_restants(pos_invalide);
            ASSERT_EQ(-1, papy_tours_restants);
        }
    }
}

TEST_F(ApiTest, ApiPapyToursRestants_PosOk)
{
    const position pos_papys[] = {
        { .colonne = 17, .ligne = 5,  .niveau = 0 }, // 1
        { .colonne = 65, .ligne = 7,  .niveau = 0 }, // 2
        { .colonne = 12, .ligne = 25, .niveau = 0 }, // 0
        { .colonne = 56, .ligne = 29, .niveau = 0 }, // 3
        { .colonne = 20, .ligne = 66, .niveau = 0 }, // 0
    };

    const int pos_papy_tours_restants[] = { 1, 2, 0, 3, 0 };

    const size_t n_papys = sizeof(pos_papys) / sizeof(*pos_papys);

    for (const auto& player : players)
    {
        for (size_t i = 0; i < n_papys; ++i)
        {
            auto pos_papy = pos_papys[i];
            auto expect_papy_tours_restants = pos_papy_tours_restants[i];

            auto papy_tours_restants = player.api->papy_tours_restants(pos_papy);
            ASSERT_EQ(expect_papy_tours_restants, papy_tours_restants);
        }
    }
}

TEST_F(ApiTest, ApiGain)
{
    const int nb_pains_array[] = { 1, 3, 5, 8, 13, 29, 37 };

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

TEST_F(ApiTest, ApiMoi)
{
    ASSERT_EQ(2, players.size());
    ASSERT_EQ(PLAYER_ID_1, players[0].api->moi());
    ASSERT_EQ(PLAYER_ID_2, players[1].api->moi());
}

TEST_F(ApiTest, ApiAdversaire)
{
    ASSERT_EQ(2, players.size());
    ASSERT_EQ(PLAYER_ID_2, players[0].api->adversaire());
    ASSERT_EQ(PLAYER_ID_1, players[1].api->adversaire());
}

TEST_F(ApiTest, ApiScore_IdJoueurInvalide)
{
    const int id_players_invalides[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41,
        43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97
    };

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
        const position position = { 0, 0, 0 };

        auto pigeon_before = player.api->game_state().get_map().get_cell(position).pigeon;
        auto pigeon_after = PIGEON_BLEU;

        auto err = player.api->debug_poser_pigeon(position, pigeon_after);
        ASSERT_EQ(OK, err);
        ASSERT_EQ(pigeon_after, player.api->game_state().get_map().get_cell(position).pigeon);

        auto is_cancel = player.api->annuler();
        ASSERT_EQ(true, is_cancel);
        ASSERT_EQ(pigeon_before, player.api->game_state().get_map().get_cell(position).pigeon);
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

TEST_F(ApiTest, ApiPains)
{
    const position positions[] = {
        { 1, 1, 0 },   // ' '
        { 5, 2, 0 },   // '.'
        { 36, 0, 0 },  // 'S'
        { 30, 28, 0 }, // 'B'
        { 43, 36, 0 }, // 'b'
        { 32, 66, 0 }, // 'X'
        { 65, 7, 0}    // 0-9
    };

    for (const auto& player : players)
        ASSERT_EQ(0, player.api->pains().size());

    int expected = 0;
    for (const auto& position : positions) 
    {
        int count = 0;
        for (const auto& player : players) 
        {
            player.api->game_state().get_map().get_cell(position).etat.nb_pains ++;
            expected++;
            count++;
            ASSERT_EQ(expected, player.api->pains().size());
            int got = 0;
            for (const auto& pain_pos : player.api->pains()) 
            {
                if (pain_pos.colonne == position.colonne
                        && pain_pos.ligne == position.ligne
                        && pain_pos.niveau == position.niveau)
                    got++;
            }
            ASSERT_EQ(count, got);
        }
    }
}
