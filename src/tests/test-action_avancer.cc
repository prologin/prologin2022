#include <utils/log.hh>

#include "../constant.hh"
#include "../position.hh"
#include "test-helpers.hh"

namespace
{
        /*
    void assert_troupe_moved(troupe& trp_after, troupe& trp_before,
                    direction dir, int line)
    {
        auto delta = get_delta_pos(dir);  
        ASSERT_EQ(trp_before.maman + delta, trp_after.maman)
                     << "First Duck did not move accordingly line: " << line;

        for (int i = trp_after.taille - 1; i > 0; --i)
        {
            ASSERT_EQ(trp_before.canards[i - 1], trp_after.canards[i])
                << "Duck number " << i 
                << ", did not move accordingly, line: " << line;    
        }
    } 

    erreur test_move_troupe(std::shared_ptr<PlayerInfo>player, 
                    std::unique_ptr<Api>& api, 
                    int id, direction dir, bool moved, int line)
    {
        troupe before = *(player->get_troupe(id));

        INFO("Apres maman: %d, %d, %d", before.maman.colonne,
                    before.maman.ligne,
                    before.maman.niveau);

        auto err = api->avancer(id, dir); 
        
        troupe after = *(player->get_troupe(id));
        
        INFO("Apres maman: %d, %d, %d", after.maman.colonne,
                    after.maman.ligne,
                    after.maman.niveau);

        
        for (int i = 0; i < before.taille; i++)
        {
            if(&(before.canards[i]) == &(after.canards[i]))
                FATAL("chiant %lu", &(before.canards[i]));
        }

        //ASSERT_EQ(err == OK, moved) << "failed line: " << line;
        if (moved)
            assert_troupe_moved(after, before, dir, line);
        return err;
    }
    */
}

TEST_F(ApiTest, ActionAvancerAutorisee)
{
    auto& player = players[0];
    //auto err = test_move_troupe(player.info, player.api, 1, EST, true, __LINE__);
    auto err = player.api->avancer(1, EST);
    ASSERT_EQ(OK, err);
}

TEST_F(ApiTest, ActionAvancerInvalidTroupe)
{
    auto& player = players[0];
    direction dir = EST;
    auto err = player.api->avancer(NB_TROUPES + 42, dir);

    ASSERT_EQ(TROUPE_INVALIDE, err);
}

