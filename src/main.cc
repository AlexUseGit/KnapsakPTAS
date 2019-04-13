#include <conio.h>
#include "knapsak.h"

int main()
{
    int taskCount = 1;
    int taskSize = 10;
    int maxWeight = 10;
    int maxPrice = 10;
    int e = 10000;

    double timeAS = 0;
    double timeBF = 0;
    int errorCount = 0;
    for (int task = 0; task < taskCount; task++)
    {
        Solver solver(taskSize, maxWeight, maxPrice);
     //   std::cout << "=========Task[" << task << "]==========" << std::endl;
     //   solver.printTask();

      //  std::cout << std::endl << "=========== BF ==========" << std::endl;
        clock_t beginBF = clock();
        Result bf = solver.bruteforce();
        clock_t endBF = clock();
        timeBF += double(endBF - beginBF);
    //    bf.print();

    //    std::cout << std::endl << "=========== AS ==========" << std::endl;
        clock_t beginAS = clock();
        Result as = solver.approximationScheme(e);
        clock_t endAS = clock();
        timeAS += double(endAS - beginAS);
    //    as.print();
     //   std::cout << std::endl << std::endl << std::endl << std::endl;

        if (bf.C != as.C)
        {
            errorCount++;
        }
    }

    std::cout << "AS Time = " << timeAS / CLOCKS_PER_SEC << std::endl;
    std::cout << "BF Time = " << timeBF / CLOCKS_PER_SEC << std::endl;
    std::cout << "Error count = " << errorCount;
    _getch();
    return 0;
}
