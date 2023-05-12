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
    int dots_quantity = 1000000000000000;

    int num_threads = 4;

    random_device rd;                           // Gerador de números aleatórios
    mt19937 gen(rd());                          // Gerador de números aleatórios com seed aleatória
    uniform_real_distribution<> dis(-1.0, 1.0); // Distribuição uniforme entre -1 e 1

    int total_dots_in_circle = 0;

    auto start = high_resolution_clock::now();

    #pragma omp parallel num_threads(num_threads)
    {
        int thread_id = omp_get_thread_num();
        std::cout << "Thread " << thread_id << " executing."
                  << endl;

        int local_dots_in_circle = 0;
        #pragma omp for
        for (int i = 0; i < dots_quantity; i++)
        {
            double x = dis(gen);
            double y = dis(gen);
            if (sqrt(x * x + y * y) <= 1.0)
            {
                local_dots_in_circle++;
            }
        }

        #pragma omp atomic
        total_dots_in_circle += local_dots_in_circle;
    }

    double pi = 4.0 * total_dots_in_circle / dots_quantity;
    auto end = high_resolution_clock::now();
    auto execution_time_ms = duration_cast<milliseconds>(end - start);

    std::cout << "Estimated pi value: " << pi << " in " << execution_time_ms.count() << "ms" << endl;
    return 0;
}