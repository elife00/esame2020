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
    double density[5] = {0.15, 0.25, 0.5, 0.75, 0.95};
    double ratInf[5] = {0.001, 0.05, 0.01, 0.2, 0.0003};
    int tMean[5] = {8, 10, 15, 20, 32};
    
    for (int i=0; i!=5; ++i)
    {
            double d = density[i];
            double r = ratInf[i];
            int S = (int) dim * dim * d;
            
            Board population(dim, d);
            Situation So = {0,S,0,0};
            CHECK( population.current_situation() == So );
            population.infection(r);
            while (population.current_situation().i != 0)
            {
                population = population.epidemic(r, tMean[i]);
                CHECK ( (population.current_situation().i + population.current_situation().s + population.current_situation().r) == S);
            }
        }
    }

TEST_CASE("testing epidemic_range")
{
    int dim = 100;
    double density[5] = {0.15, 0.25, 0.5, 0.75, 0.95};
    double ratInf[5] = {0.001, 0.05, 0.01, 0.2, 0.0003};
    int tMean[5] = {8, 10, 15, 20, 32};
    int range[5] = {5, 3, 2, 1, 4};
    for (int i=0; i!=5; ++i)
    {
            double d = density[i];
            double r = ratInf[i];
            int S = (int) dim * dim * d;
            
            Board population(dim, d);
            Situation So = {0,S,0,0};
            CHECK( population.current_situation() == So );
            population.infection(r);
            while (population.current_situation().i != 0)
            {
                population = population.epidemic_range(r, tMean[i], range[i]);
                CHECK ( (population.current_situation().i + population.current_situation().s + population.current_situation().r) == S);
            }
        }
    }

TEST_CASE("testing contact_range")
{
    int a = 5;
    int b = 1;
    int c = 3;
    int d = 4;
    Board population(5, 1);
    population.set(a,a,I);
    population.set(b,b,I);
    population.set(b,a,I);
    population.set(a,b,I);
    population.set(c,d,I);
    population.set(d,c,I);
    
    CHECK (population.contact_range(3,3,1) == 2);
    CHECK (population.contact_range(3,3,2) == 6);
}
