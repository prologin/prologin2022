#include <gtest/gtest.h>
#include <utils/log.hh>

#include "../constant.hh"
#include "../position.hh"
#include "test-helpers.hh"

namespace
{
void assert_troupe_died(erreur err, GameState& game_state, PlayerInfo& player,
                        std::vector<position>& former, troupe* trp,
                        int former_inv, int line)
{
    ASSERT_EQ(OK, err) << "failed line " << line;

    for (auto& pos : former)
        ASSERT_FALSE(game_state.get_map().get_cell(pos).canard_sur_case)
            << line;

    ASSERT_EQ(0, trp->inventaire) << line;
    ASSERT_EQ(static_cast<size_t>(1), player.get_troupe(1)->canards.size())
        << line;
    ASSERT_EQ(static_cast<size_t>(TAILLE_DEPART - 1),
              player.canards_additionnels(trp->id)->size())
        << line;
}

void place_trp(troupe* trp, std::vector<position> pos, Map& map,
				PlayerInfo& player)
{
    map.delete_troupe(*trp);
    trp->canards.clear();
    trp->taille = pos.size();
    for (size_t i = 0; i < pos.size(); ++i)
        trp->canards.push_back(pos[i]);
    trp->maman = trp->canards[0];
    map.mark_troupe(*trp, player);
}

void troup_hardcoded_setup(troupe* trp, PlayerInfo& player, Map& map)
{
    map.delete_troupe(*trp);
    trp->canards.reserve(5);
    trp->canards[0] = {.colonne = 27, .ligne = 38, .niveau = 0};
    trp->canards[1] = {.colonne = 26, .ligne = 38, .niveau = 0};
    trp->canards[2] = {.colonne = 25, .ligne = 38, .niveau = 0};
    trp->canards[3] = {.colonne = 24, .ligne = 38, .niveau = 0};
    trp->canards[4] = {.colonne = 23, .ligne = 38, .niveau = 0};
    trp->maman = trp->canards[0];
    map.mark_troupe(*trp, player);
}

} // namespace

TEST_F(ApiTest, BarriereSplitLaTroupeSansPains)
{
	auto& player = players[0];
    std::vector<position> new_pos = {
        {.colonne = 15, .ligne = 24, .niveau = 0},
        {.colonne = 15, .ligne = 23, .niveau = 0},
        {.colonne = 15, .ligne = 22, .niveau = 0},
        {.colonne = 15, .ligne = 21, .niveau = 0},
		{.colonne = 15, .ligne = 20, .niveau = 0},
        {.colonne = 15, .ligne = 19, .niveau = 0},
    };

    troupe* trp = player.info->get_troupe(1);
	place_trp(trp, new_pos, player.api->game_state().get_map(), *player.info);
	INFO("%d", trp->taille);
	auto& map = player.api->game_state().get_map();
	map.changer_barrieres();

	EXPECT_EQ(4, trp->taille);
	EXPECT_EQ(4, trp->canards.size());

	for (auto& pos : new_pos)
		ASSERT_EQ(0, map.get_cell(pos).etat.nb_pains);	
}

TEST_F(ApiTest, BarriereSplitLaTroupeAvecPains)
{
	auto& player = players[0];
    std::vector<position> new_pos = {
        {.colonne = 15, .ligne = 24, .niveau = 0},
        {.colonne = 15, .ligne = 23, .niveau = 0},
        {.colonne = 15, .ligne = 22, .niveau = 0},
        {.colonne = 15, .ligne = 21, .niveau = 0},
		{.colonne = 15, .ligne = 20, .niveau = 0},
        {.colonne = 15, .ligne = 19, .niveau = 0},
    };

    troupe* trp = player.info->get_troupe(1);
	place_trp(trp, new_pos, player.api->game_state().get_map(), *player.info);
	INFO("%d", trp->taille);
	auto& map = player.api->game_state().get_map();
	trp->inventaire = 2;
	map.changer_barrieres();

	EXPECT_EQ(4, trp->taille);
	EXPECT_EQ(4, trp->canards.size());
	EXPECT_EQ(1, trp->inventaire);

	ASSERT_EQ(1, map.get_cell(new_pos[5]).etat.nb_pains);	
	for (int i = 0; i < 5; ++i)
		ASSERT_EQ(0, map.get_cell(new_pos[i]).etat.nb_pains);
}

