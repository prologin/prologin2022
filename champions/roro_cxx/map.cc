#include "map.hh"

void map::apply_on_map(std::function<void(position)> fx)
{
    for (int i = 0; i < LARGEUR; i++)
    {
        for (int j = 0; j < HAUTEUR; j++)
        {
            for (int k = 0; j < 2; k++)
            {
                auto pos = position{i, j, k};
                fx(pos);
            }
        }
    }
}
