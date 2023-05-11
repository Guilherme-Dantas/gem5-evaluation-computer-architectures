#include <iostream>
#include <cmath>
#include <random>

using namespace std;

int main() {
    int dots_quantity = 1000000000;

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> distribution_range(-1.0, 1.0);

    int dots_in_circle = 0;
    for (int i = 0; i < dots_quantity; i++) {
        double x = distribution_range(gen);
        double y = distribution_range(gen);
        if (sqrt(x*x + y*y) <= 1.0) {
            dots_in_circle++;
        }
    }

    double pi = 4.0 * dots_in_circle / dots_quantity;
    cout << "Estimated pi value: " << pi << endl;

    return 0;
}