TEST_F(ApiTest, BarriereSplitLaTroupeMeurt)
{
	auto& player = players[0];
    std::vector<position> new_pos = {
		{.colonne = 15, .ligne = 20, .niveau = 0},
        {.colonne = 15, .ligne = 21, .niveau = 0},
        {.colonne = 15, .ligne = 22, .niveau = 0},
        {.colonne = 15, .ligne = 23, .niveau = 0},
        {.colonne = 15, .ligne = 24, .niveau = 0},
    };

    troupe* trp = player.info->get_troupe(1);
	place_trp(trp, new_pos, player.api->game_state().get_map(), *player.info);
	auto& map = player.api->game_state().get_map();
	trp->inventaire = 1;
	map.changer_barrieres();

	assert_troupe_died(OK, player.api->game_state(), *player.info, new_pos,
					trp, 2, __LINE__);
	for (int i = 0; i < trp->taille; ++i)
		if (i != 2)
			ASSERT_EQ(0, map.get_cell(trp->canards[i]).etat.nb_pains);	

	ASSERT_EQ(1, map.get_cell(trp->canards[2]).etat.nb_pains);
}

TEST_F(ApiTest, BarriereSplitLaTroupeMeurtFull)
{
	auto& player = players[0];
    std::vector<position> new_pos = {
		{.colonne = 15, .ligne = 20, .niveau = 0},
        {.colonne = 15, .ligne = 21, .niveau = 0},
        {.colonne = 15, .ligne = 22, .niveau = 0},
        {.colonne = 15, .ligne = 23, .niveau = 0},
        {.colonne = 15, .ligne = 24, .niveau = 0},
        {.colonne = 15, .ligne = 25, .niveau = 0},
    };

    troupe* trp = player.info->get_troupe(1);
	place_trp(trp, new_pos, player.api->game_state().get_map(), *player.info);
	auto& map = player.api->game_state().get_map();
	trp->inventaire = 2;
	map.changer_barrieres();

	assert_troupe_died(OK, player.api->game_state(), *player.info, new_pos,
					trp, 2, __LINE__);
	for (int i = 0; i < trp->taille; ++i)
		if (i != 2 && i != 5)
			ASSERT_EQ(0, map.get_cell(trp->canards[i]).etat.nb_pains);	

	ASSERT_EQ(1, map.get_cell(trp->canards[2]).etat.nb_pains);
	ASSERT_EQ(1, map.get_cell(trp->canards[5]).etat.nb_pains);
}

TEST_F(ApiTest, BarriereSplitLaTroupeMeurtSoftFull)
{
	auto& player = players[0];
    std::vector<position> new_pos = {
		{.colonne = 14, .ligne = 19, .niveau = 0},
        {.colonne = 15, .ligne = 19, .niveau = 0},
        {.colonne = 15, .ligne = 20, .niveau = 0},
        {.colonne = 15, .ligne = 21, .niveau = 0},
        {.colonne = 15, .ligne = 22, .niveau = 0},
        {.colonne = 15, .ligne = 23, .niveau = 0},
    };

    troupe* trp = player.info->get_troupe(1);
	place_trp(trp, new_pos, player.api->game_state().get_map(), *player.info);
	auto& map = player.api->game_state().get_map();
	trp->inventaire = 2;
	map.changer_barrieres();

	assert_troupe_died(OK, player.api->game_state(), *player.info, new_pos,
					trp, 2, __LINE__);
	for (int i = 0; i < trp->taille; ++i)
		if (i != 2 && i != 5)
			ASSERT_EQ(0, map.get_cell(trp->canards[i]).etat.nb_pains);	

	ASSERT_EQ(1, map.get_cell(trp->canards[2]).etat.nb_pains);
	ASSERT_EQ(1, map.get_cell(trp->canards[5]).etat.nb_pains);
}

TEST_F(ApiTest, BarriereSplitLaTroupeMeurtSoft)
{
	auto& player = players[0];
    std::vector<position> new_pos = {
		{.colonne = 14, .ligne = 19, .niveau = 0},
        {.colonne = 15, .ligne = 19, .niveau = 0},
        {.colonne = 15, .ligne = 20, .niveau = 0},
        {.colonne = 15, .ligne = 21, .niveau = 0},
        {.colonne = 15, .ligne = 22, .niveau = 0},
        {.colonne = 15, .ligne = 23, .niveau = 0},
    };

    troupe* trp = player.info->get_troupe(1);
	place_trp(trp, new_pos, player.api->game_state().get_map(), *player.info);
	auto& map = player.api->game_state().get_map();
	trp->inventaire = 1;
	map.changer_barrieres();

	assert_troupe_died(OK, player.api->game_state(), *player.info, new_pos,
					trp, 2, __LINE__);
	for (int i = 0; i < trp->taille; ++i)
		if (i != 2 )
			ASSERT_EQ(0, map.get_cell(trp->canards[i]).etat.nb_pains);	

	ASSERT_EQ(1, map.get_cell(trp->canards[2]).etat.nb_pains);
}
