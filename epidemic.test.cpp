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
    std::uniform_int_distribution<> int_dis(1, 39);
    
    int dim = 100;
    double density = real_dis(gen);
    double ratInf = real_dis(gen);
    int tMean = int_dis(gen);
    int S = (int) dim * dim * density;
    int I = (int) dim * dim * density * ratInf;

    Board population(dim, density);
    
    Situation So = {0, S , 0, 0};
    
    CHECK( population.current_situation() == So );
    
    std::cout << population.current_situation().t << ' ' << population.current_situation().s << ' ' << population.current_situation().i << '\n';
    
    population.infection(ratInf);
    
     std::cout << population.current_situation().t << ' ' << population.current_situation().s << ' ' << population.current_situation().i << '\n';
    
   // Situation Suno = {0, S-I , I, 0}; //t is increased by epidemic functions...is still 0
   
    CHECK( population.current_situation().s == S-I );
    CHECK( population.current_situation().i == I );
    
    while (population.current_situation().i != 0)
    {
        population = population.epidemic(ratInf, tMean);
        CHECK ( (population.current_situation().i + population.current_situation().s + population.current_situation().r) == S);
    }
    
}

TEST_CASE("testing epidemic_range")
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> real_dis(0., 1.0);
    std::uniform_int_distribution<> int_dis(1, 39);
    std::uniform_int_distribution<> int_dis2(1, 5);
    
    int dim = 100;
    int range = int_dis2(gen);
    int avg_time = int_dis(gen);
    double ratInf = real_dis(gen);
    double density = real_dis(gen);
    
    int S = (int) dim * dim * density;
    int I = (int) dim * dim * density * ratInf;
    
    Board population(dim, density);
    
    Situation So = {0, S , 0, 0};
    CHECK( population.current_situation() == So );
    
    std::cout << population.current_situation().t << ' ' << population.current_situation().s << ' ' << population.current_situation().i << '\n';
    
    population.infection(ratInf);
    
     std::cout << population.current_situation().t << ' ' << population.current_situation().s << ' ' << population.current_situation().i << '\n';
    
    CHECK( population.current_situation().s == S-I );
    CHECK( population.current_situation().i == I );
    
    while (population.current_situation().i != 0)
    {
        population = population.epidemic_range(ratInf, avg_time, range);
        CHECK ( (population.current_situation().i + population.current_situation().s + population.current_situation().r) == S);
    }
}
