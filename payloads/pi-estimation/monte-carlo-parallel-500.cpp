#include <iostream>
#include <cmath>
#include <random>
#include <omp.h>
#include <chrono>

using namespace std;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

int main()
{
    long long int dots_quantity = 500;

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> distribution_range(-1.0, 1.0);

    long long int dots_in_circle = 0;

    auto start = high_resolution_clock::now();

    #pragma omp parallel for reduction(+ : dots_in_circle)
    for (int i = 0; i < dots_quantity; i++)
    {
        double x = distribution_range(gen);
        double y = distribution_range(gen);
        if (sqrt(x * x + y * y) <= 1.0)
        {
            dots_in_circle++;
        }
    }

    std::cout << "Total dots in circle: " << dots_in_circle << endl;

    double pi = 4.0 * dots_in_circle / dots_quantity;

    auto end = high_resolution_clock::now();
    auto execution_time_ms = duration_cast<milliseconds>(end - start);

    std::cout << "Estimated pi value: " << pi << " in " << execution_time_ms.count() << "ms" << endl;
    return 0;
}