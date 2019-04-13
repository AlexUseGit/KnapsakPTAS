#ifndef _KNAPSAK_H
#define _KNAPSAK_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <climits>
#include <math.h>
#include <ctime>

struct Data
{
	Data(double _z, int _w, int _idx) : z(_z), w(_w), idx(_idx) {}
    double z;
    int w;
    int idx;
};

struct Result
{
	Result(std::vector<bool> _x, int _C) : x(_x), C(_C) {}
    std::vector<bool> x;
    int C;

    void print();
};

void printVec(const std::vector<int>& v, const char* prefix);
void swap(std::vector<Data>& arr, int i, int j);
void insertionSort(std::vector<Data>& arr);
Data findMedian(const std::vector<Data>& arr, int first, int last);
double medianOfMedians(const std::vector<Data>& arr);
int partition(std::vector<Data>& arr, int& k);
int sumOf(const std::vector<Data>& w, int size);
double weightedMedian(std::vector<Data>& arr, int W);

class Solver
{
public:
    Solver(int size, int maxWeight, int maxPrice);
    void printTask();

    Result approximationScheme(int e);
    Result bruteforce();
private:
    
    Result twoClosed();
    Result knapsakDP(int C, int e);
    void initDP(std::vector<std::vector<int>>& D, std::vector<std::vector<bool>>& S, int& C, int e);
    Result getDPAnswer(std::vector<std::vector<int>>& D, std::vector<std::vector<bool>>& S, int C);

    void bruteForceInit(std::vector<std::vector<bool>>& A);
    bool calculateVariant(const std::vector<bool>& variant, int* sumPrice);

    void genTask(int maxWeight, int maxPrice);
    
    std::vector<int> w;
    std::vector<int> c;
    int W;
    int size;
};

#endif