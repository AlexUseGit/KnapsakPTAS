#include "knapsak.h"

void printVec(const std::vector<int>& v, const char* prefix)
{
    std::cout << prefix << "{";
    for (int i : v)
        std::cout << i << " ";

    std::cout << "}" << std::endl;
}

void Result::print()
{
    std::cout << "x = { ";
    for (bool b : x)
        std::cout << b << " ";
    
    std::cout << "}" << std::endl;
    std::cout << "C(x) = " << C;
}

void swap(std::vector<Data>& arr, int i, int j)
{
    Data temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

void insertionSort(std::vector<Data>& arr)
{
    for (int i = 1; i < arr.size(); i++)
    {
        double key = arr[i].z;
        for (int j = i - 1; j >= 0; j--)
            if (arr[j].z > key)
            {
                arr[j + 1] = arr[j];
                arr[j].z = key;
            }
    }
}

Data findMedian(const std::vector<Data>& arr, int first, int last)
{
    std::vector<Data> v(arr.data() + first, arr.data() + last);
    insertionSort(v);
    int idx = v.size() / 2;
    if (v.size() % 2 != 0)
        return v[idx];
    return v[idx].z <= v[idx - 1].z ? v[idx] : v[idx - 1];
}

double medianOfMedians(const std::vector<Data>& arr)
{
    std::vector<Data> medians;
    for (int i = 0; (i + 5) < arr.size(); i += 5)
        medians.push_back(findMedian(arr, i, i + 5));
    medians.push_back(findMedian(arr, medians.size() * 5, arr.size()));
    if (medians.size() > 5)
        return medianOfMedians(medians);
    else
        return findMedian(medians, 0, medians.size()).z;
}

int partition(std::vector<Data>& arr, int& k)
{
    double key = medianOfMedians(arr);
    int i = 0;
    int j = arr.size() - 1;
    while (i < j)
    {
        if (arr[i].z == key && arr[j].z == key)
        {
            i++;
            //continue;
        }
        while (arr[i].z < key)
            i++;
        while (arr[j].z > key)
            j--;
        if (i < j)
            swap(arr, i, j);
    }
    for (int v = j - 1; v >= 0; v--)
    {
        if (arr[v].z < arr[j].z)
        {
            k = v;
            break;
        }
    }
    return j;
}

int sumOf(const std::vector<Data>& w, int size)
{
    int res = 0;
    for (int i = 0; i < size; i++)
        res += (w[i].w);
    return res;
}

double weightedMedian(std::vector<Data>& arr, int W)
{
    int k = -1;
    int l = partition(arr, k);

    std::vector<Data> left(arr.data(), arr.data() + (k + 1));
    std::vector<Data> right(arr.data() + (l + 1), arr.data() + arr.size());
    int leftSum = sumOf(arr, k + 1);
    int rightSum = sumOf(arr, l + 1);
    if (leftSum < W && rightSum >= W)
        return arr[l].z;
    else
        if (rightSum < W)
            return weightedMedian(right, W - rightSum);
        else
            return weightedMedian(left, W);
}