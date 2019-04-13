#include "knapsak.h"

Result Solver::twoClosed()
{
    std::vector<Data> data;
    for (int i = 0; i < size; i++)
        data.push_back(Data((-c[i] / (double)w[i]), w[i], i));
    double z = weightedMedian(data, W);

    std::vector<bool> x1(size);
    std::vector<bool> x2(size);
    int C1 = 0;
    int C2 = 0;
    for (int i = 0; i < size; i++)
    {
        int xi = data[i].idx;
        if (data[i].z < z)
        {
            x1[xi] = true;
            C1 += c[xi];
        }
        if (data[i].z == z)
        {
            x2[xi] = true;
            C2 += c[xi];
        }
    }

    return C1 > C2 ? Result(x1, C1) : Result(x2, C2 );
}

Result Solver::knapsakDP(int C, int e)
{
    std::vector<std::vector<int>> D;
    std::vector<std::vector<bool>> S;
    initDP(D, S, C, e);
   
    for (int j = 1; j <= size; j++)
    {
        std::vector<int> v(C, 0);
        std::vector<bool> s(C, false);
        for (int k = 0; k < C; k++)
        {
            v[k] = D[j - 1][k];
           // s[k] = S[j - 1][k];
        }
        D.push_back(v);
        S.push_back(s);

        int cj = c[j - 1];
        int wj = w[j - 1];
        for (int k = cj; k < C; k++)
        { 
            if (D[j - 1][k - cj] != std::numeric_limits<int>::max() && D[j - 1][k - cj] + wj <= std::min(W, D[j][k]))
            {
                D[j][k] = D[j - 1][k - cj] + wj;
                S[j][k] = true;
            }
        }
    }

    return getDPAnswer(D, S, C);
}

void Solver::initDP(std::vector<std::vector<int>>& D, std::vector<std::vector<bool>>& S, int& C, int e)
{
    int t = std::max(1.0, (double)C / (size * e));
    C = (2 * C / t) + 1;
    for (int i = 0; i < size; i++)
        c[i] = c[i] / t;

    S.push_back(std::vector<bool>(C, false));
    D.push_back(std::vector<int>(C, std::numeric_limits<int>::max()));
    D[0][0] = 0;
}

Result Solver::getDPAnswer(std::vector<std::vector<int>>& D, std::vector<std::vector<bool>>& S, int C)
{
    int maxc = 0;
    for (int k = 0; k < C; k++)
        if (D[size][k] < std::numeric_limits<int>::max())
            maxc = k;

    int res = 0;
    std::vector<bool> x(size, false);
    for (int j = size; j > 0; j--)
    {
        if (S[j][maxc])
        {
            x[j - 1] = true;
            maxc -= c[j - 1];
            res += c[j - 1];
        }
    }

    return Result(x, res);
}

Result Solver::approximationScheme(int e)
{
    Result S1 = twoClosed();
    
    Result S2 = knapsakDP(S1.C, e);
    return S1.C > S2.C ? S1 : S2;
}