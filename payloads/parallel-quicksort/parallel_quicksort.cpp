#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

// Função para trocar dois elementos em um vetor
void swap(vector<int>& arr, int i, int j) {
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

// Função para particionar o vetor
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr, i, j);
        }
    }

    swap(arr, i + 1, high);
    return i + 1;
}

// Função Quicksort paralela
void quicksort_parallel(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pivot = partition(arr, low, high);

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                quicksort_parallel(arr, low, pivot - 1);
            }
            #pragma omp section
            {
                quicksort_parallel(arr, pivot + 1, high);
            }
        }
    }
}

// Função de wrapper para chamar o Quicksort paralelo
void parallel_quicksort(vector<int>& arr) {
    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            quicksort_parallel(arr, 0, arr.size() - 1);
        }
    }
}

// Função para imprimir o vetor
void print_vector(const vector<int>& arr) {
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;
}

int main() {
    vector<int> arr = {9, 4, 2, 7, 1, 5, 3, 8, 6};

    cout << "Array antes da ordenação: ";
    print_vector(arr);

    parallel_quicksort(arr);

    cout << "Array após ordenação: ";
    print_vector(arr);

    return 0;
}