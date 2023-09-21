#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <chrono>


using namespace std::chrono;
using namespace std;

void merge(int array[], int const left, int const mid,
           int const right)
{
    int const leftArrayLimit = mid - left + 1;
    int const rightArrayLimit = right - mid;

    // Create temp arrays
    auto *leftArray = new int[leftArrayLimit],
         *rightArray = new int[rightArrayLimit];

       
    for (auto i = 0; i < leftArrayLimit; i++)
        leftArray[i] = array[left + i];
    for (auto j = 0; j < rightArrayLimit; j++)
        rightArray[j] = array[mid + 1 + j];

    auto indexOfLeftArray = 0, indexOfRightArray = 0;
    int indexOfMergedArray = left;

    // Merge the temp arrays back into array[left..right]
    while (indexOfLeftArray < leftArrayLimit && indexOfRightArray < rightArrayLimit)
    {
        if (leftArray[indexOfLeftArray] <= rightArray[indexOfRightArray])
        {
            array[indexOfMergedArray] = leftArray[indexOfLeftArray];
            indexOfLeftArray++;
        }
        else
        {
            array[indexOfMergedArray] = rightArray[indexOfRightArray];
            indexOfRightArray++;
        }
        indexOfMergedArray++;
    }

    // Copy the remaining elements of
    // left[], if there are any
    while (indexOfLeftArray < leftArrayLimit)
    {
        array[indexOfMergedArray] = leftArray[indexOfLeftArray];
        indexOfLeftArray++;
        indexOfMergedArray++;
    }

    // Copy the remaining elements of
    // right[], if there are any
    while (indexOfRightArray < rightArrayLimit)
    {
        array[indexOfMergedArray] = rightArray[indexOfRightArray];
        indexOfRightArray++;
        indexOfMergedArray++;
    }
    delete[] leftArray;
    delete[] rightArray;
}

// begin is for left index and end is right index
// of the sub-array of arr to be sorted
void mergeSort(int array[], int const begin, int const end)
{
    if (begin >= end)
        return;

    int mid = begin + (end - begin) / 2;
    
    #pragma task firstprivate(array, begin, mid)
    mergeSort(array, begin, mid);

    #pragma task firstprivate(array, mid, end)
    mergeSort(array, mid + 1, end);


    #pragma omp taskwait
    merge(array, begin, mid, end);
}

int main()
{
    int const arraySize = 15000000;
    auto array = new int[arraySize];
    auto temp = new int[arraySize];

    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    #pragma omp parallel for
    for (int i = 0; i < arraySize; ++i)
    {
        array[i] = std::rand() % arraySize; // Generating random integers between 0 and 99
    }

    cout << omp_get_max_threads() << endl;

    auto start = high_resolution_clock::now();
    //omp_set_nested(1); // Enable nested parallelism, if available

    omp_set_num_threads(8);
    #pragma omp parallel
    {
        #pragma omp single
        mergeSort(array, 0, arraySize - 1);
    }
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(stop - start);
    cout << duration.count() << endl;

    cout << "Finished mergesort!" << endl;

    for(int i = 0; i < arraySize - 1; i++){
        if(array[i] > array[i + 1]){
            cout << "Sort error!" <<  endl;
            return 0;
        }
    }

    cout << "Sort Success!" << endl;
    return 0;
}
