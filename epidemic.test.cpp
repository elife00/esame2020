//
//  epidemic.test.cpp
//  
//
//  Created by Michele Mattei on 04/07/2020.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "epidemic.hpp"

TEST_CASE("testing epidemic")
{
    int dim = 100;
    double ratInf = 0.1;
    int tMean = 10;
    double density = 0.20;
    int S = (int) dim * dim * density;
    int I = (int) S * ratInf;

    Board population(dim, density);
    
    Situation So = {0, S , 0, 0};
    
    CHECK( population.current_situation() == So );
    
    population.infection(ratInf);
    
    Situation Suno = {0, S-I , I, 0};
    
    CHECK( population.current_situation() == Suno );
    
    while (population.current_situation().i != 0)
    {
        population = population.epidemic(ratInf, tMean);
        CHECK ( (population.current_situation().i + population.current_situation().s + population.current_situation().r) == S);
    }
    
}
