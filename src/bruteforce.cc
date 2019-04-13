#include "knapsak.h"

Result Solver::bruteforce()
{
    std::vector<std::vector<bool>> A;
    bruteForceInit(A);

    Result r{ std::vector<bool>(size, 0), 0 };
    for (int i = 0; i < A.size(); i++)
    {
        int sumPrice = 0;
        if (calculateVariant(A[i], &sumPrice) && sumPrice > r.C)
        {
            r.C = sumPrice;
            r.x = A[i];
        }
    }

    return r;
}

bool Solver::calculateVariant(const std::vector<bool>& variant, int* sumPrice)
{
    int sumWeight = 0;
    for (int j = 0; j < variant.size(); j++)
    {
        if (variant[j])
        {
            sumWeight += w[j];
            *sumPrice += c[j];
            if (sumWeight > W)
                return false;
        }
    }
    return true;
}

void Solver::bruteForceInit(std::vector<std::vector<bool>>& A)
{
    std::vector<int> masks;
    for (int i = 0; i < size; i++)
        masks.push_back(pow(2, i));

    int variantsCount = pow(2, size);
    for (int i = 0; i < variantsCount; i++)
    {
        std::vector<bool> tmp;
        for (int j = size - 1; j >= 0; j--)
            tmp.push_back(masks[j] & i);
        A.push_back(tmp);
    }
}