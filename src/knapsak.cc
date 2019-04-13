#include "knapsak.h"

Solver::Solver(int size, int maxWeight, int maxPrice) : size(size)
{
    genTask(maxWeight, maxPrice);
}

void Solver::genTask(int maxWeight, int maxPrice)
{
    srand(time(NULL));
    int sum = 0;
    for (int i = 0; i < size; i++)
    {
        int weight = 1 + rand() % maxWeight;
        int price = 1 + rand() % maxPrice;
        sum += weight;
        w.push_back(weight);
        c.push_back(price);
    }

    W = sum / 2;

    /*w = { 6,2,7,6,8,7,4,10,6,7 };
    c = { 3,5,4,9,10,10,7,4,2,10 };

    W = 31;*/
}

void Solver::printTask()
{
    printVec(w, "w = ");
    printVec(c, "c = ");
    std::cout << "W = " << W;
}