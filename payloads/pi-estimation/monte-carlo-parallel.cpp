#include <iostream>
#include <cmath>
#include <random>
#include <omp.h>

using namespace std;

int main()
{
    int dots_quantity = 1000000000;

    int num_threads = 2;

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(-1.0, 1.0);

    int dots_in_circle = 0;
    #pragma omp parallel num_threads(num_threads)
    {
        int thread_id = omp_get_thread_num();
        cout << "Thread " << thread_id << " is being used";

        int thread_dots_in_circle = 0;
        #pragma omp for
        for (int i = 0; i < dots_quantity; i++)
        {
            double x = dis(gen);
            double y = dis(gen);
            if (sqrt(x * x + y * y) <= 1.0)
            {
                thread_dots_in_circle++;
            }
        }

        #pragma omp atomic
        dots_in_circle += thread_dots_in_circle;
    }

    double pi = 4.0 * dots_in_circle / dots_quantity;
    cout << "Estimated pi value: " << pi << endl;

    return 0;
}