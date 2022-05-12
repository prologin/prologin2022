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