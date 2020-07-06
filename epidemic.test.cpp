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
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> real_dis(0., 1.0);
    std::uniform_int_distribution<> int_dis(1, 40);
    
    int dim = 100;
    double density = real_dis(gen);
    double ratInf = real_dis(gen);
    int tMean = int_dis(gen);
    int S = (int) dim * dim * density;
    int I = (int) S * ratInf;

    Board population(dim, density);
    
    Situation So = {0, S , 0, 0};
    
    CHECK( population.current_situation() == So );
    
    population.infection(ratInf);
    
    Situation Suno = {0, S-I , I, 0}; //t is increased by epidemic functions...is still 0
    
    CHECK( population.current_situation() == Suno );
    
    while (population.current_situation().i != 0)
    {
        population = population.epidemic(ratInf, tMean);
        CHECK ( (population.current_situation().i + population.current_situation().s + population.current_situation().r) == S);
    }
    
}
